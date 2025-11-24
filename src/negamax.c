#include "negamax.h"

const int material_score[12] = {
    100, // white pawn score
    300, // white knight score
    350, // white bishop
    500, // white rook
    1000, // white queen
    10000, // white king
    -100, // black pawn score
    -300, // black knight score
    -350, // black bishop
    -500, // black rook
    -1000, // black queen
    -10000 // black king
};

const int pawn_score[64] = {
    90,  90,  90,  90,  90,  90,  90,  90,
    30,  30,  30,  40,  40,  30,  30,  30,
    20,  20,  20,  30,  30,  30,  20,  20,
    10,  10,  10,  20,  20,  10,  10,  10,
     5,   5,  10,  20,  20,   5,   5,   5,
     0,   0,   0,   5,   5,   0,   0,   0,
     0,   0,   0, -10, -10,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,   0
};

// knight positional score
const int knight_score[64] = {
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5,   0,   0,  10,  10,   0,   0,  -5,
    -5,   5,  20,  20,  20,  20,   5,  -5,
    -5,  10,  20,  30,  30,  20,  10,  -5,
    -5,  10,  20,  30,  30,  20,  10,  -5,
    -5,   5,  20,  10,  10,  20,   5,  -5,
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5, -10,   0,   0,   0,   0, -10,  -5
};

// bishop positional score
const int bishop_score[64] = {
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,   0,
     0,  20,   0,  10,  10,   0,  20,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,  10,   0,   0,   0,   0,  10,   0,
     0,  30,   0,   0,   0,   0,  30,   0,
     0,   0, -10,   0,   0, -10,   0,   0
};

const int rook_score[64] = {
    50,  50,  50,  50,  50,  50,  50,  50,
    50,  50,  50,  50,  50,  50,  50,  50,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,   0,  20,  20,   0,   0,   0

};

const int king_score[64] = {
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   5,   5,   5,   5,   0,   0,
     0,   5,   5,  10,  10,   5,   5,   0,
     0,   5,  10,  20,  20,  10,   5,   0,
     0,   5,  10,  20,  20,  10,   5,   0,
     0,   0,   5,  10,  10,   5,   0,   0,
     0,   5,   5,  -5,  -5,   0,   5,   0,
     0,   0,  10,   0, -15,   0,  15,   0
};

const int mirror_score[128] = {
	a1, b1, c1, d1, e1, f1, g1, h1,
	a2, b2, c2, d2, e2, f2, g2, h2,
	a3, b3, c3, d3, e3, f3, g3, h3,
	a4, b4, c4, d4, e4, f4, g4, h4,
	a5, b5, c5, d5, e5, f5, g5, h5,
	a6, b6, c6, d6, e6, f6, g6, h6,
	a7, b7, c7, d7, e7, f7, g7, h7,
	a8, b8, c8, d8, e8, f8, g8, h8
};

// MVV LVA [attacker][victim]
//    P,   N,   B,   R,   Q,   K,    p,   n,   b,   r,   q,   k
int const mvv_lva[12][12] = {
 	{105, 205, 305, 405, 505, 605,  105, 205, 305, 405, 505, 605}, // P
	{104, 204, 304, 404, 504, 604,  104, 204, 304, 404, 504, 604}, // N
	{103, 203, 303, 403, 503, 603,  103, 203, 303, 403, 503, 603}, // B
	{102, 202, 302, 402, 502, 602,  102, 202, 302, 402, 502, 602}, // R
	{101, 201, 301, 401, 501, 601,  101, 201, 301, 401, 501, 601}, // Q
	{100, 200, 300, 400, 500, 600,  100, 200, 300, 400, 500, 600}, // K

	{105, 205, 305, 405, 505, 605,  105, 205, 305, 405, 505, 605}, // p
	{104, 204, 304, 404, 504, 604,  104, 204, 304, 404, 504, 604}, // n
	{103, 203, 303, 403, 503, 603,  103, 203, 303, 403, 503, 603}, // b
	{102, 202, 302, 402, 502, 602,  102, 202, 302, 402, 502, 602}, // r
	{101, 201, 301, 401, 501, 601,  101, 201, 301, 401, 501, 601}, // q
	{100, 200, 300, 400, 500, 600,  100, 200, 300, 400, 500, 600}  // k
};

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

int score_move(int move, Board* board, search_heuristics* search_data) {
    if(search_data->score_pv) {
        if(search_data->pv_table[0][search_data->ply] == move) {
            search_data->score_pv = 0;
            return PV_MOVE_SCORE;
        }
    }

    if(get_move_capture(move)) {
        // captures scores with mvv_lva
        int target_piece = P; // if it isnt initialized en passant wont work, because doesnt containe the piiece on the square
        int target_square = get_move_target(move);

        int start_piece, end_piece;
        if(board->side_to_move == white) {
            start_piece = p;
            end_piece = k;
        } else if(board->side_to_move == black) {
            start_piece = P;
            end_piece = K;
        }

        for(int bb_piece = start_piece; bb_piece <= end_piece; bb_piece++ ) {
            if(get_bit(board->pieces[bb_piece], target_square)) {
                target_piece = bb_piece;
                break;
            }
        }

        return mvv_lva[get_move_piece(move)][target_piece] + CAPTURE_MOVE_SCORE;
    } else { // quiet move
        // score 1st killer move
        if(search_data->killer_moves[0][search_data->ply] == move) {
            return KILLER_MOVE_SCORE_1;
        }
        // score 2nd killer move
        else if(search_data->killer_moves[1][search_data->ply] == move) {
            return KILLER_MOVE_SCORE_2;
        }
        else  {
            return search_data->history_moves[get_move_piece(move)][get_move_target(move)];
        }
        // score history move

        return 0;
    }

    return 0;
}

void merge(Moves* move_list, int* move_scores, int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int left_moves[n1], right_moves[n2];
    int left_scores[n1], right_scores[n2];
    
    for(i = 0; i < n1; i++) {
        left_moves[i] = move_list->moves[left + i];
        left_scores[i] = move_scores[left + i];
    }
    for(j = 0; j < n2; j++) {
        right_moves[j] = move_list->moves[mid + 1 + j];
        right_scores[j] = move_scores[mid + 1 + j];
    }
    i = 0;
    j = 0;
    k = left;
    while(i < n1 && j < n2) {
        if(left_scores[i] >= right_scores[j]) {
            move_list->moves[k] = left_moves[i];
            move_scores[k] = left_scores[i];
            i++;
        } else {
            move_list->moves[k] = right_moves[j];
            move_scores[k] = right_scores[j];
            j++;
        }
        k++;
    }

    while(i < n1) {
        move_list->moves[k] = left_moves[i];
        move_scores[k] = left_scores[i];
        i++;
        k++;
    }
    while(j < n2) {
        move_list->moves[k] = right_moves[j];
        move_scores[k] = right_scores[j];
        j++;
        k++;
    }
}

void merge_sort(Moves* move_list, int* move_scores, int left, int right) {
    if(left < right) {
        int mid = left + (right - left) / 2;
        merge_sort(move_list, move_scores, left, mid);
        merge_sort(move_list, move_scores, mid + 1, right);
        merge(move_list, move_scores, left, mid, right);
    }
}

void sort_moves(Moves* move_list, Board* board, search_heuristics* search_data) {
    int move_scores[move_list->count];
    for(int i = 0; i < move_list->count; i++) {
        move_scores[i] = score_move(move_list->moves[i], board, search_data);
    }

    merge_sort(move_list, move_scores, 0, move_list->count - 1);
}

void enable_pv_scoring(Moves* move_list, search_heuristics* search_data) {
    search_data->follow_pv = 0;
    
    for(int count = 0; count < move_list->count; count++) {
        if(search_data->pv_table[0][search_data->ply] == move_list->moves[count]) {
            search_data->score_pv = 1;
            search_data->follow_pv = 1;
            break;
        }
    }
}

int evaluate(Board* board) {
    int score = 0;
    uint64_t bitboard;
    int square;

    for(int bb_piece = P; bb_piece <= k; bb_piece++) {
        bitboard = board->pieces[bb_piece];
        while(bitboard) {
            square = get_least_significant_bit_index(bitboard);
            score += material_score[bb_piece];

            // evaluate with positional score tables
            // early queen movement leads to loss
            switch(bb_piece) {
                // white
                case P:
                    score += pawn_score[square];
                    break;
                case N:
                    score += knight_score[square];
                    break;
                case B:
                    score += bishop_score[square];
                    break;
                case R:
                    score += rook_score[square];
                    break;
                case K:
                    score += king_score[square];
                    break;
                // black
                case p:
                    score -= pawn_score[mirror_score[square]];
                    break;
                case n:
                    score -= knight_score[mirror_score[square]];
                    break;
                case b:
                    score -= bishop_score[mirror_score[square]];
                    break;
                case r:
                    score -= rook_score[mirror_score[square]];
                    break;
                case k:
                    score -= king_score[mirror_score[square]];
                    break;
            }

            pop_bit(bitboard, square);
        }
    }

    if(board->side_to_move == white) {
        return score;
    } else if (board->side_to_move == black) {
        return -score;
    }
    return 0;
}

int quiescence(Board* board, leaper_moves_masks* leaper_masks, slider_moves_masks* slider_masks, search_heuristics* search_data, time_controls* time_info, zoobrist_hash_keys* hash_keys, int alpha, int beta) {
    if((search_data->nodes & 2047) == 0) { // check every 2048 nodes for time up
        communicate(time_info);
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
        if(make_move(board, move_list->moves[count], only_captures, leaper_masks, slider_masks, hash_keys) == 0) {
            search_data->ply--;
            continue;
        }

        int score = -quiescence(board, leaper_masks, slider_masks, search_data, time_info, hash_keys, -beta, -alpha);
        take_back(board);
        take_back_board_hash_key(hash_keys);
        search_data->ply--;

        if(score >= beta){
            return beta;
        }

        if(score > alpha) {
            alpha = score;

        }
    }

    return alpha;
}

int negamax(Board* board, leaper_moves_masks* leaper_masks, slider_moves_masks* slider_masks, search_heuristics* search_data, time_controls* time_info, zoobrist_hash_keys* hash_keys, tag_hash* transposition_table, int alpha, int beta, int depth) {
    int score;
    int moves_searched = 0; // moves seaarch in the move list 
    
    int hash_flag = HASH_FLAG_ALPHA; // default flag to alpha
    if((score = read_hash_entry(transposition_table, hash_keys, alpha, beta, depth)) != NO_HASH_ENTRY) {
        // if the move is found in the hash table, return the score
        // return the score without searching further
        return score;
    }

    search_data->pv_lenght[search_data->ply] = search_data->ply;
    
    if((search_data->nodes & 2047) == 0) { // check every 2048 nodes for time up
        communicate(time_info);
    }

    if(depth == 0) {
        // extends search tree to the point where the state has a good score
        return quiescence(board, leaper_masks, slider_masks, search_data, time_info, hash_keys, alpha, beta);
    }
    if(search_data->ply >= MAX_PLY) {
        return evaluate(board);
    }

    search_data->nodes++; // will be used later to reduced search space
    int legal_moves = 0;

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
        board->side_to_move ^= 1; // switch side to move
        board->en_passant_square = no_square; // cant do en passant after null move
        hash_keys->board_hash_key ^= hash_keys->side_key; // update hash key for side to move change
        hash_keys->board_hash_key ^= hash_keys->en_passant_keys[board->en_passant_square];
        score = -negamax(board, leaper_masks, slider_masks, search_data, time_info, hash_keys, transposition_table, -beta, -beta + 1, depth - REDUCTION - 1);
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
        if(make_move(board, move_list->moves[count], all_moves, leaper_masks, slider_masks, hash_keys) == 0) {
            search_data->ply--;
            continue;
        }

        legal_moves++;

        if(moves_searched == 0) { // use full window for PV MOVE
            score = -negamax(board, leaper_masks, slider_masks, search_data, time_info, hash_keys, transposition_table, -beta, -alpha, depth-1);
        } else { // LMR for non PV moves
            if(moves_searched >= FULL_DEPTH_MOVE && depth >= REDUCED_DEPTH_MOVE 
                && in_check == 0 
                && get_move_capture(move_list->moves[count]) == 0 
                && get_move_promoted(move_list->moves[count]) == 0) {
                score = -negamax(board, leaper_masks, slider_masks, search_data, time_info, hash_keys, transposition_table, -alpha -1, -alpha, depth - 2);
            } else {
                score = alpha + 1; // set score to be higher than alpha to enter the if condition below
            }

            if(score > alpha) { // if LMR fails, search at normal depth and kept the score bandwidth
                score = -negamax(board, leaper_masks, slider_masks, search_data, time_info, hash_keys, transposition_table, -alpha - 1, -alpha, depth-1);
                if(score > alpha && score < beta) { // if the move is good, search with full window and normal depth
                    score = -negamax(board, leaper_masks, slider_masks, search_data, time_info, hash_keys, transposition_table, -beta, -alpha, depth-1);
                }
            }
        }

        
        take_back(board);
        take_back_board_hash_key(hash_keys);

        if(time_info->stopped) {
            return 0;
        }

        search_data->ply--;
        moves_searched++;

        if(score >= beta) {
            write_hash_entry(transposition_table, hash_keys, beta, depth, HASH_FLAG_BETA);

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
            return ALPHA + 1 + search_data->ply;
        } else {
            // stalemate
            return 0;
        }
    }
    write_hash_entry(transposition_table, hash_keys, alpha, depth, hash_flag); // store hash entry with exact score or alpha score

    return alpha;
}



// -----------
void print_move_scores(Moves* move_list, Board* board, search_heuristics* search_data) {
    // debugging purpose
    // to see if sort move function works correcly
    for(int i = 0; i < move_list->count; i++) {
        print_move(move_list->moves[i]);
        printf("move score: %d\n\n", score_move(move_list->moves[i], board, search_data));
    }
}