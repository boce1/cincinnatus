#include "evaluate.h"

const int material_score[12] = {
    100, // white pawn score
    300, // white knight score
    350, // white bishop
    500, // white rook
    1000, // white queen
    10000, // white king
    -100, // black pawn score
    -300, // black knight score
    -350, // black bishop
    -500, // black rook
    -1000, // black queen
    -10000 // black king
};

const int pawn_score[64] = {
    90,  90,  90,  90,  90,  90,  90,  90,
    30,  30,  30,  40,  40,  30,  30,  30,
    20,  20,  20,  30,  30,  30,  20,  20,
    10,  10,  10,  20,  20,  10,  10,  10,
     5,   5,  10,  20,  20,   5,   5,   5,
     0,   0,   0,   5,   5,   0,   0,   0,
     0,   0,   0, -10, -10,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,   0
};

// knight positional score
const int knight_score[64] = {
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5,   0,   0,  10,  10,   0,   0,  -5,
    -5,   5,  20,  20,  20,  20,   5,  -5,
    -5,  10,  20,  30,  30,  20,  10,  -5,
    -5,  10,  20,  30,  30,  20,  10,  -5,
    -5,   5,  20,  10,  10,  20,   5,  -5,
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5, -10,   0,   0,   0,   0, -10,  -5
};

// bishop positional score
const int bishop_score[64] = {
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,   0,
     0,  20,   0,  10,  10,   0,  20,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,  10,   0,   0,   0,   0,  10,   0,
     0,  30,   0,   0,   0,   0,  30,   0,
     0,   0, -10,   0,   0, -10,   0,   0
};

const int rook_score[64] = {
    50,  50,  50,  50,  50,  50,  50,  50,
    50,  50,  50,  50,  50,  50,  50,  50,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,   0,  20,  20,   0,   0,   0

};

const int king_score[64] = {
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   5,   5,   5,   5,   0,   0,
     0,   5,   5,  10,  10,   5,   5,   0,
     0,   5,  10,  20,  20,  10,   5,   0,
     0,   5,  10,  20,  20,  10,   5,   0,
     0,   0,   5,  10,  10,   5,   0,   0,
     0,   5,   5,  -5,  -5,   0,   5,   0,
     0,   0,  10,   0, -15,   0,  15,   0
};

const int mirror_score[128] = {
	a1, b1, c1, d1, e1, f1, g1, h1,
	a2, b2, c2, d2, e2, f2, g2, h2,
	a3, b3, c3, d3, e3, f3, g3, h3,
	a4, b4, c4, d4, e4, f4, g4, h4,
	a5, b5, c5, d5, e5, f5, g5, h5,
	a6, b6, c6, d6, e6, f6, g6, h6,
	a7, b7, c7, d7, e7, f7, g7, h7,
	a8, b8, c8, d8, e8, f8, g8, h8
};

const int square_bonus_index[64] = {
    7, 7, 7, 7, 7, 7, 7, 7,
    6, 6, 6, 6, 6, 6, 6, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    4, 4, 4, 4, 4, 4, 4, 4,
    3, 3, 3, 3, 3, 3, 3, 3,
    2, 2, 2, 2, 2, 2, 2, 2,
    1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0
};

const int square_bonus[8] = {
    0,   // rank 1
    5,  // rank 2
    10,  // rank 3
    20,  // rank 4
    35,  // rank 5
    60,  // rank 6
    100,  // rank 7
    200   // rank 8
};

evaluation_masks* create_evaluation_masks() {
    evaluation_masks* masks = (evaluation_masks*)malloc(sizeof(evaluation_masks));
    return masks;
}

void init_evaluation_masks(evaluation_masks* masks) {
    // file masks
    uint64_t file_mask;
    for(int square = 0; square < 64; square++) {
        file_mask = 0ULL;
        int file = square % 8;
        for(int rank = 0; rank < 8; rank++) {
            set_bit(file_mask, rank * 8 + file);
        }
        masks->file_masks[square] = file_mask;
    }

    // rank masks
    uint64_t rank_mask;
    for(int square = 0; square < 64; square++) {
        rank_mask = 0ULL;
        int rank = square / 8;
        for(int file = 0; file < 8; file++) {
            set_bit(rank_mask, rank * 8 + file);
        }
        masks->rank_masks[square] = rank_mask;
    }

    // isolated pawn masks
    for(int rank = 0; rank < 8; rank++) {
        for(int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            int prev_file = file - 1;
            int next_file = file + 1;
            masks->isolated_masks[square] = 0ULL;
            if(file > 0) {
                masks->isolated_masks[square] |= masks->file_masks[rank * 8 + prev_file];
            }
            if(file < 7) {
                masks->isolated_masks[square] |= masks->file_masks[rank * 8 + next_file];
            }
        }
    }
    
    // passed white pawn masks
    for(int rank=0; rank < 8; rank++) {
        for(int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            masks->passed_pawn_masks[white][square] = 0ULL;
            masks->passed_pawn_masks[white][square] |= masks->file_masks[square];
            masks->passed_pawn_masks[white][square] |= masks->isolated_masks[square];
    
            for(int r = rank; r < 8; r++) {
                pop_bit(masks->passed_pawn_masks[white][square], r * 8 + file);
                pop_bit(masks->passed_pawn_masks[white][square], r * 8 + file - 1);
                pop_bit(masks->passed_pawn_masks[white][square], r * 8 + file + 1);
            }
        }
    }

    // passed black pawn masks
    for(int rank = 0; rank < 8; rank++) {
        for(int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            masks->passed_pawn_masks[black][square] = 0ULL;
            masks->passed_pawn_masks[black][square] |= masks->file_masks[square];
            masks->passed_pawn_masks[black][square] |= masks->isolated_masks[square];

            for(int r = rank; r >= 0; r--) {
                pop_bit(masks->passed_pawn_masks[black][square], r * 8 + file);
                pop_bit(masks->passed_pawn_masks[black][square], r * 8 + file - 1);
                pop_bit(masks->passed_pawn_masks[black][square], r * 8 + file + 1);
            }
        }
    }
}


int evaluate(Board* board, evaluation_masks* eval_masks) {
    int score = 0;
    uint64_t bitboard;
    int square;
    int pawns;
    for(int bb_piece = P; bb_piece <= k; bb_piece++) {
        bitboard = board->pieces[bb_piece];
        while(bitboard) {
            square = get_least_significant_bit_index(bitboard);
            score += material_score[bb_piece];

            // evaluate with positional score tables
            // early queen movement leads to loss
            switch(bb_piece) {
                // white
                case P:
                    score += pawn_score[square];

                    // double pawn penalty
                    pawns = count_bits(board->pieces[P] & eval_masks->file_masks[square]);
                    if(pawns > 1) {
                        score += DOUBLE_PAWN_PENALTY * (pawns - 1);
                    }

                    // isolated pawn penalty
                    if((board->pieces[P] & eval_masks->isolated_masks[square]) == 0ULL) {
                        score += ISOLATED_PAWN_PENALTY;
                    }

                    // passed pawn bonus
                    if((board->pieces[p] & eval_masks->passed_pawn_masks[white][square]) == 0ULL) {
                        score += square_bonus[square_bonus_index[square]];
                    }

                    break;
                case N:
                    score += knight_score[square];
                    break;
                case B:
                    score += bishop_score[square];
                    break;
                case R:
                    score += rook_score[square]; // positioan scores

                    // semi open file bonus
                    if((board->pieces[P] & eval_masks->file_masks[square]) == 0ULL) {
                        score += 2 * SEMI_OPEN_FILE_SCORE;
                    }

                    if(((board->pieces[P] | board->pieces[p]) & eval_masks->file_masks[square]) == 0ULL) {
                        score += SEMI_OPEN_FILE_SCORE;
                    }

                    break;
                case K:
                    score += king_score[square];
                    
                    // open file penalty for king
                    if((board->pieces[P] & eval_masks->file_masks[square]) == 0ULL) {
                        score -= 2 * SEMI_OPEN_FILE_SCORE;
                    }

                    if(((board->pieces[P] | board->pieces[p]) & eval_masks->file_masks[square]) == 0ULL) {
                        score -= SEMI_OPEN_FILE_SCORE;
                    }

                    break;


                // black
                case p:
                    score -= pawn_score[mirror_score[square]];

                    // double pawn penalty
                    pawns = count_bits(board->pieces[p] & eval_masks->file_masks[square]);
                    if(pawns > 1) {
                        score -= DOUBLE_PAWN_PENALTY * (pawns - 1);
                    }

                    // isolated pawn penalty
                    if((board->pieces[p] & eval_masks->isolated_masks[square]) == 0ULL) {
                        score -= ISOLATED_PAWN_PENALTY;
                    }

                    // passed pawn bonus
                    if((board->pieces[P] & eval_masks->passed_pawn_masks[black][square]) == 0ULL) {
                        score -= square_bonus[square_bonus_index[mirror_score[square]]];
                    }

                    break;
                case n:
                    score -= knight_score[mirror_score[square]];
                    break;
                case b:
                    score -= bishop_score[mirror_score[square]];
                    break;
                case r:
                    score -= rook_score[mirror_score[square]];

                    // semi open file bonus
                    if((board->pieces[p] & eval_masks->file_masks[square]) == 0ULL) {
                        score -= 2 * SEMI_OPEN_FILE_SCORE;
                    }

                    if(((board->pieces[P] | board->pieces[p]) & eval_masks->file_masks[square]) == 0ULL) {
                        score -= SEMI_OPEN_FILE_SCORE;
                    }

                    break;
                case k:
                    score -= king_score[mirror_score[square]];

                    // open file penalty for king
                    if((board->pieces[p] & eval_masks->file_masks[square]) == 0ULL) {
                        score += 2 * SEMI_OPEN_FILE_SCORE;
                    }

                    if(((board->pieces[P] | board->pieces[p]) & eval_masks->file_masks[square]) == 0ULL) {
                        score += SEMI_OPEN_FILE_SCORE;
                    }

                    break;
            }

            pop_bit(bitboard, square);
        }
    }

    if(board->side_to_move == white) {
        return score;
    } else if (board->side_to_move == black) {
        return -score;
    }
    return 0;
}