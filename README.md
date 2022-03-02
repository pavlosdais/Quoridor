# Quoridor

* **Board**:
Quoridor is played on a game board of 81 square cells (9x9) and each player starts with 10 walls. However, the game can be also played in different boardsizes and with a different number of starting walls.

* **Game setup**:
Each player is represented by a pawn (black or white) which begins at the center space of his baseline.

* **Gameplay**:
The black Player starts first, then players alternate turn. Each player in turn, chooses to move his pawn or to put up one of his fences. When he has run out of fences, the player must move his pawn.

* **Pawn moves**:
The pawns are moved one square at a time, horizontally or vertically, forwards or backwards. The pawns must get around the fences. When two pawns face each other on neighboring squares which are not separated by a fence, the player whose turn it is can jump the opponent's pawn (and place himself behind him), thus advancing an extra square. If there is a fence behind the said pawn, or the square behind him is out of the board, the player can place his pawn to the left or the right of the other pawn.

* **Positioning of the fences**:
Walls are flat two-cell-wide pieces which can be placed between 2 sets of 2 squares. The fences can be used to facilitate the player's progress or to impede that of the opponent, however, an access to the goal line must always be left open.

* **End of the game**:
The first player who reaches any of the squares opposite his baseline is the winner.


* > Make the executable file: make
* > Clear excess object files: make clear
* > Run the program: make play

Problem set 4, Semester 1, Department of Informatics and Telecommunications - UoA (2022)
