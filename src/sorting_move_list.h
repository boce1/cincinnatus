#ifndef SORTING_MOVE_LIST_H
#define SORTING_MOVE_LIST_H

#include "board.h"
#include "move_generator.h"
#include "perft.h"
#include "bitboard.h"

#define KILLER_MOVE_SCORE_1 9000
#define KILLER_MOVE_SCORE_2 8000
#define CAPTURE_MOVE_SCORE 10000
#define PV_MOVE_SCORE 20000

// most valuable victim least valuable attacker
extern const int mvv_lva[12][12];

void enable_pv_scoring(Moves* move_list, search_heuristics* search_data);
int score_move(int move, Board* board, search_heuristics* search_data);
void merge(Moves* move_list, int* move_scores, int left, int mid, int right);
void merge_sort(Moves* move_list, int* move_scores, int left, int right);
void sort_moves(Moves* move_list, Board* board, search_heuristics* search_data);
void print_move_scores(Moves* move_list, Board* board, search_heuristics* search_data);

#endif // SORTING_MOVE_LIST_H