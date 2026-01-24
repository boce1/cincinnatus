#ifndef UCI_H
#define UCI_H

#include "board.h"
#include "bitboard.h"
#include "move_generator.h"
#include "negamax.h"
#include "zoobrist_hash.h"
#include "evaluate.h"
#include <string.h>
#include <stdio.h>

int parse_move(char* move_string, Board* board, leaper_moves_masks* leaper_masks, slider_moves_masks* slider_masks);

void parse_position(char* command, Board* board, leaper_moves_masks* leaper_masks, slider_moves_masks* slider_masks, 
    search_heuristics* search_data, zoobrist_hash_keys* hash_keys, repetition_data* repetition_table);

void parse_go(char* command, Board* board, leaper_moves_masks* leaper_masks, slider_moves_masks* slider_masks, 
    search_heuristics* search_data, time_controls* time_info, 
    zoobrist_hash_keys* hash_keys, tag_hash* transposition_table, repetition_data* repetition_table, evaluation_masks* eval_masks); // called when GUI tells engine to calculate moves

void uci_loop(Board* board, leaper_moves_masks* leaper_masks, slider_moves_masks* slider_masks, 
    search_heuristics* search_data, time_controls* time_info, 
    zoobrist_hash_keys* hash_keys, tag_hash* transposition_table, repetition_data* repetition_table, evaluation_masks* eval_masks);

void search_position(int depth, Board* board, leaper_moves_masks* leaper_masks, slider_moves_masks* slider_masks, 
    search_heuristics* search_data, time_controls* time_info, 
    zoobrist_hash_keys* hash_keys, tag_hash* transposition_table, repetition_data* repetition_table, evaluation_masks* eval_masks);
/*
if the GUI doesn't send any time controls,
the engine will do infinity search until stopped by the GUI
or until it finds a checkmate
or until the max depht wich is 64 plies
*/

void print_valid_uci_move(int move);

#endif // UCI_H