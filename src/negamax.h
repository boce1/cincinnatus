#ifndef NEGAMAX_H
#define NEGAMAX_H

#include "board.h"
#include "move_generator.h"
#include "perft.h"
#include "bitboard.h"
#include "zoobrist_hash.h"
#include "time_control.h"
#include <stdint.h>

extern const int material_score[12];
extern const int pawn_score[64];
extern const int pawn_score[64];
extern const int knight_score[64];
extern const int bishop_score[64];
extern const int rook_score[64];
extern const int king_score[64];
extern const int mirror_score[128];

#define ALPHA -50000
#define BETA 50000
#define WINDOW_VALUE 50

#define KILLER_MOVE_SCORE_1 9000
#define KILLER_MOVE_SCORE_2 8000
#define CAPTURE_MOVE_SCORE 10000
#define PV_MOVE_SCORE 20000


#define FULL_DEPTH_MOVE 4
#define REDUCED_DEPTH_MOVE 3
#define REDUCTION 2 // for null move pruning


/*
        Most valuable victim, least valuebale piece table                        
    (Victims) Pawn Knight Bishop   Rook  Queen   King
  (Attackers)
        Pawn   105    205    305    405    505    605
      Knight   104    204    304    404    504    604
      Bishop   103    203    303    403    503    603
        Rook   102    202    302    402    502    602
       Queen   101    201    301    401    501    601
        King   100    200    300    400    500    600

*/

// Trianglular PV table (principle variation)
// Principle variation is a move that lies along the best line of play found during the search
// The idea is storing the best moves and in the next negamax iteration first check them because they are good candidates for prunning
// they are stored in 2d matrix and they are indexed by [ply][ply]
/*
    ex.
    PV line: move1 move2 move3 ...
    ply/ply 
       0  1  2  3  4 ....
    0  m1 m2 m3 m4 
    1  0  m2 m3 m4
    2  0  0  m3 m4
    3  0  0  0  m4 
    4
    at the given ply pv moves are stored from the point it starts searching

    ply 0: e2e4
    ply 1: e7e5
    ply 2: g1f3
    ply 3: b8c6
    ply 4: f1b5
    when the ply is 2 negamax must be able to find g1f3, b8c6, f1b5
*/



/*
  follow_pv is the flag that indicates whether the current search is following the principle variation line,
  it resets to 0 when a non-pv move is being searched,

  score_pv is the flag that if the searced moved is pv move (it is in the pv_table), 
  That move is ussually the best and thats why its being searched first,
  it is used for move ordering

  At the start of move generation at a node, 
  if follow_pv is set, enable_pv_scoring is called, 
  which sets score_pv if the PV move is present.
  In score_move, if score_pv is set and the move matches the PV move, 
  it scores it the highest and then set score_pv = 0 so only the first match gets the bonus.
  Using follow_pv and score_pv is the common practical patter in chess engines fore clarity and control.
  
*/

search_heuristics* create_search_heuristics();
void init_search_heuristics(search_heuristics* search_data);

int quiescence(Board* board, leaper_moves_masks* leaper_masks, slider_moves_masks* slider_masks, search_heuristics* search_data, time_controls* time_info, zoobrist_hash_keys* hash_keys, int alpha, int beta);
int negamax(Board* board, leaper_moves_masks* leaper_masks, slider_moves_masks* slider_masks, search_heuristics* search_data, time_controls* time_info, zoobrist_hash_keys* hash_keys, tag_hash* transposition_table, int alpha, int beta, int depth);

int evaluate(Board* board);

// most valuable victim least valuable attacker
extern const int mvv_lva[12][12];

void enable_pv_scoring(Moves* move_list, search_heuristics* search_data);
int score_move(int move, Board* board, search_heuristics* search_data);
void merge(Moves* move_list, int* move_scores, int left, int mid, int right);
void merge_sort(Moves* move_list, int* move_scores, int left, int right);
void sort_moves(Moves* move_list, Board* board, search_heuristics* search_data);
void print_move_scores(Moves* move_list, Board* board, search_heuristics* search_data);

#endif