#include "negamax.h"

search_heuristics* create_search_heuristics() {
    search_heuristics* search_data = (search_heuristics*)malloc(sizeof(search_heuristics));
    // if(!search_data) {
    //     fprintf(stderr, "Memory allocation failed for Search Heueristics\n");
    //     exit(EXIT_FAILURE);
    // }
    return search_data;
}

void init_search_heuristics(search_heuristics* data) {
    data->ply = 0;
    data->nodes = 0;
    data->follow_pv = 0; 
    data->score_pv = 0;
    memset(data->pv_lenght, 0, sizeof(data->pv_lenght));
    memset(data->pv_table, 0, sizeof(data->pv_table));
    memset(data->killer_moves, 0, sizeof(data->killer_moves));
    memset(data->history_moves, 0, sizeof(data->history_moves));
}


int quiescence(Board* board, leaper_moves_masks* leaper_masks, slider_moves_masks* slider_masks, 
    search_heuristics* search_data, time_controls* time_info, 
    zoobrist_hash_keys* hash_keys, repetition_data* repetition_table, 
    int alpha, int beta) {
    if((search_data->nodes & 255) == 0) { // check every 2048 nodes for time up
        communicate(time_info);
    }

    if(search_data->ply && check_repetition(repetition_table, hash_keys)) {
        return 0; // draw score
    }

    if(search_data->ply >= MAX_PLY) {
        return evaluate(board);
    }
    search_data->nodes++; 

    int eval = evaluate(board);
    if(eval >= beta) {
        return beta;
    }
    if(eval > alpha) {
        alpha = eval;
    }
    
    Moves move_list[1];
    init_move_list(move_list);
    generate_moves(board, leaper_masks, slider_masks, move_list);

    sort_moves(move_list, board, search_data);

    for(int count = 0; count < move_list->count; count++) {
        copy_board(board);
        copy_board_hash_key(hash_keys);

        search_data->ply++;

        repetition_table->keys[repetition_table->index++] = hash_keys->board_hash_key;

        if(make_move(board, move_list->moves[count], only_captures, leaper_masks, slider_masks, hash_keys) == 0) {
            search_data->ply--;
            repetition_table->index--;
            continue;
        }

        int score = -quiescence(board, leaper_masks, slider_masks, search_data, time_info, hash_keys, repetition_table, -beta, -alpha);
        
        take_back(board);
        take_back_board_hash_key(hash_keys);
        search_data->ply--;
        repetition_table->index--;

        if(time_info->stopped) return 0;

        if(score >= beta){
            return beta;
        }
                
        if(score > alpha) {
            alpha = score;

        }
    }

    return alpha;
}

int negamax(Board* board, leaper_moves_masks* leaper_masks, slider_moves_masks* slider_masks, 
    search_heuristics* search_data, time_controls* time_info, zoobrist_hash_keys* hash_keys, 
    tag_hash* transposition_table, repetition_data* repetition_table, int alpha, int beta, int depth) {
    int score;
    int moves_searched = 0; // moves seaarch in the move list 
    
    // repetiotion check
    if(search_data->ply && check_repetition(repetition_table, hash_keys)) {
        return 0; // draw score
    }


    int hash_flag = HASH_FLAG_ALPHA; // default flag to alpha
    int pv_node = (beta - alpha) > 1;
    if(search_data->ply && 
        (score = read_hash_entry(transposition_table, hash_keys, search_data, alpha, beta, depth)) != NO_HASH_ENTRY &&
        !pv_node) {
        // if the move is not in the principal variation 
        // if the move is found in the hash table, return the score
        // return the score without searching further
            return score;
    }

    search_data->pv_lenght[search_data->ply] = search_data->ply;
    
    if((search_data->nodes & 255) == 0) { // check every 2048 nodes for time up
        communicate(time_info);
    }

    if(depth == 0) {
        // extends search tree to the point where the state has a good score
        return quiescence(board, leaper_masks, slider_masks, search_data, time_info, hash_keys, repetition_table, alpha, beta);
    }
    if(search_data->ply >= MAX_PLY) {
        return evaluate(board);
    }

    search_data->nodes++; // will be used later to reduced search space
    int legal_moves = 0; // for checkmate and stalemate detection

    int in_check = 0;
    int king_pos;
    if(board->side_to_move == white) {
        king_pos = get_least_significant_bit_index(board->pieces[K]);
        if(is_square_attacked(king_pos, board, leaper_masks, slider_masks)) {
            in_check = 1;
        }
    } else if(board->side_to_move == black) {
        king_pos = get_least_significant_bit_index(board->pieces[k]);
        if(is_square_attacked(king_pos, board, leaper_masks, slider_masks)) {
            in_check = 1;
        }
    }

    // increase search depth if the king has been exposed into the check
    if(in_check) {
        depth++;
    }

    Moves move_list[1];
    init_move_list(move_list);

    // NULL MOVE PRUNING
    if(depth >= REDUCTION + 1 && in_check == 0 && search_data->ply) { // dont do null move pruning if in check or at root node
        copy_board(board);
        copy_board_hash_key(hash_keys);

        search_data->ply++;

        repetition_table->keys[repetition_table->index++] = hash_keys->board_hash_key;
        
        if(board->en_passant_square != no_square) {
            hash_keys->board_hash_key ^= hash_keys->en_passant_keys[board->en_passant_square];
        }
        board->side_to_move ^= 1; // switch side to move
        hash_keys->board_hash_key ^= hash_keys->side_key; // update hash key for side to move change
        board->en_passant_square = no_square; // cant do en passant after null move

        score = -negamax(board, leaper_masks, slider_masks, search_data, time_info, hash_keys, transposition_table, repetition_table, -beta, -beta + 1, depth - REDUCTION - 1);
        
        search_data->ply--;
        repetition_table->index--;
        take_back(board);
        take_back_board_hash_key(hash_keys);

        if(time_info->stopped) {
            return 0;
        }

        if(score >= beta) {
            return beta;
        }
    }
    // ---------

    generate_moves(board, leaper_masks, slider_masks, move_list);

    if(search_data->follow_pv) { // if principle variation line is followed
        enable_pv_scoring(move_list, search_data);
    }

    sort_moves(move_list, board, search_data);

    for(int count = 0; count < move_list->count; count++) {
        copy_board(board);
        copy_board_hash_key(hash_keys);

        search_data->ply++;

        repetition_table->keys[repetition_table->index++] = hash_keys->board_hash_key;
        if(make_move(board, move_list->moves[count], all_moves, leaper_masks, slider_masks, hash_keys) == 0) {
            search_data->ply--;
            repetition_table->index--;
            continue;
        }

        legal_moves++;

        if(moves_searched == 0) { // use full window for PV MOVE
            score = -negamax(board, leaper_masks, slider_masks, search_data, time_info, hash_keys, transposition_table, repetition_table, -beta, -alpha, depth-1);
        } else { // LMR for non PV moves
            if(moves_searched >= FULL_DEPTH_MOVE && depth >= REDUCED_DEPTH_MOVE 
                && in_check == 0 
                && get_move_capture(move_list->moves[count]) == 0 
                && get_move_promoted(move_list->moves[count]) == 0) {
                score = -negamax(board, leaper_masks, slider_masks, search_data, time_info, hash_keys, transposition_table, repetition_table, -alpha -1, -alpha, depth - 2);
            } else {
                score = alpha + 1; // set score to be higher than alpha to enter the if condition below
            }

            if(score > alpha) { // if LMR fails, search at normal depth and kept the score bandwidth
                score = -negamax(board, leaper_masks, slider_masks, search_data, time_info, hash_keys, transposition_table, repetition_table, -alpha - 1, -alpha, depth-1);
                if(score > alpha && score < beta) { // if the move is good, search with full window and normal depth
                    score = -negamax(board, leaper_masks, slider_masks, search_data, time_info, hash_keys, transposition_table, repetition_table, -beta, -alpha, depth-1);
                }
            }
        }

        
        take_back(board);
        take_back_board_hash_key(hash_keys);

        if(time_info->stopped) {
            return 0;
        }

        search_data->ply--;
        repetition_table->index--;
        moves_searched++;

        if(score >= beta) {
            write_hash_entry(transposition_table, hash_keys, search_data, beta, depth, HASH_FLAG_BETA);

            if(get_move_capture(move_list->moves[count]) == 0) {
                search_data->killer_moves[1][search_data->ply] = search_data->killer_moves[0][search_data->ply];
                search_data->killer_moves[0][search_data->ply] = move_list->moves[count];
            }
            // store killer moves

            return beta;
        }

        if(score > alpha) {
            hash_flag = HASH_FLAG_EXACT;

            if(get_move_capture(move_list->moves[count]) == 0) {
                search_data->history_moves[get_move_piece(move_list->moves[count])][get_move_target(move_list->moves[count])] += depth;
            }
            // store history moves

            alpha = score;
            // write PV mvoe
            search_data->pv_table[search_data->ply][search_data->ply] = move_list->moves[count];

            // copy move from deeper ply into a current plys line
            for(int next_ply = search_data->ply + 1; next_ply < search_data->pv_lenght[search_data->ply+1]; next_ply++) {
                search_data->pv_table[search_data->ply][next_ply] = search_data->pv_table[search_data->ply + 1][next_ply];
            }

            search_data->pv_lenght[search_data->ply] = search_data->pv_lenght[search_data->ply + 1]; 

        }
    }

    if(legal_moves == 0) {
        if(in_check) { // mating score assuming closest distance to mating position
            return -MATE_VALUE + search_data->ply; // +ply for mate in how many moves
        } else {
            // stalemate
            return 0;
        }
    }
    write_hash_entry(transposition_table, hash_keys, search_data, alpha, depth, hash_flag); // store hash entry with exact score or alpha score

    return alpha;
}
