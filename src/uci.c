#include "uci.h"

int parse_move(char* move_string, Board* board, leaper_moves_masks* leaper_masks, slider_moves_masks* slider_masks) {
    Moves move_list[1];
    init_move_list(move_list);
    generate_moves(board, leaper_masks, slider_masks, move_list);

    int source_square = (move_string[0] - 'a') + (8 - (move_string[1] - '0')) * 8;
    int target_square = (move_string[2] - 'a') + (8 - (move_string[3] - '0')) * 8;
    int promoted_piece = no_piece;

    int move;
    for(int move_count = 0; move_count < move_list->count; move_count++) {
        move = move_list->moves[move_count];

        if(source_square == get_move_source(move) && target_square == get_move_target(move)) {
            promoted_piece = get_move_promoted(move);

            if(promoted_piece) {
                if((promoted_piece == Q || promoted_piece == q) && move_string[4] == 'q') {
                    return move;
                }
                else if((promoted_piece == R || promoted_piece == r) && move_string[4] == 'r') {
                    return move;
                }
                else if((promoted_piece == B || promoted_piece == b) && move_string[4] == 'b') {
                    return move;
                }
                else if((promoted_piece == N || promoted_piece == n) && move_string[4] == 'n') {
                    return move;
                }
                continue;
            } 

            return move; // legal move
        }
    }

    return 0; // illegal move
}

/*
for UCI commands are being executed in terminal
each command starts with "position"

to command user can pas FEN string
or FEN string + moves to make
*/

void parse_position(char* command, Board* board, leaper_moves_masks* leaper_masks, slider_moves_masks* slider_masks, zoobrist_hash_keys* hash_keys) {
    command += 9;
    char* current_char = command;

    // parse UCI startpos command
    if(strncmp(command, "startpos", 8) == 0) {
        parse_fen(start_position, board);
    } else { // parse UCI FEN command
        // make sure FEN is available within command string
        current_char = strstr(command, "fen");
        if(current_char == NULL) {
            parse_fen(start_position, board);
        } else {
            current_char += 4; // "fen" + " "
            parse_fen(current_char, board);
        }
    }

    init_board_hash_key(board, hash_keys); // initialize hash key after setting up the board

    current_char = strstr(command, "moves");
    if(current_char != NULL) {
        current_char += 6; // "moves" + " "
        while(*current_char) {
            int move = parse_move(current_char, board, leaper_masks, slider_masks);

            if(move == 0) {
                break;
            }

            make_move(board, move, all_moves, leaper_masks, slider_masks, hash_keys); // the board hash will be updated in make move function

            while(*current_char && *current_char != ' ') {
                current_char++;
            }
            current_char++;
        }
    }
    print_board(board);
}

/*
if the GUI doesn't send any time controls,
the engine will do infinity search until stopped by the GUI
or until it finds a checkmate
or until the max depht wich is 64 plies
*/

void parse_go(char* command, Board* board, leaper_moves_masks* leaper_masks, slider_moves_masks* slider_masks, search_heuristics* search_data, time_controls* time_info, zoobrist_hash_keys* hash_keys, tag_hash* transposition_table) {
    int depth = -1;
    char* current_char = NULL;

    if((current_char = strstr(command, "infinite"))) 
        depth = MAX_PLY; // effectively infinite
    
    if ((current_char = strstr(command,"binc")) && board->side_to_move == black)
        // parse black time increment
        time_info->inc = atoi(current_char + 5);

    if ((current_char = strstr(command,"winc")) && board->side_to_move == white)
        // parse white time increment
        time_info->inc = atoi(current_char + 5);

    if ((current_char = strstr(command,"wtime")) && board->side_to_move == white)
        // parse white time limit
        time_info->time = atoi(current_char + 6);

    if ((current_char = strstr(command,"btime")) && board->side_to_move == black)
        // parse black time limit
        time_info->time = atoi(current_char + 6);

    if ((current_char = strstr(command,"movestogo")))
        // parse number of moves to go
        time_info->movestogo = atoi(current_char + 10);

    if ((current_char = strstr(command,"movetime")))
        // parse amount of time allowed to spend to make a move
        time_info->movetime = atoi(current_char + 9);

    if((current_char = strstr(command, "depth"))) 
        depth = atoi(current_char + 6); // "depth "

    
    if(time_info->movetime != -1)
    {
        time_info->time = time_info->movetime; // set time equal to move time
        time_info->movestogo = 1; // set moves to go to 1
    }
    time_info->starttime = get_time_ms();

    // if time control is available
    if(time_info->time != -1)
    {
        // flag we're playing with time control
        time_info->timeset = 1;

        // set up timing
        time_info->time /= time_info->movestogo;
        
        // "illegal" (empty) move bug fix
        if (time_info->time > 1500) time_info->time -= 50;
        /*
            Engines dont want to use 100% of the remaining time, because small inaccuracies in timing or delays in processing could make the engine run out of time before it makes a move.
            Subtracting 50 ms ensures the engine stops slightly before the absolute limit.

            Engines at the beggining use more time to think at the beggining of the game, because the position is more complex
            and early mistakes cost more. Remaining time at the beggining is large and uci tells the engine to excptect 40 moves to be played with movestogo.
            So the engine can use more time to think at the beggining of the game.
        */
        
        // init stoptime
        time_info->stoptime = time_info->starttime + time_info->time + time_info->inc;
    }

    // if depth is not available
    if(depth == -1)
        // set depth to 64 plies (takes ages to complete...)
        depth = MAX_PLY;

    // print debug info
    printf("time:%d start:%d stop:%d depth:%d timeset:%d\n",
    time_info->time, time_info->starttime, time_info->stoptime, depth, time_info->timeset);

    search_position(depth, board, leaper_masks, slider_masks, search_data, time_info, hash_keys, transposition_table);
}


void uci_loop(Board* board, leaper_moves_masks* leaper_masks, slider_moves_masks* slider_masks, search_heuristics* search_data, time_controls* time_info, zoobrist_hash_keys* hash_keys, tag_hash* transposition_table) {
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    char input[2000];

    printf("id name cincinnatus\n");
    printf("id name Boyan\n");
    printf("uciok\n");

    while(1) {
        memset(input, 0, sizeof(input));

        fflush(stdout);
        if(!fgets(input, 2000, stdin)) {
            continue;
        }
        if(input[0] == '\n') {
            continue;
        }

        if(strncmp(input, "isready", 7) == 0) {
            printf("readyok\n");
            continue;
        }
        else if(strncmp(input, "position", 8) == 0) {
            parse_position(input, board, leaper_masks, slider_masks, hash_keys);
            // init_board_hash_key(board, hash_keys);
        }
        else if(strncmp(input, "ucinewgame", 10) == 0) {
            parse_position("position startpos", board, leaper_masks, slider_masks, hash_keys);
            // init_board_hash_key(board, hash_keys);
        }
        else if(strncmp(input, "go", 2) == 0) {
            parse_go(input, board, leaper_masks, slider_masks, search_data, time_info, hash_keys, transposition_table);
        }
        else if(strncmp(input, "quit", 4) == 0) {
            break;
        }
        else if(strncmp(input, "uci", 3) == 0) {
            // testing if the engine supports uci protocol
            printf("id name cincinnatus\n");
            printf("id name Boyan\n");
            printf("uciok\n");
        }
    }

}

void search_position(int depth, Board* board, leaper_moves_masks* leaper_masks, slider_moves_masks* slider_masks, search_heuristics* search_data, time_controls* time_info, zoobrist_hash_keys* hash_keys, tag_hash* transposition_table) {
    // clear helper data structure for search
    init_search_heuristics(search_data);
    int alpha = ALPHA;
    int beta = BETA;

    time_info->stopped = 0;

    for(int current_depth = 1; current_depth <= depth; current_depth++) {
        if(time_info->stopped) {
            // STOPS CALCULATING AND RETURNS BEST MOVE FOUND SO FAR
            break;
        }

        search_data->follow_pv = 1;

        int score = negamax(board, leaper_masks, slider_masks, search_data, time_info, hash_keys, transposition_table, alpha, beta, current_depth);      

        // window aspiration
        if(score <= ALPHA || score >= BETA) {
            alpha = ALPHA;
            beta = BETA;
            continue;
        } 
        alpha = score - WINDOW_VALUE;
        beta = score + WINDOW_VALUE;
        // -----

        printf("info score cp %d depth %d nodes %ld pv ", score, current_depth, search_data->nodes);
        for(int i = 0; i < search_data->pv_lenght[0]; i++) {
            printf("%s%s ", square_to_cordinates[get_move_source(search_data->pv_table[0][i])],
                            square_to_cordinates[get_move_target(search_data->pv_table[0][i])]);
        }
        printf("\n");
    }

    int source_square = get_move_source(search_data->pv_table[0][0]);
    int target_square = get_move_target(search_data->pv_table[0][0]);
    
    //printf("\n");
    printf("bestmove %s%s\n", square_to_cordinates[source_square], square_to_cordinates[target_square]);    
}