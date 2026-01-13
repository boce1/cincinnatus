#include "sorting_move_list.h"

// MVV LVA [attacker][victim]
//    P,   N,   B,   R,   Q,   K,    p,   n,   b,   r,   q,   k
int const mvv_lva[12][12] = {
 	{105, 205, 305, 405, 505, 605,  105, 205, 305, 405, 505, 605}, // P
	{104, 204, 304, 404, 504, 604,  104, 204, 304, 404, 504, 604}, // N
	{103, 203, 303, 403, 503, 603,  103, 203, 303, 403, 503, 603}, // B
	{102, 202, 302, 402, 502, 602,  102, 202, 302, 402, 502, 602}, // R
	{101, 201, 301, 401, 501, 601,  101, 201, 301, 401, 501, 601}, // Q
	{100, 200, 300, 400, 500, 600,  100, 200, 300, 400, 500, 600}, // K

	{105, 205, 305, 405, 505, 605,  105, 205, 305, 405, 505, 605}, // p
	{104, 204, 304, 404, 504, 604,  104, 204, 304, 404, 504, 604}, // n
	{103, 203, 303, 403, 503, 603,  103, 203, 303, 403, 503, 603}, // b
	{102, 202, 302, 402, 502, 602,  102, 202, 302, 402, 502, 602}, // r
	{101, 201, 301, 401, 501, 601,  101, 201, 301, 401, 501, 601}, // q
	{100, 200, 300, 400, 500, 600,  100, 200, 300, 400, 500, 600}  // k
};

int score_move(int move, Board* board, search_heuristics* search_data) {
    if(search_data->score_pv) {
        if(search_data->pv_table[0][search_data->ply] == move) {
            search_data->score_pv = 0;
            return PV_MOVE_SCORE;
        }
    }

    if(get_move_capture(move)) {
        // captures scores with mvv_lva
        int target_piece = P; // if it isnt initialized en passant wont work, because doesnt containe the piiece on the square
        int target_square = get_move_target(move);

        int start_piece, end_piece;
        if(board->side_to_move == white) {
            start_piece = p;
            end_piece = k;
        } else if(board->side_to_move == black) {
            start_piece = P;
            end_piece = K;
        }

        for(int bb_piece = start_piece; bb_piece <= end_piece; bb_piece++ ) {
            if(get_bit(board->pieces[bb_piece], target_square)) {
                target_piece = bb_piece;
                break;
            }
        }

        return mvv_lva[get_move_piece(move)][target_piece] + CAPTURE_MOVE_SCORE;
    } else { // quiet move
        // score 1st killer move
        if(search_data->killer_moves[0][search_data->ply] == move) {
            return KILLER_MOVE_SCORE_1;
        }
        // score 2nd killer move
        else if(search_data->killer_moves[1][search_data->ply] == move) {
            return KILLER_MOVE_SCORE_2;
        }
        else  {
            return search_data->history_moves[get_move_piece(move)][get_move_target(move)];
        }
        // score history move

        return 0;
    }

    return 0;
}

void merge(Moves* move_list, int* move_scores, int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int left_moves[n1], right_moves[n2];
    int left_scores[n1], right_scores[n2];
    
    for(i = 0; i < n1; i++) {
        left_moves[i] = move_list->moves[left + i];
        left_scores[i] = move_scores[left + i];
    }
    for(j = 0; j < n2; j++) {
        right_moves[j] = move_list->moves[mid + 1 + j];
        right_scores[j] = move_scores[mid + 1 + j];
    }
    i = 0;
    j = 0;
    k = left;
    while(i < n1 && j < n2) {
        if(left_scores[i] >= right_scores[j]) {
            move_list->moves[k] = left_moves[i];
            move_scores[k] = left_scores[i];
            i++;
        } else {
            move_list->moves[k] = right_moves[j];
            move_scores[k] = right_scores[j];
            j++;
        }
        k++;
    }

    while(i < n1) {
        move_list->moves[k] = left_moves[i];
        move_scores[k] = left_scores[i];
        i++;
        k++;
    }
    while(j < n2) {
        move_list->moves[k] = right_moves[j];
        move_scores[k] = right_scores[j];
        j++;
        k++;
    }
}

void merge_sort(Moves* move_list, int* move_scores, int left, int right) {
    if(left < right) {
        int mid = left + (right - left) / 2;
        merge_sort(move_list, move_scores, left, mid);
        merge_sort(move_list, move_scores, mid + 1, right);
        merge(move_list, move_scores, left, mid, right);
    }
}

void sort_moves(Moves* move_list, Board* board, search_heuristics* search_data) {
    int move_scores[move_list->count];
    for(int i = 0; i < move_list->count; i++) {
        move_scores[i] = score_move(move_list->moves[i], board, search_data);
    }

    merge_sort(move_list, move_scores, 0, move_list->count - 1);
}

void enable_pv_scoring(Moves* move_list, search_heuristics* search_data) {
    search_data->follow_pv = 0;
    
    for(int count = 0; count < move_list->count; count++) {
        if(search_data->pv_table[0][search_data->ply] == move_list->moves[count]) {
            search_data->score_pv = 1;
            search_data->follow_pv = 1;
            break;
        }
    }
}

// -----------
void print_move_scores(Moves* move_list, Board* board, search_heuristics* search_data) {
    // debugging purpose
    // to see if sort move function works correcly
    for(int i = 0; i < move_list->count; i++) {
        print_move(move_list->moves[i]);
        printf("move score: %d\n\n", score_move(move_list->moves[i], board, search_data));
    }
}