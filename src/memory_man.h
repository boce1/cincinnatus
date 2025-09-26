#ifndef MEMORY_MAN_H
#define MEMORY_MAN_H

#include "board.h"
#include "time_control.h"
#include "zoobrist_hash.h"
#include "bitboard.h"
#include "negamax.h"
#include <stdlib.h>
#include <stdio.h>

/*
leaper_moves_masks* leaper_masks, slider_moves_masks* slider_masks, Board* board, search_heuristics* search_data, time_controls* time_info, zoobrist_hash_keys* hash_data,
*/

void create_data_structures(leaper_moves_masks** leaper_masks, slider_moves_masks** slider_masks, Board** board, search_heuristics** search_data, time_controls** time_info, zoobrist_hash_keys** hash_data, tag_hash** transposition_table);
void free_data_structures(leaper_moves_masks* leaper_masks, slider_moves_masks* slider_masks, Board* board, search_heuristics* search_data, time_controls* time_info, zoobrist_hash_keys* hash_data, tag_hash* transposition_table);
void init_data_structures(leaper_moves_masks* leaper_masks, slider_moves_masks* slider_masks, Board* board, search_heuristics* search_data, time_controls* time_info, zoobrist_hash_keys* hash_data, tag_hash* transposition_table);


#endif // MEMORY_MAN_H