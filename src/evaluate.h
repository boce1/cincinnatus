#ifndef EVALUATE_H
#define EVALUATE_H

#include "board.h"
#include "move_generator.h"
#include "perft.h"
#include "bitboard.h"

#define DOUBLE_PAWN_PENALTY -20
#define ISOLATED_PAWN_PENALTY -10

#define SEMI_OPEN_FILE_SCORE 10

extern const int material_score[12];
extern const int pawn_score[64];
extern const int knight_score[64];
extern const int bishop_score[64];
extern const int rook_score[64];
extern const int king_score[64];
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

int evaluate(Board* board, evaluation_masks* eval_masks);

evaluation_masks* create_evaluation_masks();
void init_evaluation_masks(evaluation_masks* masks);

#endif // EVALUATE_H