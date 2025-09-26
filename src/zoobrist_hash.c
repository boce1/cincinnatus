#include "zoobrist_hash.h"

zoobrist_hash_keys* create_zoobrist_hash_keys() {
    zoobrist_hash_keys* hash_data = (zoobrist_hash_keys*)malloc(sizeof(zoobrist_hash_keys));
    // if (!hash_data) {
    //     fprintf(stderr, "Memory allocation failed for zoobrist_hash_keys\n");
    //     exit(EXIT_FAILURE);
    // }
    return hash_data;
}

void init_zoobrist_random_keys(zoobrist_hash_keys* hash_data) {
    // The seed is the random_state variable from magic_numbers.c
    // function for getting the random u64 ints are also from magic_numbers.c

    for (int piece = P; piece <= k; piece++) {
        for (int square = 0; square < 64; square++) {
            hash_data->piece_keys[piece][square] = get_random_uint64_t();
        }
    }

    for (int square = 0; square < 64; square++) {
        hash_data->en_passant_keys[square] = get_random_uint64_t();
    }

    for (int i = 0; i < 16; i++) {
        hash_data->castle_keys[i] = get_random_uint64_t();
    }

    hash_data->side_key = get_random_uint64_t();

    hash_data->board_hash_key = 0; // Initialize to zero, will be updated as pieces are added/removed
}

uint64_t generate_board_hash_key(Board* board, zoobrist_hash_keys* hash_data) {
    uint64_t final_key = 0;

    for(int piece = P; piece <= k; piece++) {
        uint64_t bitboard = board->pieces[piece];
        while (bitboard) {
            int square = get_least_significant_bit_index(bitboard); // Get the index of the least significant bit
            final_key ^= hash_data->piece_keys[piece][square]; // XOR the piece-square key
            pop_bit(bitboard, square); 
        }
    }

    if (board->en_passant_square != no_square) {
        final_key ^= hash_data->en_passant_keys[board->en_passant_square];
    }

    final_key ^= hash_data->castle_keys[board->castling_rights];
    if(board->side_to_move == black) { // hash the side if black is to move
        final_key ^= hash_data->side_key;
    }
    return final_key;
}

void init_board_hash_key(Board* board, zoobrist_hash_keys* hash_data) {
    hash_data->board_hash_key = generate_board_hash_key(board, hash_data);
}

void print_hash_key(Board* board, zoobrist_hash_keys* hash_data) {
    uint64_t hash_key = generate_board_hash_key(board, hash_data);
    printf("Hash key: %" PRIu64 "\n", hash_key);
}

tag_hash* create_transposition_table() {
    tag_hash* transposition_table = (tag_hash*)malloc(HASH_SIZE * sizeof(tag_hash));
    // if (!transposition_table) {
    //     fprintf(stderr, "Memory allocation failed for hash table\n");
    //     exit(EXIT_FAILURE);
    // }
    // Initialize the hash table entries to zero
    clear_transposition_table(transposition_table);
    return transposition_table;
}
void clear_transposition_table(tag_hash* transposition_table) {
    if (transposition_table) {
        for(int i = 0; i < HASH_SIZE; i++) {
            transposition_table[i].key = 0;
            transposition_table[i].depth = 0;
            transposition_table[i].flag = 0;
            transposition_table[i].score = 0;
        }
    }
}