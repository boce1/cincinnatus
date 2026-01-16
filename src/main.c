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
    repetition_data* repetition_table = NULL;
    evaluation_masks* eval_masks = NULL;

    create_data_structures(&leaper_masks, &slider_masks, &board, &search_data, &time_info, &hash_data, &transposition_table, &repetition_table, &eval_masks);
    init_data_structures(leaper_masks, slider_masks, board, search_data, time_info, hash_data, transposition_table, repetition_table, eval_masks);

    int debug = 1; // set to 0 to run UCI loop
    if(debug) {

        parse_fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -", board);
        parse_fen(start_position, board);
        parse_fen(repetitions, board);
        // info score cp 0 depth 5 nodes 56624 pv e2a6 e6d5 c3d5 b6d5 e4d5
        // info score cp 0 depth 5 nodes 55960 pv e2a6 e6d5 c3d5 b6d5 e4d5 
        init_board_hash_key(board, hash_data);
        print_board(board);
        
        // write_hash_entry(transposition_table, hash_data, 6, 5, HASH_FLAG_BETA);
        // int score = read_hash_entry(transposition_table, hash_data, 5, 15, 5);
        // printf("Hash score: %d\n", score);
        // search_position(10, board, leaper_masks, slider_masks, search_data, time_info, hash_data, transposition_table, repetition_table);

        // perft_test(5, board, leaper_masks, slider_masks, hash_data);

        //uint64_t start = 0ULL;
        //set_bit(start, 0);
        //start <<= 61;
        //print_bitboard(start);

        for(int i = 0; i < 64; i++) {
            print_bitboard(eval_masks->passed_pawn_masks[black][i]);
        }

    } else {
        uci_loop(board, leaper_masks, slider_masks, search_data, time_info, hash_data, transposition_table, repetition_table);
    }

    free_data_structures(leaper_masks, slider_masks, board, search_data, time_info, hash_data, transposition_table, repetition_table, eval_masks);
    return 0;
}