# Quoridor AI based on minimax with alpha-beta pruning

Wiki: https://en.wikipedia.org/wiki/Quoridor

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

**Ingame commands**:
> playmove <player> <position> <br />
  example: playmove white e2 - move white's pawn to E2
  
> playwall <player> <position> <orientation> <br />
  examples:<br />
  playwall black a3 horizontal(OR h) - black places a horizontal wall starting at A3 <br />
  playwall white b4 vertical(OR v) - white places a vertical wall starting at B4 <br />
  
> genmove <player> <br />
  example: genmove white - engine plays a move for the player specified.
 
> showboard <br />
  Draw the board.
  
> quit <br />
  Exit the game.
  
> undo <n> <br />
  The game goes n moves back.
  
> boardsize <n> <br />
  The board size is changed to n.
  
> walls <br />
  The number of walls for each player is changed to n.
  
> list_commands <br />
  List of commands

Problem set 4, Semester 1, Department of Informatics and Telecommunications - UoA (2022)
> Project made by Pavlos Dais (sdi2100122) and Stavros Prentzas (sdi2100164)
