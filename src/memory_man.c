#include "memory_man.h"

void create_data_structures(leaper_moves_masks** leaper_masks, slider_moves_masks** slider_masks, Board** board, search_heuristics** search_data, time_controls** time_info, zoobrist_hash_keys** hash_data, tag_hash** transposition_table) {
    *leaper_masks = create_leaper_moves_masks(); // pawn, knight, king masks
    if(!leaper_masks) {
        fprintf(stderr, "Failed to allocate memory for leaper moves masks\n");
        exit(EXIT_FAILURE);
    }
    *slider_masks = create_slider_moves_masks(); // rook, bishop, queen masks4110-+
    if(!slider_masks) {
        free(leaper_masks);
        fprintf(stderr, "Failed to allocate memory for slider moves masks\n");
        exit(EXIT_FAILURE);
    }
    *board = create_board();
    if(!board) {
        free(leaper_masks);
        free(slider_masks);
        fprintf(stderr, "Failed to allocate memory for board\n");
        exit(EXIT_FAILURE);
    }
    *search_data = create_search_heuristics();
    if(!search_data) {
        free(leaper_masks);
        free(slider_masks);
        free(board);
        fprintf(stderr, "Failed to allocate memory for search heuristics\n");
        exit(EXIT_FAILURE);
    }
    *time_info = create_time_controls();
    if(!time_info) {
        free(leaper_masks);
        free(slider_masks);
        free(board);
        free(search_data);
        fprintf(stderr, "Failed to allocate memory for time controls\n");
        exit(EXIT_FAILURE);
    }
    *hash_data = create_zoobrist_hash_keys();
    if(!hash_data) {
        free(leaper_masks);
        free(slider_masks);
        free(board);
        free(search_data);
        free(time_info);
        fprintf(stderr, "Failed to allocate memory for Zobrist hash keys\n");
        exit(EXIT_FAILURE);
    }
    *transposition_table = create_transposition_table();
    if(!transposition_table) {
        free(leaper_masks);
        free(slider_masks);
        free(board);
        free(search_data);
        free(time_info);
        free(hash_data);
        fprintf(stderr, "Failed to allocate memory for transposition table\n");
        exit(EXIT_FAILURE);
    }
}
void free_data_structures(leaper_moves_masks* leaper_masks, slider_moves_masks* slider_masks, Board* board, search_heuristics* search_data, time_controls* time_info, zoobrist_hash_keys* hash_data, tag_hash* transposition_table) {
    if(board != NULL) {
        free(board); 
    }
    if(leaper_masks != NULL) {
        free(leaper_masks);  
    }
    if(slider_masks != NULL) {
        free(slider_masks);
    }
    if(search_data != NULL) {
        free(search_data);
    }
    if(time_info != NULL) {
        free(time_info);
    }
    if(hash_data != NULL) {
        free(hash_data);
    }
    if(transposition_table != NULL) {
        clear_transposition_table(transposition_table);
        free(transposition_table);
    }
}
void init_data_structures(leaper_moves_masks* leaper_masks, slider_moves_masks* slider_masks, Board* board, search_heuristics* search_data, time_controls* time_info, zoobrist_hash_keys* hash_data, tag_hash* transposition_table) {
    init_leaper_moves_masks(leaper_masks);
    init_slider_moves_masks(slider_masks);
    init_board(board);
    init_search_heuristics(search_data);
    init_time_controls(time_info);
    init_zoobrist_random_keys(hash_data);
    clear_transposition_table(transposition_table);
}
