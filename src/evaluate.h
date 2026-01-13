#ifndef EVALUATE_H
#define EVALUATE_H

#include "board.h"
#include "move_generator.h"
#include "perft.h"
#include "bitboard.h"

extern const int material_score[12];
extern const int pawn_score[64];
extern const int pawn_score[64];
extern const int knight_score[64];
extern const int bishop_score[64];
extern const int rook_score[64];
extern const int king_score[64];
extern const int mirror_score[128];

int evaluate(Board* board);

#endif // EVALUATE_H