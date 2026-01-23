#ifndef NNUE_EVAL_H
#define NNUE_EVAL_H

/*
    Wraper is needed because there are some conflicting definitions and variables
    between the main engine code and the NNUE library code. 
*/

/* NNUE wrapper function headers */
void init_nnue(char *filename);
int evaluate_nnue(int player, int *pieces, int *squares);
int evaluate_fen_nnue(char *fen);

/*
    To pass the board position to NNUE, the board needs to be converted

    The mapping of pices of this engine is different from Stockfish piece mapping.   
                                 0  1  2  3  4  5  6  7  8  9 10 11
    piece coding in this engine: P, N, B, R, Q, K, p, n, b, r, q, k

                                 1  2  3  4  5  6  7  8  9 10 11 12 
    piece coding in Stockfish:   K, Q, R, B, N, P, k, q, r, b, n, p

    So first the pieces need to be mapped accordingly before passing to NNUE.

    Also the squares need to be mirrored since Stockfish considers a1 as 0 and h8 as 63
    So that why i use mirror_squares array to convert the squares.

    The nnue_input function prepares the pieces and squares arrays to be passed to NNUE

    First 2 elements are reserved for kings
    pieces[0] = white king piece code
    squares[0] = white king square
    Other pieces follow in any order

    square arrays determine the square of each piece in the pieces array

    Both arrays are terminated with a 0 element

    EXAMPLE:
    picies[0] = K
    squares[0] = 60 (e1)
    This tells NNUE that white king is on e1
*/

char* find_nnue_path();

#endif // NNUE_EVAL_H