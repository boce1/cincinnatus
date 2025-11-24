#include "move_generator.h"

const int castling_rights_vals[64] = {
    7, 15, 15, 15, 3, 15, 15, 11,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    13, 15, 15, 15, 12, 15, 15, 14
};

void generate_pawn_moves(Board* board, leaper_moves_masks* leaper_masks, int piece, Moves* move_list) {
    int source_square, target_square;
    uint64_t bitboard, attacks;

    bitboard = board->pieces[piece];
    if(white == board->side_to_move) { // generate white pawn and white king castiling moves (they are not realing on precalculated attack tables)
        if(piece == P) {
            while(bitboard) {
                source_square = get_least_significant_bit_index(bitboard);
                target_square = source_square - 8; // move pawn one place forward
                if(!(target_square < a8) && !get_bit(board->occupancies[both], target_square) ) {
                    // pawn promotion
                    if(source_square >= a7 && source_square <= h7) {
                        // generate all promotion moves
                        add_move(move_list, encode_move(source_square, target_square, piece, Q, not_capturing, not_double_push, not_enpassant, not_castiling));
                        add_move(move_list, encode_move(source_square, target_square, piece, R, not_capturing, not_double_push, not_enpassant, not_castiling));
                        add_move(move_list, encode_move(source_square, target_square, piece, B, not_capturing, not_double_push, not_enpassant, not_castiling));
                        add_move(move_list, encode_move(source_square, target_square, piece, N, not_capturing, not_double_push, not_enpassant, not_castiling));
                    } else {
                        // one square forward
                        add_move(move_list, encode_move(source_square, target_square, piece, no_piece, not_capturing, not_double_push, not_enpassant, not_castiling));

                        // two squares forward
                        if((source_square >= a2 && source_square <= h2) && !get_bit(board->occupancies[both], target_square - 8)) {
                            target_square -= 8; // move pawn two places forward
                            add_move(move_list, encode_move(source_square, target_square, piece, no_piece, not_capturing, double_push, not_enpassant, not_castiling));

                        }
                    }
                }
                attacks = get_pawn_attacks(leaper_masks, source_square, white) & board->occupancies[black];
                while(attacks) {
                    target_square = get_least_significant_bit_index(attacks);

                    // pawn promotion
                    if(source_square >= a7 && source_square <= h7) {
                        // generate all promotion moves
                        add_move(move_list, encode_move(source_square, target_square, piece, Q, capturing, not_double_push, not_enpassant, not_castiling));
                        add_move(move_list, encode_move(source_square, target_square, piece, R, capturing, not_double_push, not_enpassant, not_castiling));
                        add_move(move_list, encode_move(source_square, target_square, piece, B, capturing, not_double_push, not_enpassant, not_castiling));
                        add_move(move_list, encode_move(source_square, target_square, piece, N, capturing, not_double_push, not_enpassant, not_castiling));
                    } else {
                        add_move(move_list, encode_move(source_square, target_square, piece, no_piece, capturing, not_double_push, not_enpassant, not_castiling));

                    }

                    pop_bit(attacks, target_square);
                }

                // en passant
                if(board->en_passant_square != no_square) {
                    uint64_t enpassant_mask = get_pawn_attacks(leaper_masks, source_square, white) & (1ULL << board->en_passant_square);
                    if(enpassant_mask) {
                        int target_enpassant = get_least_significant_bit_index(enpassant_mask);
                        add_move(move_list, encode_move(source_square, target_enpassant, piece, no_piece, capturing, not_double_push, enpassant, not_castiling));

                    }   
                }

                pop_bit(bitboard, source_square);
            }   
        }
    } else { // generate black pawn
        if(piece == p) {
            while(bitboard) {
                source_square = get_least_significant_bit_index(bitboard);
                target_square = source_square + 8; // move pawn one place down
                if(!(target_square > h1) && !get_bit(board->occupancies[both], target_square) ) {
                    // pawn promotion
                    if(source_square >= a2 && source_square <= h2) {
                        // generate all promotion moves
                        add_move(move_list, encode_move(source_square, target_square, piece, q, not_capturing, not_double_push, not_enpassant, not_castiling));
                        add_move(move_list, encode_move(source_square, target_square, piece, r, not_capturing, not_double_push, not_enpassant, not_castiling));
                        add_move(move_list, encode_move(source_square, target_square, piece, b, not_capturing, not_double_push, not_enpassant, not_castiling));
                        add_move(move_list, encode_move(source_square, target_square, piece, n, not_capturing, not_double_push, not_enpassant, not_castiling));
                    } else {
                        // one square forward
                        add_move(move_list, encode_move(source_square, target_square, piece, no_piece, not_capturing, not_double_push, not_enpassant, not_castiling));

                        // two squares forward
                        if((source_square >= a7 && source_square <= h7) && !get_bit(board->occupancies[both], target_square + 8)) {
                            target_square += 8; // move pawn two places dows
                            add_move(move_list, encode_move(source_square, target_square, piece, no_piece, not_capturing, double_push, not_enpassant, not_castiling));

                        }
                    }
                }

                attacks = get_pawn_attacks(leaper_masks, source_square, black) & board->occupancies[white];
                while(attacks) {
                    target_square = get_least_significant_bit_index(attacks);

                    // pawn promotion
                    if(source_square >= a2 && source_square <= h2) {
                        // generate all promotion moves
                        add_move(move_list, encode_move(source_square, target_square, piece, q, capturing, not_double_push, not_enpassant, not_castiling));
                        add_move(move_list, encode_move(source_square, target_square, piece, r, capturing, not_double_push, not_enpassant, not_castiling));
                        add_move(move_list, encode_move(source_square, target_square, piece, b, capturing, not_double_push, not_enpassant, not_castiling));
                        add_move(move_list, encode_move(source_square, target_square, piece, n, capturing, not_double_push, not_enpassant, not_castiling));
                    } else {
                        add_move(move_list, encode_move(source_square, target_square, piece, no_piece, capturing, not_double_push, not_enpassant, not_castiling));
                    }

                    pop_bit(attacks, target_square);
                }

                // en passant
                if(board->en_passant_square != no_square) {
                    uint64_t enpassant_mask = get_pawn_attacks(leaper_masks, source_square, black) & (1ULL << board->en_passant_square);
                    if(enpassant_mask) {
                        int target_enpassant = get_least_significant_bit_index(enpassant_mask);
                        add_move(move_list, encode_move(source_square, target_enpassant, piece, no_piece, capturing, not_double_push, enpassant, not_castiling));
                    }   
                }

                pop_bit(bitboard, source_square);
            }   
        }
    }
}

void generate_king_castle(Board* board, leaper_moves_masks* leaper_masks, slider_moves_masks* slider_masks, int piece, Moves* move_list) {
    if(white == board->side_to_move) {
        if(piece == K) {
            if(board->castling_rights & wk) {
                if(!get_bit(board->occupancies[both], f1) && !get_bit(board->occupancies[both], g1)) { // check if f1 and g1 are empty
                    if(!is_square_attacked(e1, board, leaper_masks, slider_masks) && !is_square_attacked(f1, board, leaper_masks, slider_masks)) { // g1 is checked in make move function 
                        add_move(move_list, encode_move(e1, g1, piece, no_piece, not_capturing, not_double_push, not_enpassant, castiling));
                    }
                }
            }

            if(board->castling_rights & wq) {   
                if(!get_bit(board->occupancies[both], d1) && !get_bit(board->occupancies[both], c1) && !get_bit(board->occupancies[both], b1)) { // check if d1, c1 and b1 are empty
                    if(!is_square_attacked(e1, board, leaper_masks, slider_masks) && !is_square_attacked(d1, board, leaper_masks, slider_masks)) { // c1 is checked in king leagal moves
                        add_move(move_list, encode_move(e1, c1, piece, no_piece, not_capturing, not_double_push, not_enpassant, castiling));
                    }
                }
            }
        }
    } else {
        if(piece == k) {
            if(board->castling_rights & bk) {
                if(!get_bit(board->occupancies[both], f8) && !get_bit(board->occupancies[both], g8)) { // check if f8 and g8 are empty
                    if(!is_square_attacked(e8, board, leaper_masks, slider_masks) && !is_square_attacked(f8, board, leaper_masks, slider_masks)) { // g8 is checked in make move function
                        add_move(move_list, encode_move(e8, g8, piece, no_piece, not_capturing, not_double_push, not_enpassant, castiling));
                    }
                }
            } 

            if (board->castling_rights & bq) {   
                if(!get_bit(board->occupancies[both], d8) && !get_bit(board->occupancies[both], c8) && !get_bit(board->occupancies[both], b8)) { // check if d8, c8 and b8 are empty
                    if(!is_square_attacked(e8, board, leaper_masks, slider_masks) && !is_square_attacked(d8, board, leaper_masks, slider_masks)) { // c8 is checked in king leagal moves
                        add_move(move_list, encode_move(e8, c8, piece, no_piece, not_capturing, not_double_push, not_enpassant, castiling));
                    }
                }
            }
        }
    }
}

void generate_knight_moves(Board* board, leaper_moves_masks* leaper_masks, int piece, Moves* move_list) {
    int source_square, target_square;
    uint64_t bitboard, attacks;

    int opp_side = (board->side_to_move == white) ? black : white;

    bitboard = board->pieces[piece];
    if((board->side_to_move == white && piece == N) || (board->side_to_move == black && piece == n)) {
        while(bitboard) {
            source_square = get_least_significant_bit_index(bitboard);
            attacks = get_knight_attacks(leaper_masks, source_square) & ~board->occupancies[board->side_to_move]; // get knight attacks by remove capturing own pieces

            while(attacks) {
                target_square = get_least_significant_bit_index(attacks);
                
                // quite moves
                if(!get_bit(board->occupancies[opp_side], target_square)) {
                    add_move(move_list, encode_move(source_square, target_square, piece, no_piece, not_capturing, not_double_push, not_enpassant, not_castiling));
                } else { // capture moves
                    add_move(move_list, encode_move(source_square, target_square, piece, no_piece, capturing, not_double_push, not_enpassant, not_castiling));
                }
                
                pop_bit(attacks, target_square);
            }

            pop_bit(bitboard, source_square);
        }
    }
    
}

void generate_bishop_moves(Board* board, slider_moves_masks* slider_masks, int piece, Moves* move_list) {
    int source_square, target_square;
    uint64_t bitboard, attacks;

    int opp_side = (board->side_to_move == white) ? black : white;

    bitboard = board->pieces[piece];
    if((board->side_to_move == white && piece == B) || (board->side_to_move == black && piece == b)) {
        while(bitboard) {
            source_square = get_least_significant_bit_index(bitboard);
            attacks = get_bishop_attacks(slider_masks, source_square, board->occupancies[both]) & ~board->occupancies[board->side_to_move]; // get bishop attacks by remove capturing own pieces

            while(attacks) {
                target_square = get_least_significant_bit_index(attacks);
                
                // quite moves
                if(!get_bit(board->occupancies[opp_side], target_square)) {
                    add_move(move_list, encode_move(source_square, target_square, piece, no_piece, not_capturing, not_double_push, not_enpassant, not_castiling));
                } else { // capture moves
                    add_move(move_list, encode_move(source_square, target_square, piece, no_piece, capturing, not_double_push, not_enpassant, not_castiling));
                }
                
                pop_bit(attacks, target_square);
            }

            pop_bit(bitboard, source_square);
        }
    }
}

void generate_rook_moves(Board* board, slider_moves_masks* slider_masks, int piece, Moves* move_list) {
    int source_square, target_square;
    uint64_t bitboard, attacks;

    int opp_side = (board->side_to_move == white) ? black : white;

    bitboard = board->pieces[piece];
    if((board->side_to_move == white && piece == R) || (board->side_to_move == black && piece == r)) {
        while(bitboard) {
            source_square = get_least_significant_bit_index(bitboard);
            attacks = get_rook_attacks(slider_masks, source_square, board->occupancies[both]) & ~board->occupancies[board->side_to_move]; // get rook attacks by remove capturing own pieces

            while(attacks) {
                target_square = get_least_significant_bit_index(attacks);
                
                // quite moves
                if(!get_bit(board->occupancies[opp_side], target_square)) {
                    add_move(move_list, encode_move(source_square, target_square, piece, no_piece, not_capturing, not_double_push, not_enpassant, not_castiling));
                } else { // capture moves
                    add_move(move_list, encode_move(source_square, target_square, piece, no_piece, capturing, not_double_push, not_enpassant, not_castiling));
                }
                
                pop_bit(attacks, target_square);
            }

            pop_bit(bitboard, source_square);
        }
    }
}

void generate_queen_moves(Board* board, slider_moves_masks* slider_masks, int piece, Moves* move_list) {
    int source_square, target_square;
    uint64_t bitboard, attacks;

    int opp_side = (board->side_to_move == white) ? black : white;

    bitboard = board->pieces[piece];
    if((board->side_to_move == white && piece == Q) || (board->side_to_move == black && piece == q)) {
        while(bitboard) {
            source_square = get_least_significant_bit_index(bitboard);
            attacks = get_queen_attacks(slider_masks, source_square, board->occupancies[both]) & ~board->occupancies[board->side_to_move]; // get queen attacks by remove capturing own pieces

            while(attacks) {
                target_square = get_least_significant_bit_index(attacks);
                
                // quite moves
                if(!get_bit(board->occupancies[opp_side], target_square)) {
                    add_move(move_list, encode_move(source_square, target_square, piece, no_piece, not_capturing, not_double_push, not_enpassant, not_castiling));
                } else { // capture moves
                    add_move(move_list, encode_move(source_square, target_square, piece, no_piece, capturing, not_double_push, not_enpassant, not_castiling));
                }
                
                pop_bit(attacks, target_square);
            }

            pop_bit(bitboard, source_square);
        }
    }
}

void generate_king_moves(Board* board, leaper_moves_masks* leaper_masks, int piece, Moves* move_list) {
    int source_square, target_square;
    uint64_t bitboard, attacks;

    int opp_side = (board->side_to_move == white) ? black : white;

    bitboard = board->pieces[piece];
    if((board->side_to_move == white && piece == K) || (board->side_to_move == black && piece == k)) {
        while(bitboard) {
            source_square = get_least_significant_bit_index(bitboard);
            attacks = get_king_attacks(leaper_masks, source_square) & ~board->occupancies[board->side_to_move]; // get king attacks by remove capturing own pieces

            while(attacks) {
                target_square = get_least_significant_bit_index(attacks);
                
                // quite moves
                if(!get_bit(board->occupancies[opp_side], target_square)) {
                    add_move(move_list, encode_move(source_square, target_square, piece, no_piece, not_capturing, not_double_push, not_enpassant, not_castiling));
                } else { // capture moves
                    add_move(move_list, encode_move(source_square, target_square, piece, no_piece, capturing, not_double_push, not_enpassant, not_castiling));
                }
                
                pop_bit(attacks, target_square);
            }

            pop_bit(bitboard, source_square);
        }
    }
}


void generate_moves(Board* board, leaper_moves_masks* leaper_masks, slider_moves_masks* slider_masks, Moves* move_list) {
    for(int piece = P; piece <= k; piece++) {
        generate_pawn_moves(board, leaper_masks, piece, move_list); // generate only when pawn
        generate_king_castle(board, leaper_masks, slider_masks, piece, move_list); // generate only when king
        generate_knight_moves(board, leaper_masks, piece, move_list); // generate only when knight
        generate_bishop_moves(board, slider_masks, piece, move_list); // generate only when bishop
        generate_rook_moves(board, slider_masks, piece, move_list); // generate only when rook
        generate_queen_moves(board, slider_masks, piece, move_list); // generate only when queen
        generate_king_moves(board, leaper_masks, piece, move_list); // generate only when king
    }
}

//------------------------
const char promoted_pieces[128] = { // needs to be encoded with lowercase letter by standard
    [Q] = 'q',
    [R] = 'r',
    [B] = 'b',
    [N] = 'n',
    [q] = 'q',
    [r] = 'r',
    [b] = 'b',
    [n] = 'n'
};

void print_move(int move) {
    printf("%s%s%c   %c       %c      %c        %c        %c\n", square_to_cordinates[get_move_source(move)],
        square_to_cordinates[get_move_target(move)],
        get_move_promoted(move) ? promoted_pieces[get_move_promoted(move)] : ' ',
        ascii_pieces[get_move_piece(move)],
        get_move_capture(move) ? '1' : '-',
        get_move_double_push(move) ? '1' : '-',
        get_move_enpassant(move) ? '1' : '-',
        get_move_castiling(move) ? '1' : '-');
}

void init_move_list(Moves* move_list) {
    if(move_list) {
        move_list->count = 0;
        for(int i = 0; i < 256; i++) {
            move_list->moves[i] = 0;
        }
    }

}

void print_move_list(Moves* move_list) {
    printf("move  piece  capture double enpassant castile\n");
    for(int i = 0; i < move_list->count; i++) {
        print_move(move_list->moves[i]);
    }
    printf("Moves number: %d\n\n", move_list->count);
}

void add_move(Moves* move_list, int move) {
    if(move_list->count < 256) {
        move_list->moves[move_list->count++] = move;
    } else {
        printf("Move list is full, cannot add more moves.\n");
    }
}
// -----------------------

int make_move(Board* board, int move, int move_flag, leaper_moves_masks* leaper_masks, slider_moves_masks* slider_masks, zoobrist_hash_keys* hash_keys) {
    if(move_flag == all_moves) {
        copy_board(board);
        copy_board_hash_key(hash_keys);

        int source_square = get_move_source(move);
        int target_square = get_move_target(move);
        int piece = get_move_piece(move);
        int promoted_piece = get_move_promoted(move);
        int capture = get_move_capture(move);
        int double_push = get_move_double_push(move);
        int enpass = get_move_enpassant(move);
        int castiling = get_move_castiling(move);

        pop_bit(board->pieces[piece], source_square);
        set_bit(board->pieces[piece], target_square);

        // UPDATE HASH KEY
        hash_keys->board_hash_key ^= hash_keys->side_key; // hash side for black, returns at the previous state for white (it toggles this parameter)
        hash_keys->board_hash_key ^= hash_keys->piece_keys[piece][source_square]; // remove piece from source square
        hash_keys->board_hash_key ^= hash_keys->piece_keys[piece][target_square]; // add piece to target square
        // if move is capture the capture piece removes the hash it gave
        // for en passant the hash key is updated in double push if statement
        // if ene passant happens remove the en passant hash in en passant if statement
        // for castiling unhash the castling right then hash it again (in the castling right update)
        // for promotion remove pawn from target square and add promoted piece to target square (done in promotion if statement)

        if(capture) { // capture pieces
            int start_piece, end_piece;
            if(board->side_to_move == white) {
                start_piece = p;
                end_piece = k;
            } else if(board->side_to_move == black) { // black side
                start_piece = P;
                end_piece = K;
            }

            for(int bb_piece = start_piece; bb_piece <= end_piece; bb_piece++ ) {
                if(get_bit(board->pieces[bb_piece], target_square)) {
                    pop_bit(board->pieces[bb_piece], target_square);

                    // UPDATE HASH KEY FOR CAPTURE
                    hash_keys->board_hash_key ^= hash_keys->piece_keys[bb_piece][target_square]; // remove captured piece from target square

                    break;
                }
            }
        }

        if(promoted_piece) {
            if(board->side_to_move == white) {
                pop_bit(board->pieces[P], target_square);
                hash_keys->board_hash_key ^= hash_keys->piece_keys[P][target_square]; // remove pawn from target square
            } else if(board->side_to_move == black) {
                pop_bit(board->pieces[p], target_square);
                hash_keys->board_hash_key ^= hash_keys->piece_keys[p][target_square]; // remove pawn from target square
            }
            set_bit(board->pieces[promoted_piece], target_square);
            hash_keys->board_hash_key ^= hash_keys->piece_keys[promoted_piece][target_square]; // add promoted piece to target square
        }

        if(enpass) { // en passant captures
            if(board->side_to_move == white) {
                pop_bit(board->pieces[p], target_square + 8);
                hash_keys->board_hash_key ^= hash_keys->piece_keys[p][target_square + 8]; // remove captured pawn from target square
            }
            else if (board->side_to_move == black) {
                pop_bit(board->pieces[P], target_square - 8);
                hash_keys->board_hash_key ^= hash_keys->piece_keys[P][target_square - 8]; // remove captured pawn from target square

            }
        }

        if(board->en_passant_square != no_square) { // remove en passant hash key if there was one
            hash_keys->board_hash_key ^= hash_keys->en_passant_keys[board->en_passant_square];
        }

        board->en_passant_square = no_square; // reset en passant exceot double pawn push

        if(double_push) {
            if(board->side_to_move == white) {
                board->en_passant_square = target_square + 8;

            } else if(board->side_to_move == black){
                board->en_passant_square = target_square - 8;
            }
            hash_keys->board_hash_key ^= hash_keys->en_passant_keys[board->en_passant_square]; // update hash key for en passant
        }

        if(castiling) {
            switch(target_square) {
                case (g1): // white king side
                    pop_bit(board->pieces[R], h1);
                    set_bit(board->pieces[R], f1);

                    hash_keys->board_hash_key ^= hash_keys->piece_keys[R][h1]; // remove rook from h1
                    hash_keys->board_hash_key ^= hash_keys->piece_keys[R][f1]; // add rook to f1
                    break;
                case (c1): // white queen side
                    pop_bit(board->pieces[R], a1);
                    set_bit(board->pieces[R], d1);

                    hash_keys->board_hash_key ^= hash_keys->piece_keys[R][a1]; // remove rook from a1
                    hash_keys->board_hash_key ^= hash_keys->piece_keys[R][d1]; // add rook to d1
                    break;
                case (g8): // black king side
                    pop_bit(board->pieces[r], h8);
                    set_bit(board->pieces[r], f8);

                    hash_keys->board_hash_key ^= hash_keys->piece_keys[r][h8]; // remove rook from h8
                    hash_keys->board_hash_key ^= hash_keys->piece_keys[r][f8]; // add rook to f8
                    break;
                case (c8): // black queen side
                    pop_bit(board->pieces[r], a8);
                    set_bit(board->pieces[r], d8);

                    hash_keys->board_hash_key ^= hash_keys->piece_keys[r][a8]; // remove rook from a8
                    hash_keys->board_hash_key ^= hash_keys->piece_keys[r][d8]; // add rook to d8
                    break;
            }
        }


        hash_keys->board_hash_key ^= hash_keys->castle_keys[board->castling_rights]; // remove castling right hash
        // update castiling rights
        board->castling_rights &= castling_rights_vals[source_square]; // if pieces move
        board->castling_rights &= castling_rights_vals[target_square]; // if pieces get captured
        hash_keys->board_hash_key ^= hash_keys->castle_keys[board->castling_rights]; // add castling right hash

        memset(board->occupancies, 0ULL, 24);
        int bb_piece;
        for(bb_piece = P; bb_piece <= K; bb_piece++) {
            board->occupancies[white] |= board->pieces[bb_piece]; 
        }
        for(bb_piece = p; bb_piece <= k; bb_piece++) {
            board->occupancies[black] |= board->pieces[bb_piece]; 
        }
        board->occupancies[both] |= board->occupancies[white];
        board->occupancies[both] |= board->occupancies[black];

        
        // king has not been exposed into a check
        int king_pos;
        if(board->side_to_move == white) {
            king_pos = get_least_significant_bit_index(board->pieces[K]);
            if(is_square_attacked(king_pos, board, leaper_masks, slider_masks)) {
                take_back(board);
                take_back_board_hash_key(hash_keys);
                return 0;
            }
        } else if(board->side_to_move == black) {
            king_pos = get_least_significant_bit_index(board->pieces[k]);
            if(is_square_attacked(king_pos, board, leaper_masks, slider_masks)) {
                take_back(board);
                take_back_board_hash_key(hash_keys);
                return 0;
            }
        }

        // change side
        board->side_to_move ^= 1; 

        // DEBUGGING HASH KEY
        // uint64_t current_hash_key = generate_board_hash_key(board, hash_keys);
        // if(hash_keys->board_hash_key != current_hash_key) {
        //     printf("Make move testing hash\n");
        //     print_move(move);
        //     print_board(board);
        //     printf("Board hash key: %" PRIu64 "\n", hash_keys->board_hash_key);
        //     printf("Current hash key: %" PRIu64 "\n", current_hash_key);
        //     getchar();
        // }

        return 1;
        
    } else if(move_flag == only_captures) {
        if(get_move_capture(move)) {
            return make_move(board, move, all_moves, leaper_masks, slider_masks, hash_keys);
        } else {
            return 0;
        }
    }
    return 0;
       
}