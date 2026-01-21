#ifndef EVALUATE_H
#define EVALUATE_H

#include "board.h"
#include "move_generator.h"
#include "perft.h"
#include "bitboard.h"
#include "attack.h"

// game phases
enum { opening, endgame, middlegame };

// piece types
enum { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };


#define DOUBLE_PAWN_PENALTY -20
#define ISOLATED_PAWN_PENALTY -10

#define SEMI_OPEN_FILE_SCORE 10
#define KING_SHIELD_SCORE 5

#define OPENING_PHASE_SCORE 6192
#define ENDING_PHASE_SCORE 518

extern const int material_score[2][12];
extern const int positional_score[2][6][64]; // positional piece scores [game phase][piece][square]
extern const int mirror_score[128];

extern const int square_bonus_index[64];
extern const int square_bonus[8];

typedef struct {
    // 64 for squares
    uint64_t file_masks[64]; // masks for each file
    uint64_t rank_masks[64]; // masks for each rank
    uint64_t isolated_masks[64]; // masks for isolated pawns
    uint64_t passed_pawn_masks[2][64]; // masks for passed pawns
} evaluation_masks;

int evaluate(Board* board, leaper_moves_masks* leaper_masks, slider_moves_masks* slider_masks, evaluation_masks* eval_masks);

evaluation_masks* create_evaluation_masks();
void init_evaluation_masks(evaluation_masks* masks);


/*
    The game phase score of the game is derived from the pieces
    not counting pawns nad kings, that are still on the board
    The full material starting position game phase score is

    knights material in the oppening + 
    bishop material in the oppening + 
    rook material in the oppening + 
    queen material in the oppening
*/
int get_game_phase_score(Board* board);

#endif // EVALUATE_H