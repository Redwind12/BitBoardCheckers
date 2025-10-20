# BitBoard Checkers (C Project)

This is a command-line Checkers game implemented in C using bitboards to represent the board.

## ✅ How to Build

### ✅ Option 1: Using Make (Linux/Mac or Windows with make installed)
```bash
make
```

This creates an executable named:
```
checkers
```

To run:
```bash
./checkers
```

To clean:
```bash
make clean
```

---

## ✅ Gameplay Instructions

When the game starts, the board is printed with rows and columns labeled.

- Dark squares use playable pieces
- Light squares use `.`
- Empty dark squares show `-`

**Piece Legend:**
- Player 1:
  - `r` = regular piece
  - `R` = king
- Player 2:
  - `b` = regular piece
  - `B` = king

**Input Format:**

Enter moves using 4 numbers:
```
from_row from_col to_row to_col
```

✅ Example:
```
2 1 3 0
```

To quit the game:
```
-1
```

---

## ✅ Features Implemented

✔ Bitboard-based board representation
✔ Piece movement (diagonal)
✔ Single captures (jumping opponent pieces)
✔ King promotion when reaching the last row
✔ Turn switching
✔ Win detection when the opponent has no pieces
✔ Board display after each move

---

## ✅ Sample Moves to Test

Use this sequence to test movement, capturing, and promotion:

```
2 1 3 0
5 2 4 3
3 0 5 2
6 3 5 4
5 2 6 1
6 5 5 6
6 1 7 0
5 4 4 3
2 5 3 4
4 3 2 5
-1
```

---

## ✅ File List

```
main.c
checkers.c
bitops.c
bitops.h
Makefile
README.md
CMakeLists.txt
```

Everything needed to compile and run the project is included.

