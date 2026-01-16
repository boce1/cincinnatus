#ifndef MEMORY_MAN_H
#define MEMORY_MAN_H

#include "board.h"
#include "time_control.h"
#include "zoobrist_hash.h"
#include "bitboard.h"
#include "negamax.h"
#include "evaluate.h"
#include <stdlib.h>
#include <stdio.h>


void create_data_structures(leaper_moves_masks** leaper_masks, slider_moves_masks** slider_masks, 
    Board** board, search_heuristics** search_data, time_controls** time_info, zoobrist_hash_keys** hash_data, 
    tag_hash** transposition_table, repetition_data** repetition_table, evaluation_masks** eval_masks);

void free_data_structures(leaper_moves_masks* leaper_masks, slider_moves_masks* slider_masks, 
    Board* board, search_heuristics* search_data,time_controls* time_info, zoobrist_hash_keys* hash_data, 
    tag_hash* transposition_table, repetition_data* repetition_table, evaluation_masks* eval_masks);

void init_data_structures(leaper_moves_masks* leaper_masks, slider_moves_masks* slider_masks, 
    Board* board, search_heuristics* search_data, time_controls* time_info, zoobrist_hash_keys* hash_data, 
    tag_hash* transposition_table, repetition_data* repetition_table, evaluation_masks* eval_masks);

#endif // MEMORY_MAN_H