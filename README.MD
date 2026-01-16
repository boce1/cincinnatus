# Bitboard Chess Engine

## Features

- [**Bitboard Representation**](#bitboards) – Fast move generation and board manipulation using 64-bit integers.  
- [**Move Generation**](#move-generation) – Legal and pseudo-legal moves for all pieces.  
- [**Negamax Search**](#negamax) – Implements the negamax variant of minimax with alpha-beta pruning.  
- [**Quiescence Search**](#quiescence-search) – Extends leaf nodes to reduce the horizon effect.  
- [**Move Ordering**](#move-ordering) – Includes killer moves, history heuristics, and principal variation (PV) search.  
- [**Evaluation Function**](#evaluation) – Basic material + positional evaluation (can be extended).  
- [**UCI Protocol**](#uci-protocol) – Communicates with GUIs like **Arena**, **CuteChess**, or **lichess-bot**.  
- [**Perft Testing**](#perft) – Verify correctness of move generation.  
- [**Principle variation search**](#pv-search) - Improves the performance up to 10%
- [**Late move reduction**](#late-move-reduction) - Improves the performance
- [**Null move pruning**](#null-move-pruning) - Improves the performance
- [**Iterative deepening**](#iterative-deepeningd) - Give the best move so far in the tree if engine runs out of time
- [**Zoobrist hashing**](#zoobrist-hashing) - Giving the "IDs" to the board positions
- [**3 fold repetition**](#3-fold-repetition) - if the moves are about to be repeated return draw score for negamax ad quesience search

## References
1. Youtube tutorials and Github repos

This project is inspired by and follows the tutorial series from Maksim Korzh:
- Youtube series: https://youtube.com/playlist?list=PLmN0neTso3Jxh8ZIylk74JpwfiWNI76Cs&si=Sd3jy2bCRtksQ3oJ
- Github repo of his engine: https://github.com/maksimKorzh/bbc.git

This project also follows the tutorial series from Richard Albert (BluefeverSoftware)
- Youtube series: https://youtube.com/playlist?list=PLZ1QII7yudbc-Ky058TEaOstZHVbT-2hg&si=IY0aUhYoSW81WAQd
- Github repo of his engine: https://github.com/bluefeversoft/vice.git

**NOTE: The BBC chess engine from Maksim Korzh has the same philosophy for pv tables and time controls as the VICE and TSCP chess engines.**

2. Principle Variation Search by Bruce Moreland: 
- https://web.archive.org/web/20071030220825/http://www.brucemo.com/compchess/programming/pvs.htm#expand
3. Manimax and alpha-beta prunning 
- MIT OpenCourseWare: https://youtu.be/STjW3eH0Cik?si=kpKddQLv2FGQYds4
- Sebastian Lague youtube video: https://www.youtube.com/watch?v=l-hh51ncgDI
- Artificial Intelligence and Machine Learning Fundamentals - Zsolt Nagy, pg. 70, ISBN 978-86-7310-544-4
4. Magic numbers and magic bitboard for move generation
- TimosPal, repo: https://github.com/TimosPal/NNUE-Bitboard-Chess-Engine
- Chess programming wiki: https://www.chessprogramming.org/Magic_Bitboards
5. Time controls by VICE chess engine made by Richard Allbert
- Github repo of the egnine: https://github.com/bluefeversoft/vice
- Time control code: https://github.com/bluefeversoft/vice/blob/main/Vice11/src/misc.c
6. The PV table implementation if from TSCP chess engine
- https://www.chessprogramming.org/TSCP
- https://www.tckerrigan.com/Chess/TSCP/
7. UCI protocol
- https://www.chessprogramming.org/UCI
- Weiss chess engine by Richard Olbert: http://github.com/TerjeKir/weiss
8. FEN
- https://www.chess.com/terms/fen-chess
9. Late move reduction
- https://web.archive.org/web/20150212051846/http://www.glaurungchess.com/lmr.html
10. Perft
- https://www.chessprogramming.org/Perft
11. Null move pruning by Bruce Moreland
- https://web.archive.org/web/20071031095933/http://www.brucemo.com/compchess/programming/nullmove.htm
12. Window Aspiration by Bruce Moreland
- https://web.archive.org/web/20071031095918/http://www.brucemo.com/compchess/programming/aspiration.htm
13. Transposition table - Bruce Moreland
- https://web.archive.org/web/20071031100051/http://www.brucemo.com/compchess/programming/hashing.htm
14.  Exploring modern chess engine architectures - Bechlor Thesis - Pieter Bijl, Anh Phi Tiet 
- https://www.cs.vu.nl/~wanf/theses/bijl-tiet-bscthesis.pdf

# Running the engine
This engine works with UCI protocol and every GUI that supports UCI can start the bot. The most popular GUI for chess engines are:
- Arena
- Lucas Chess
- Tarrasch
- Cute Chess


There's executables in ```bin``` folder. 
- For ***Windows*** load the ```cincinnatus_windows_release.exe```
- For ***Linux distros*** load the ```cincinnatus_linux_release```

## Build Instructions (If the executables are not system compatable)
### Requirements
- GCC compiller
- make tool for reading and executing the makefile

### Compile
Clone the repo
```bash
git clone https://github.com/boce1/cincinnatus.git
cd cincinnatus
```

To compile the realise vesrion
```bash
make
```

If your OS is `Windows` and `GCC` is added to the path, you can compile with
```bash
make linux_debug
```


After compilation load the executable into the UCI GUI.

## Name of the engine
This bitboard chess engine is named after the statesman Lucius Quinctius Cincinnatus in early Roman Republic (5th BC). When Rome was threatened by the Aequi, the Senate appointed him dictator, an emergency office with supreme authority. Cincinnatus raised a citizen army, defeated the Aequi in just over two weeks, and, instead of clinging to power, resigned his dictatorship and returned to his farm. Cincinnatus became a symbol of Roman ideals — duty, service, and humility. Later generations admired him as the man who chose the plow over permanent power.

Under certain conditions, the idea of a dictator can sound appealing, like when a democracy isn’t functioning as it should due to corruption or political polarization. People may believe the solution is a "benevolent dictator"— a leader who only uses their absolute power for the common good. But can such a leader actually exist in today’s world?

TED-Ed video essay: "Has there ever been a good dictator?" - Stephanie Honchell Smith (https://www.youtube.com/watch?v=kkZ7B-Fv-ck&t=11s)

# Bitboards
Bitboards (also called bitmaps) are unsigned 64-bit integers. They are used to represent the state of the board. This method offers great computational speed because bitboard can be manipulated with binary operation. Modern computers are 64-bit arhitectures wich allows ussability of 64 bit registers. Binary operations with 64bit registers are much faster than the standard 2d representation of the board.

# Board
Board is a structure that contains occupancy of all 12 pieces (pawn, rook, knight, bishop, queen. king for black and white), occupancies for white and black pieces and occupancy of all pieces of the board. The occupancies are represented as bitboards. Board also contains variables for wich side is to move, en passant sqaure and castling rights.

# Move generation
For move generation there's two move categories: pseudo moves and legal moves. A pseudo move is a move that is legal if we leave the fact that the move might make the king in check or let the king be checked. Legal move is a move that doesn't allow the king to be in check position. Algorithm first generates the pseudo moves then it filtrates them afterwards. All the attacks are pre-generated at startup. This way getting the pseudo moves is faster because theres no need to calculate them on the fly. Exception in this implementation are pawn pushes and king castles. They are being calculated on the fly.

In context of generating moves there's two types of pieces: 
- leaper pieces (pawns, king, knight)
- slider pieces (rook, bishop, queen)

For slider pieces getting the attack mask is done by multiply-right-shift perfect hashing algorithm. Depending on the relevant occupancy bits, function gives the index of the attack bitboard. 
Terminology:
- Relevant occupancy bits are bitmask that represents the squares that have pieces that are in the tragectory of slider piece attack. The edge squares aren't included because they are not in the way of the rays (Rays end at the edge squares). 
- Magic number is the unique unsigned 64 bit integer when being multiplied with relevant occupancy bitmask gives consecutive combination of the masked bits. The multiplication operation puts all bits in the most significant bits part. That combination is then right-shifted creating the index of the attack mask. Magic numbers can be generated by brute-force trial and error.

How does it work: (For more detailed explanation check the [Refrence section](#references))
1. Mask the relevant occupancy bits to form a key
2. Multiply the key by a "magic number" to obtain an index mapping
3. Right shift the index mapping by 64-n bits to create an index, where n is the number of bits in the index. A better magic number will have less bits required in the index
4. Use the index to reference a preinitialized move database

The picture bellow show the visualization of the stuff explained above. It's refrenced from [TimosPal's chess engine](#references).
![magic numbers example](pics/magic_bitboards_example.png)

# Moves
For sipmlicity and performance purposes moves are represented as unsigned 32bit integer. The move contains information about source square, target square, piece, promotion piece (if piece is pawn and its promoted), capture flag, double push flag (if piece is pawn), enpassant flag (if piece is pawn), castiling flag (if piece is king).
- target and source square are encoded as 6 bits each, representing the 64 (2^6) squares on the board
- piece and promotion piece are encoded as 4 bits each, representing the 12 pieces
- capture flag, double push flag, enpassant flag, castling flag are encoded as 1 bit each
- total size of the move is 6 + 6 + 4 + 4 + 1 + 1 + 1 + 1 = 24 bits (3 bytes)

| Binary Representation         | Description             | Hex Value  |
|-------------------------------|-------------------------|------------|
| `0000 0000 0000 0000 0011 1111` | Source square mask      | `0x3f`     |
| `0000 0000 0000 1111 1100 0000` | Target square mask      | `0xfc0`    |
| `0000 0000 1111 0000 0000 0000` | Piece mask              | `0xf000`   |
| `0000 1111 0000 0000 0000 0000` | Promotion piece mask    | `0xf0000`  |
| `0001 0000 0000 0000 0000 0000` | Capture flag mask       | `0x100000` |
| `0010 0000 0000 0000 0000 0000` | Double push flag mask   | `0x200000` |
| `0100 0000 0000 0000 0000 0000` | En passant flag mask    | `0x400000` |
| `1000 0000 0000 0000 0000 0000` | Castling flag mask      | `0x800000` |

Castiling rights is also represented as 32bit integer where the first bit is the right for white to castle on king side, second bit is the right for white to castle on queen side, third bit is the right for black to castle on king side, forth bit is the right for black to castle on the queen side. wk = 1, wq = 2, bk = 4, bq = 8.
The rights for castle are updated with move. When move is made, it bitwise castling right with constants for removing the bits. 

new_castling_rights = castling_rights & move_update
| Move                        | Binary | Decimal |
|-----------------------------|--------|---------|
| White king moved            | `1100` | 12      |
| White king's rook moved     | `1110` | 14      |
| White queen's rook moved    | `1101` | 13      |
| Black king moved            | `0011` | 3       |
| Black king's rook moved     | `1011` | 11      |
| Black queen's rook moved    | `0111` | 7       |

Moves are stored in move list structure that constains count of moves at the given positon and array of ints with size of 256 (maxumum numbers of moves at the most complex position).

# Negamax
For more detailed explanation check the [links for Minimax in the Reference section](#references).

Negamax is version of minimax where instead of having two separate function for minimazing and maximazing there's only one function that flips the sign of the score. ```max(score) = -min(-score)```. Intuition is what is good for the opponent is bad for me and vice versa.
Alpa-beta prunning is technic that cuts off branches where it's sure the opponent won't go that path. The branches that we know are uselles to visit, recursive negamax call isnt done on them. 

Alpha is variable that holds the best value for maximazing player and Beta is the best value for minimazing player. If Alpha >= Beta at any point then other branches are prunned (It breaks out of the loop in the move list).
- Fail low: the score of the move is less or equal than Alpha wich means the move is too bad for maximazing player then the branch is pruned.
- Fail high: the score of the move is greater or equal to beta wich means opponent won't allow it.
- If the score is in between, it is optimal and it is Principla Variation move. (Principal Variation node is the principla variation move that is on the line of the most optimal sequence of move for both sides).

```score(state) = -negamax(next_position, -Beta, -Alpha, depth-1)```

In negamax there's no need to check fail low moves because when the side is flipped negamax prunes in the fail hight part. Meaning both players now look for fail-high cutoffs only. What would be a fail-low in minimax automatically becomes a fail-high after the sign flip.

# Evaluation
When the depth of 0 is reached negamax evaluates board. The evaluation is done by adding material score of the pieces. Also to the score is added bonus or penalty for a specific piece being on a specific square. These values are taken from piece-square tables that represent 8x8 grid. 
- Exaple 1: Knights are stronger in the center of the board and they are weaker on the edges. 
- Example 2: Evaluation gives more bonuses to pawn that are closer to the opposite edge because pawn will get promoted. 

# Quiescence search
Quiescence search is technique used in game tree search to reduce the horizon effect. It extends search in tactical positions untel they settle down or moves become quiet, then it evaluates the board. Instead of stopping the search immediately at depth 0, the engine continues searching only "noisy" moves (Captures, Checks, Promotions). 

Horion effect happens when the search algorithm cannot see far enough ahead due to limited depth and therefore delays recognizing an inevitable bad event as if the event is "just beyond the horizon".

# Move ordering
Move ordering is the technique that sort generated moves based on points. How much point move gets depends how good the move is. If the move is killer move or is in history moves or is PV node or makes more material score then gets the most points and its sorted at the begging of the move_list array. Putting the good moves first makes negamax search more likely to prune bigger parts of the tree and that makes search faster and more efficient.

### Movest valuable vicim - Least valuable attacker
This heuristic prefers moves where the most valuable pieces (queen > rook > bishop / knight > pawn) are being captured by least valuable pieces. 
### PV moves
PV moves are moves that are on the best line of play for both players. In negamax thes move is being search first because it has the best chance for pruning the biggest part of the tree. For every half move counter the PV move is being saved and the PV moves from deeper depth are being coppied. The PV table structure is from [TSCP chess engine](#references).
### History moves
This heuristic that saves non-catpures moves for every piece that made cutoff previously. 
### Killer moves
Killer move is non-capture move that causes a beta cutoff at a given depth. For each depth there are 2 moves stored. 
Engines store two killer moves per depth because:
- One may overwrite the other (redundancy).
- Different branches might have different strong quiet moves.
- Two gives most of the benefit — more than two usually adds noise and slows things down.

# UCI protocol
[UCI (Universal Chess Interface)](#references) is communication protocol that is designed to work with GUIs. Supported commands by this engine:
- ```parse startpos``` - puts the board in the starting position
- ```parse fen``` ```fen string``` - change the board state based on the fen
- ```parse fen``` ```fen string``` ```move list``` - change the board state based on the fen and makes the moves in the move list
- ```go``` ```depth``` - searches for the best move at the given position. Does the negamax search at the specified depth. If the depth isnt specified, default depth is 6.
- ```uci``` - test if the engine supports UCI protocol
- ```quit``` - stops the engine
-  ```ucinewgame``` - initialize the board the start position and starts the game

### FEN
Fen is standard notation for discribing the board. The rows are devided with "/", white pieces are represented as capital letters K - King, Q - Queen, B - Bishop, N - Kning, R - Rook, P - Pawn. For black pieces is the same except they are represented with lowercase letters. Empty square are represented with positive integer number wich tells how many consecutive empty square is there on the row. For more detail check the [FEN link in the reference section](#references).

# PV search
The main idea behind [Principal Variation Search](#references) searching the PV nodes with full window search. Other nodes are being search with reduced window search. In the case that the move gives the score in alpha-beta region it means that it hasnt failed low or failed high, wich mean the move might have been good. In that case that move is being search with full window search.

It's implemented in ```search_heuristics``` data scructure in ```zoobrist_hash.c```. Its a 2D table that stores the moves in triangle. When negamax or quesience search get to the last recursion layer it store the pv move in the n-th row and n-th column. When returning in the upper layers in the (n-1)-th row pv move is determined for the board and the pv moves bellow are being copied.
The search uses a triangular PV table to store the best move sequence (the Principal Variation) found at each ply. As the search moves deeper, the PV line is copied upward to maintain the full sequence from the root.

### PV Storage Visualization

| Start Ply | Ply 0 | Ply 1 | Ply 2 | Ply 3 | Ply 4 |
| :---: | :---: | :---: | :---: | :---: | :---: |
| **0** | $m_1$ | $m_2$ | $m_3$ | $m_4$ | $m_5$ |
| **1** | — | $m_2$ | $m_3$ | $m_4$ | $m_5$ |
| **2** | — | — | $m_3$ | $m_4$ | $m_5$ |
| **3** | — | — | — | $m_4$ | $m_5$ |
| **4** | — | — | — | — | $m_5$ |

### How it works:
1. **Row Index:** The ply at which the search is currently working.
2. **Column Index:** The depth of the move in the predicted sequence.
3. **Copying Logic:** When a move improves Alpha (a "PV-node"), the engine updates the current ply's row by taking the current move and appending the PV line from the next ply (`ply + 1`).

```c
// the code bellow is in negamax.c in negamax functinon

// at the begginning of negamax 
search_data->pv_lenght[search_data->ply] = search_data->ply;
//------------------------------------
// if better move is found (score > alpha)
search_data->pv_table[search_data->ply][search_data->ply] = move_list->moves[count];
// copy move from deeper ply into a current plys line
for(int next_ply = search_data->ply + 1; next_ply < search_data->pv_lenght[search_data->ply+1]; next_ply++) {
    search_data->pv_table[search_data->ply][next_ply] = search_data->pv_table[search_data->ply + 1][next_ply];

search_data->pv_lenght[search_data->ply] = search_data->pv_lenght[search_data->ply + 1]; 

}
```

# PERFT
[PERFT (Performance Test)](#references) is debbuging function that tells how many leaf nodes move generator gives at the given position at the certain depth. Move generator of this engine passed PERFT. 
# Late move reduction
The engine searches the first few moves deeply, since they are the most prommising. For later, less prommising moves (late moves) the engine reduces the search window.
# Null move pruning
The main idea behing this tecnhique is if the current side skips the turn will the opponent make good move. It works because strongs position dont lose their value after one skipped move. If the opponent can make a good move then do regular negamax search, if not make a cutof. Null move pruning doesnt work with Zugzwang positions. Passing a move might actually make the position worse. For null move en passant is disabled, because doing en passant loses its right after not making it immidiently.
# Iterative deepening
Iterative deepening (or iterative deepening depth-first search, IDDFS) is a technique where the engine searches the game tree repeatedly, increasing the search depth by one ply each time, until time runs out or the maximum depth is reached.
1. Start with depth = 1, run alpha-beta search.
2. Increase depth = 2, run alpha-beta search again.
3. Keep increasing depth until the allotted time is used.
4. The best move found at the deepest completed depth is chosen.

Why is it userfull
- Move ordering improvement: Each shallow search produces a principal variation (PV). At the next depth, those PV moves are searched first, improving alpha-beta cutoffs.
- Time management: The engine always has a "best move so far" even if time runs out suddenly.
- Better heuristics: Killer moves, history heuristics, and transposition tables all benefit from information collected in earlier iterations.
# Window Aspiration
The idea lies in the assumtion that in the iterative deepening the next layer wont give much of a different score from the previous one and thats why to the alpha-beta window is being reduced, prudocing more cutofs.
# Zoobrist hashing
Zobrist hashing is a way to assign a unique hash value to a game state (chess position) in a very efficient manner. It’s designed so you can update the hash quickly when a move is made, instead of recomputing everything from scratch.

In board games the same position can be reached with different move orders. Without the hashing, the engine will treat it as the separate nodes and it will waste time. The zoobrist hashing alg will produce the same hash and engine wont analyze the position more than once. This method is very fast, because instead of comparing the entire board, engine will compare unsigned 64-bit integers. Also because the alg is working with 64-bit unsigned int, it is rare for hash collisions to occur.

Speciffictly, one of the use cases is checking the cycle repetition. Previous position can be stored and lately checked.

Representation of the board is really compact, because it is represented as 64-bit unsigned ints and that allows storing a large numbers of hashes of the boards.

It is also used for their speed, because updating them is xor-ing the previous position and xor-ing the new position with the current hash, instead of recomputing the board all the time.
## 3 fold repetition
In modern GUIs that supports UCI, when move is repetead 3 times GUI sends signal to stop the game and the oppenent wins.

It's implemented with help of zoobrist hashing. When a move is already been visited its written in repetion_data structure that keep track of the index of visited board and the boards themself. If the board is about to repeat even once return draw score of 0.