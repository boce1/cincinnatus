#include "evaluate.h"

const int material_score[2][12] =
{
//  P    N    B    R    Q     K      p     n     b     r    q        k 
    // opening material score     
    { 82, 337, 365, 477, 1025, 12000, -82, -337, -365, -477, -1025, -12000 },
    
    // endgame material score
    { 94, 281, 297, 512,  936, 12000, -94, -281, -297, -512,  -936, -12000 }
};

const int positional_score[2][6][64] = {
    /* ==================== Opening ==================== */
    {
        /* Pawn */
        {
            0,   0,   0,   0,   0,   0,   0,   0,
            98, 134,  61,  95,  68, 126,  34, -11,
            -6,   7,  26,  31,  65,  56,  25, -20,
            -14, 13,   6,  21,  23,  12,  17, -23,
            -27, -2,  -5,  12,  17,   6,  10, -25,
            -26, -4,  -4, -10,   3,   3,  33, -12,
            -35, -1, -20, -23, -15,  24,  38, -22,
            0,   0,   0,   0,   0,   0,   0,   0
        },

        /* Knight */
        {
            -167, -89, -34, -49,  61, -97, -15, -107,
            -73,  -41,  72,  36,  23,  62,   7,  -17,
            -47,   60,  37,  65,  84, 129,  73,   44,
            -9,    17,  19,  53,  37,  69,  18,   22,
            -13,    4,  16,  13,  28,  19,  21,   -8,
            -23,   -9,  12,  10,  19,  17,  25,  -16,
            -29,  -53, -12,  -3,  -1,  18, -14,  -19,
            -105, -21, -58, -33, -17, -28, -19,  -23
        },

        /* Bishop */
        {
            -29,   4, -82, -37, -25, -42,   7,  -8,
            -26,  16, -18, -13,  30,  59,  18, -47,
            -16,  37,  43,  40,  35,  50,  37,  -2,
            -4,    5,  19,  50,  37,  37,   7,  -2,
            -6,   13,  13,  26,  34,  12,  10,   4,
            0,    15,  15,  15,  14,  27,  18,  10,
            4,    15,  16,   0,   7,  21,  33,   1,
            -33,  -3, -14, -21, -13, -12, -39, -21
        },

        /* Rook */
        {
            32,  42,  32,  51,  63,   9,  31,  43,
            27,  32,  58,  62,  80,  67,  26,  44,
            -5,  19,  26,  36,  17,  45,  61,  16,
            -24, -11,   7,  26,  24,  35,  -8, -20,
            -36, -26, -12,  -1,   9,  -7,   6, -23,
            -45, -25, -16, -17,   3,   0,  -5, -33,
            -44, -16, -20,  -9,  -1,  11,  -6, -71,
            -19, -13,   1,  17,  16,   7, -37, -26
        },

        /* Queen */
        {
            -28,   0,  29,  12,  59,  44,  43,  45,
            -24, -39,  -5,   1, -16,  57,  28,  54,
            -13, -17,   7,   8,  29,  56,  47,  57,
            -27, -27, -16, -16,  -1,  17,  -2,   1,
            -9,  -26,  -9, -10,  -2,  -4,   3,  -3,
            -14,   2, -11,  -2,  -5,   2,  14,   5,
            -35,  -8,  11,   2,   8,  15,  -3,   1,
            -1,  -18,  -9,  10, -15, -25, -31, -50
        },

        /* King */
        {
            -65,  23,  16, -15, -56, -34,   2,  13,
            29,   -1, -20,  -7,  -8,  -4, -38, -29,
            -9,   24,   2, -16, -20,   6,  22, -22,
            -17, -20, -12, -27, -30, -25, -14, -36,
            -49,  -1, -27, -39, -46, -44, -33, -51,
            -14, -14, -22, -46, -44, -30, -15, -27,
            1,     7,  -8, -64, -43, -16,   9,   8,
            -15,  36,  12, -54,   8, -28,  24,  14
        }
    },

    /* ==================== ENDGAME ==================== */
    {
        /* Pawn */
        {
            0,   0,   0,   0,   0,   0,   0,   0,
            178, 173, 158, 134, 147, 132, 165, 187,
            94, 100,  85,  67,  56,  53,  82,  84,
            32,  24,  13,   5,  -2,   4,  17,  17,
            13,   9,  -3,  -7,  -7,  -8,   3,  -1,
            4,    7,  -6,   1,   0,  -5,  -1,  -8,
            13,   8,   8,  10,  13,   0,   2,  -7,
            0,    0,   0,   0,   0,   0,   0,   0
        },

        /* Knight */
        {
            -58, -38, -13, -28, -31, -27, -63, -99,
            -25,  -8, -25,  -2,  -9, -25, -24, -52,
            -24, -20,  10,   9,  -1,  -9, -19, -41,
            -17,   3,  22,  22,  22,  11,   8, -18,
            -18,  -6,  16,  25,  16,  17,   4, -18,
            -23,  -3,  -1,  15,  10,  -3, -20, -22,
            -42, -20, -10,  -5,  -2, -20, -23, -44,
            -29, -51, -23, -15, -22, -18, -50, -64
        },

        /* Bishop */
        {
            -14, -21, -11,  -8,  -7,  -9, -17, -24,
            -8,   -4,   7, -12,  -3, -13,  -4, -14,
            2,    -8,   0,  -1,  -2,   6,   0,   4,
            -3,    9,  12,   9,  14,  10,   3,   2,
            -6,    3,  13,  19,   7,  10,  -3,  -9,
            -12,  -3,   8,  10,  13,   3,  -7, -15,
            -14, -18,  -7,  -1,   4,  -9, -15, -27,
            -23,  -9, -23,  -5,  -9, -16,  -5, -17
        },

        /* Rook */
        {
            13, 10, 18, 15, 12, 12,  8,  5,
            11, 13, 13, 11, -3,  3,  8,  3,
            7,   7,  7,  5,  4, -3, -5, -3,
            4,   3, 13,  1,  2,  1, -1,  2,
            3,   5,  8,  4, -5, -6, -8, -11,
            -4,  0, -5, -1, -7, -12, -8, -16,
            -6, -6,  0,  2, -9,  -9, -11, -3,
            -9,  2,  3, -1, -5, -13,  4, -20
        },

        /* Queen */
        {
            -9,  22,  22,  27,  27,  19,  10,  20,
            -17, 20,  32,  41,  58,  25,  30,   0,
            -20,  6,   9,  49,  47,  35,  19,   9,
            3,   22,  24,  45,  57,  40,  57,  36,
            -18, 28,  19,  47,  31,  34,  39,  23,
            -16, -27, 15,   6,   9,  17,  10,   5,
            -22, -23, -30, -16, -16, -23, -36, -32,
            -33, -28, -22, -43,  -5, -32, -20, -41
        },

        /* King */
        {
            -74, -35, -18, -18, -11,  15,   4, -17,
            -12,  17,  14,  17,  17,  38,  23,  11,
            10,   17,  23,  15,  20,  45,  44,  13,
            -8,   22,  24,  27,  26,  33,  26,   3,
            -18,  -4,  21,  24,  27,  23,   9, -11,
            -19,  -3,  11,  21,  23,  16,   7,  -9,
            -27, -11,   4,  13,  14,   4,  -5, -17,
            -53, -34, -21, -11, -28, -14, -24, -43
        }
    }
};

const int mirror_squares[64] = {
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


//---- NNUE piece mapping ----//
/*
                             0  1  2  3  4  5  6  7  8  9 10 11
piece coding in this engine: P, N, B, R, Q, K, p, n, b, r, q, k

                             1  2  3  4  5  6  7  8  9 10 11 12 
piece coding in Stockfish:   K, Q, R, B, N, P, k, q, r, b, n, p
*/
//                           P  N  B  R  Q  K   p   n   b  r  q  k
const int nnue_pieces[12] = {6, 5, 4, 3, 2, 1, 12, 11, 10, 9, 8, 7};

// map squares of this engine to Stockfish squares
// a1 = 0, h1 = 7, a8 = 56, h8 = 63
// mirror_squares will be used to convert squares


void nnue_input(Board* board, int* pieces, int* squares) {
    int piece, square;
    int index = 2;
    uint64_t bitboard;

    // iterate over all piece types
    for(int bb_piece = P; bb_piece <= k; bb_piece++) {
        bitboard = board->pieces[bb_piece];

        // iterate over all pieces of the current type
        while(bitboard) {
            piece = bb_piece;
            square = get_least_significant_bit_index(bitboard);

            if(piece == K) {
                pieces[0] = nnue_pieces[piece];
                squares[0] = mirror_squares[square];
            } else if(piece == k) {
                pieces[1] = nnue_pieces[piece];
                squares[1] = mirror_squares[square];
            } else {
                pieces[index] = nnue_pieces[piece];
                squares[index] = mirror_squares[square];
                index++;
            }

            // remove the least significant bit
            pop_bit(bitboard, square);
        }

    }
    // Terminate the arrays
    pieces[index] = 0;
    squares[index] = 0;
}

//---------------------------//

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

    for(int i = 0; i < 33; i++) {
        masks->nnue_squares[i] = 0;
        masks->nnue_pieces[i] = 0;
    }
}


int get_game_phase_score(Board* board) {
    int white_pieces_score = 0;
    int black_pieces_score = 0;
    int piece;
    
    for(piece = N; piece <= Q; piece++) {
        white_pieces_score += count_bits(board->pieces[piece]) * material_score[opening][piece];
    }

    for(piece = n; piece <= q; piece++) {
        black_pieces_score += count_bits(board->pieces[piece])* (-material_score[opening][piece]);
    }

    return white_pieces_score + black_pieces_score;
}

int get_interpolated_material_score(int bb_piece, int game_phase_score) {
    return (material_score[opening][bb_piece] * game_phase_score +
            material_score[endgame][bb_piece] * (OPENING_PHASE_SCORE - game_phase_score)) 
            / OPENING_PHASE_SCORE;
}

int get_interpolated_positional_score(int piece_type, int square, int game_phase_score) {
    return (positional_score[opening][piece_type][square] * game_phase_score +
            positional_score[endgame][piece_type][square] * (OPENING_PHASE_SCORE - game_phase_score)) 
            / OPENING_PHASE_SCORE;
}

int get_material_score(int phase, int bb_piece, int game_phase_score) {
    if(phase == opening || phase == endgame) {
        return material_score[phase][bb_piece];
    } else {
        return get_interpolated_material_score(bb_piece, game_phase_score);
    }
    return 0;
} 

int get_positional_score(int phase, int piece_type, int square, int game_phase_score) {
    if(phase == middlegame) {
        return get_interpolated_positional_score(piece_type, square, game_phase_score);
    } else {
        return positional_score[phase][piece_type][square];
    }
    return 0;
}
int get_bishop_mobility(int phase) {
    if(phase == opening) {
        return BISHOP_MOBILITY_OPENING;
    } else if(phase == endgame) {
        return BISHOP_MOBILITY_ENDGAME;
    }
    return (BISHOP_MOBILITY_OPENING + BISHOP_MOBILITY_ENDGAME) / 2;
}

int get_queen_mobility(int phase) {
    if(phase == opening) {
        return QUEEN_MOBILITY_OPENING;
    } else if(phase == endgame) {
        return QUEEN_MOBILITY_ENDGAME;
    }
    return (QUEEN_MOBILITY_OPENING + QUEEN_MOBILITY_ENDGAME) / 2;
}

int clamp(int score, int min, int max) {
    if(score < min) return min;
    if(score > max) return max;
    return score;
}

void reset_nnue_input(evaluation_masks* eval_masks) {
    for(int i = 0; i < 33; i++) {
        eval_masks->nnue_squares[i] = 0;
        eval_masks->nnue_pieces[i] = 0;
    }
}

int evaluate(Board* board, leaper_moves_masks* leaper_masks, slider_moves_masks* slider_masks, evaluation_masks* eval_masks) {
    int game_phase_s = get_game_phase_score(board);
    int game_phase = -1; // not opening, not middle, not ending

    if(game_phase_s > OPENING_PHASE_SCORE) game_phase = opening;
    else if (game_phase_s < ENDING_PHASE_SCORE) game_phase = endgame;
    else game_phase = middlegame;

    int score = 0;

    uint64_t bitboard;
    int square;
    int pawns;

    if (game_phase != opening) {
        for(int bb_piece = P; bb_piece <= k; bb_piece++) {
            bitboard = board->pieces[bb_piece];
            while(bitboard) {
                square = get_least_significant_bit_index(bitboard);

                score += get_material_score(game_phase, bb_piece, game_phase_s);

                // evaluate with positional score tables
                // early queen movement leads to loss
                switch(bb_piece) {
                    // white
                    case P:
                        score += get_positional_score(game_phase, PAWN, square, game_phase_s);

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
                        score += get_positional_score(game_phase, KNIGHT, square, game_phase_s);
                        break;
                    case B:
                        score += get_positional_score(game_phase, BISHOP, square, game_phase_s);
                        score += (count_bits(get_bishop_attacks(slider_masks, square, board->occupancies[both])) - BISHOP_UNIT) * get_bishop_mobility(game_phase);
                        break;
                    case R:
                        score += get_positional_score(game_phase, ROOK, square, game_phase_s); // positioan scores

                        // semi open file bonus
                        if((board->pieces[P] & eval_masks->file_masks[square]) == 0ULL) {
                            score += 2 * SEMI_OPEN_FILE_SCORE;
                        }

                        if(((board->pieces[P] | board->pieces[p]) & eval_masks->file_masks[square]) == 0ULL) {
                            score += SEMI_OPEN_FILE_SCORE;
                        }

                        break;
                    case K:
                        score += get_positional_score(game_phase, KING, square, game_phase_s);

                        // open file penalty for king
                        if((board->pieces[P] & eval_masks->file_masks[square]) == 0ULL) {
                            score -= 2 * SEMI_OPEN_FILE_SCORE;
                        }
                        if(((board->pieces[P] | board->pieces[p]) & eval_masks->file_masks[square]) == 0ULL) {
                            score -= SEMI_OPEN_FILE_SCORE;
                        }

                        // king safety bonus
                        score += count_bits((get_king_attacks(leaper_masks, square) & board->occupancies[white])) * KING_SHIELD_SCORE;

                        break;
                    case Q:
                        score += (count_bits(get_queen_attacks(slider_masks, square, board->occupancies[both])) - QUEEN_UNIT) * get_queen_mobility(game_phase);
                        break;



                    // black
                    case p:
                        score -= get_positional_score(game_phase, PAWN, mirror_squares[square], game_phase_s);

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
                            score -= square_bonus[square_bonus_index[mirror_squares[square]]];
                        }

                        break;
                    case n:
                        score -= get_positional_score(game_phase, KNIGHT, mirror_squares[square], game_phase_s);
                        break;
                    case b:
                        score -= get_positional_score(game_phase, BISHOP, mirror_squares[square], game_phase_s);
                        score -= (count_bits(get_bishop_attacks(slider_masks, square, board->occupancies[both])) - BISHOP_UNIT) * get_bishop_mobility(game_phase);
                        break;
                    case r:
                        score -= get_positional_score(game_phase, ROOK, mirror_squares[square], game_phase_s);

                        // semi open file bonus
                        if((board->pieces[p] & eval_masks->file_masks[square]) == 0ULL) {
                            score -= 2 * SEMI_OPEN_FILE_SCORE;
                        }

                        if(((board->pieces[P] | board->pieces[p]) & eval_masks->file_masks[square]) == 0ULL) {
                            score -= SEMI_OPEN_FILE_SCORE;
                        }

                        break;
                    case k:
                        score -= get_positional_score(game_phase, KING, mirror_squares[square], game_phase_s);

                        // open file penalty for king
                        if((board->pieces[p] & eval_masks->file_masks[square]) == 0ULL) {
                            score += 2 * SEMI_OPEN_FILE_SCORE;
                        }

                        if(((board->pieces[P] | board->pieces[p]) & eval_masks->file_masks[square]) == 0ULL) {
                            score += SEMI_OPEN_FILE_SCORE;
                        }

                        score -= count_bits((get_king_attacks(leaper_masks, square) & board->occupancies[black])) * KING_SHIELD_SCORE;

                        break;
                    case q: 
                        score -= (count_bits(get_queen_attacks(slider_masks, square, board->occupancies[both])) - QUEEN_UNIT) * get_queen_mobility(game_phase);
                        break;
                }

                pop_bit(bitboard, square);
            }
        }
    } 

    if(game_phase == middlegame) {
        score = clamp(score, -MIDDLE_GAME_CLAMP_LIMIT, MIDDLE_GAME_CLAMP_LIMIT);
    } else if (game_phase == endgame) {
        score = clamp(score, -LATE_GAME_CLAMP_LIMIT, LATE_GAME_CLAMP_LIMIT);
    }

    score = (board->side_to_move == white) ? score : -score;

    reset_nnue_input(eval_masks);
    nnue_input(board, eval_masks->nnue_pieces, eval_masks->nnue_squares);
    int nnue_score = evaluate_nnue(board->side_to_move, eval_masks->nnue_pieces, eval_masks->nnue_squares) * 5 / 4; // scale NNUE score

    return nnue_score + score;
}