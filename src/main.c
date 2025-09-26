#include "bitboard.h"
#include "magic_numbers.h"
#include "attack.h"
#include "board.h"
#include "move_generator.h"
#include "uci.h"
#include "negamax.h"
#include "time_control.h"
#include "zoobrist_hash.h"
#include "perft.h"
#include "memory_man.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    leaper_moves_masks* leaper_masks = NULL;
    slider_moves_masks* slider_masks = NULL;
    Board* board = NULL;
    search_heuristics* search_data = NULL;
    time_controls* time_info = NULL;   
    zoobrist_hash_keys* hash_data = NULL;
    tag_hash* transposition_table = NULL;

    create_data_structures(&leaper_masks, &slider_masks, &board, &search_data, &time_info, &hash_data, &transposition_table);
    init_data_structures(leaper_masks, slider_masks, board, search_data, time_info, hash_data, transposition_table);

    int debug = 0; // set to 0 to run UCI loop
    if(debug) {

        parse_fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -", board);
        parse_fen(start_position, board);

        init_board_hash_key(board, hash_data);
        print_board(board);
        
        // print_hash_key(board, hash_data);
        // printf("Board hash key: %" PRIu64 "\n", hash_data->board_hash_key);
        perft_test(4, board, leaper_masks, slider_masks, hash_data);
        //print_hash_key(board, hash_data);

        //search_position(6, board, leaper_masks, slider_masks, search_data, time_info);
        //Moves mv[1];
        //init_move_list(mv);
        //generate_moves(board, leaper_masks, slider_masks, mv);
//
        //sort_moves(mv, board, search_data);
        //print_move_scores(mv, board, search_data);        
        //printf("score %d\n", evaluate(board));

        // 7986424829486667136
        // 7986424829486667136
    } else {
        uci_loop(board, leaper_masks, slider_masks, search_data, time_info, hash_data);
    }
    
    free_data_structures(leaper_masks, slider_masks, board, search_data, time_info, hash_data, transposition_table);
    return 0;
}