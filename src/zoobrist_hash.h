#ifndef ZOOBRIST_HASH_H
#define ZOOBRIST_HASH_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "magic_numbers.h"
#include "board.h"
#include "bitboard.h"

#define HASH_FLAG_EXACT 0 // hash position if it is in window search
#define HASH_FLAG_ALPHA 1 // hash it and mark it as alpha
#define HASH_FLAG_BETA 2 // hash it and mark it as beta

// no hash entry found constant, that is higher than any possible score
#define NO_HASH_ENTRY 100000 

#define HASH_SIZE 0x400000 // 4MB hash size
// Hex: 1 0 0 0 0 0
// Binary: 0001 0000 0000 0000 0000 0000 = 2^20
// 2^10 = 1024 bytes = 1 KB
// 2^10 * 1KB = 1024 * 1KB = 1 MB

typedef struct {
    uint64_t piece_keys[12][64]; // 12 pieces, 64 squares
    uint64_t en_passant_keys[64]; // 8 possible en passant files (a-h)
    uint64_t castle_keys[16]; // 16 possible castling rights combinations
    uint64_t side_key; // side to move

    uint64_t board_hash_key; // current board hash key, it is not random, represents id of the current position
} zoobrist_hash_keys;

#define MAX_PLY 64
#define MATE_VALUE 49000
#define LOWER_BOUND_MATE_SCORE 48000

typedef struct {
    int ply;
    int follow_pv; // if the current principle variation line is followed
    int score_pv;
    int pv_lenght[MAX_PLY]; // stores the ply where the PV nodes end
    int pv_table[MAX_PLY][MAX_PLY];
    int killer_moves[2][MAX_PLY];
    int history_moves[12][64]; // [piece][sqaure]
    long nodes;
} search_heuristics;

typedef struct {
    uint64_t key; // unique position identifier
    int depth; // depth of the search when the entry was stored
    int flag; // type of the hash entry (exact, alpha, beta)
    int score; // evaluation score (exact score or alpha/beta bound)
} tag_hash;

typedef struct {
    uint64_t keys[500]; // stores hash keys for each ply, 250 moves for each side
    int index; // current ply in the game
} repetition_data;

#define copy_board_hash_key(hash_keys) \
    uint64_t board_hash_key_copy = (hash_keys)->board_hash_key; \

#define take_back_board_hash_key(hash_keys) \
    (hash_keys)->board_hash_key = board_hash_key_copy; \

zoobrist_hash_keys* create_zoobrist_hash_keys();
void init_zoobrist_random_keys(zoobrist_hash_keys* hash_data);

uint64_t generate_board_hash_key(Board* board, zoobrist_hash_keys* hash_data);
void init_board_hash_key(Board* board, zoobrist_hash_keys* hash_data);

void print_hash_key(Board* board, zoobrist_hash_keys* hash_data);

tag_hash* create_transposition_table();
void clear_transposition_table(tag_hash* transposition_table);

int read_hash_entry(tag_hash* transposition_table, zoobrist_hash_keys* hash_data, search_heuristics* search_data, int alpha, int beta, int depth);
void write_hash_entry(tag_hash* transposition_table, zoobrist_hash_keys* hash_data, search_heuristics* search_data, int score, int depth, int hash_flag);

repetition_data* create_repetition_table(); // repetition table has size of MAX_PLY
void init_repetition_table(repetition_data* repetition_data);
int check_repetition(repetition_data* repetition_table, zoobrist_hash_keys* hash_data);

#endif // ZOOBRIST_HASH_H