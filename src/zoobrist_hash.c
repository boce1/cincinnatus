#include "zoobrist_hash.h"

zoobrist_hash_keys* create_zoobrist_hash_keys() {
    zoobrist_hash_keys* hash_data = (zoobrist_hash_keys*)malloc(sizeof(zoobrist_hash_keys));
    return hash_data;
}

void init_zoobrist_random_keys(zoobrist_hash_keys* hash_data) {
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
    tag_hash* transposition_table = (tag_hash*)malloc(HAST_ENTRIES * sizeof(tag_hash));
    // Initialize the hash table entries to zero
    clear_transposition_table(transposition_table);
    return transposition_table;
}
void clear_transposition_table(tag_hash* transposition_table) {
    if (transposition_table) {
        for(int i = 0; i < HAST_ENTRIES; i++) {
            transposition_table[i].key = 0;
            transposition_table[i].depth = 0;
            transposition_table[i].flag = 0;
            transposition_table[i].score = 0;
        }
    }
}

int read_hash_entry(tag_hash* transposition_table, zoobrist_hash_keys* hash_data, search_heuristics* search_data, int alpha, int beta, int depth) {
    /*
    Check if there is a valid hash entry for the current position
    If found and the stored depth is sufficient, use the entry to return a score or bound
    Return NO_HASH_ENTRY if no valid entry is found

    If the entrys flag is HASH_FLAG_EXACT, return the exact score
    If the entrys flag is outside the alpha-beta window, return the bound, IT MEANS THIS NODE CAN BE PRUNED
    ALPHA-BETA WINDOWS IS alpha < score < beta
    */
    tag_hash* entry = &transposition_table[hash_data->board_hash_key % HAST_ENTRIES];
    if(entry->key == hash_data->board_hash_key) {
        if(entry->depth >= depth) {
            int score = entry->score;
            // adjust mate scores based on ply
            if(score < -LOWER_BOUND_MATE_SCORE) {
                score += search_data->ply; 
            } else if(score > LOWER_BOUND_MATE_SCORE) {
                score -= search_data->ply; 
            }

            if(entry->flag == HASH_FLAG_EXACT) {
                return score;
            }
            if(entry->flag == HASH_FLAG_ALPHA && score <= alpha) {
                return alpha;
            }
            if(entry->flag == HASH_FLAG_BETA && score >= beta) {
                return beta;
            }
        }
    }
    return NO_HASH_ENTRY; // no valid entry found
}

void write_hash_entry(tag_hash* transposition_table, zoobrist_hash_keys* hash_data, search_heuristics* search_data, int score, int depth, int hash_flag) {
    tag_hash* entry = &transposition_table[hash_data->board_hash_key % HAST_ENTRIES];
    
    if(score < -LOWER_BOUND_MATE_SCORE) {
        score -= search_data->ply; 
    } else if(score > LOWER_BOUND_MATE_SCORE) {
        score += search_data->ply; 
    }

    entry->key = hash_data->board_hash_key;
    entry->depth = depth;
    entry->flag = hash_flag;
    entry->score = score;
}

repetition_data* create_repetition_table() {
    repetition_data* repetition_table = (repetition_data*)malloc(sizeof(repetition_data));
    return repetition_table;
}

void init_repetition_table(repetition_data* repetition_table) {
    memset(repetition_table->keys, 0, sizeof(repetition_table->keys));
    repetition_table->index = 0;
}

int check_repetition(repetition_data* repetition_table, zoobrist_hash_keys* hash_data) {
    for(int i = 0; i < repetition_table->index; i++) {
        if(repetition_table->keys[i] == hash_data->board_hash_key) {
            return 1; // repetition found
        }
    }
    return 0; // no repetition
}