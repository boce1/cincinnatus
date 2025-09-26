#ifndef UCI_H
#define UCI_H

#include "board.h"
#include "bitboard.h"
#include "move_generator.h"
#include "negamax.h"
#include "zoobrist_hash.h"
#include <string.h>
#include <stdio.h>

int parse_move(char* move_string, Board* board, leaper_moves_masks* leaper_masks, slider_moves_masks* slider_masks);
void parse_position(char* command, Board* board, leaper_moves_masks* leaper_masks, slider_moves_masks* slider_masks, zoobrist_hash_keys* hash_keys);
void parse_go(char* command, Board* board, leaper_moves_masks* leaper_masks, slider_moves_masks* slider_masks, search_heuristics* search_data, time_controls* time_info, zoobrist_hash_keys* hash_keys); // called when GUI tells engine to calculate moves
void uci_loop(Board* board, leaper_moves_masks* leaper_masks, slider_moves_masks* slider_masks, search_heuristics* search_data, time_controls* time_info, zoobrist_hash_keys* hash_keys);
void search_position(int depth, Board* board, leaper_moves_masks* leaper_masks, slider_moves_masks* slider_masks, search_heuristics* search_data, time_controls* time_info, zoobrist_hash_keys* hash_keys);


/*
if the GUI doesn't send any time controls,
the engine will do infinity search until stopped by the GUI
or until it finds a checkmate
or until the max depht wich is 64 plies
*/

#endif // UCI_H