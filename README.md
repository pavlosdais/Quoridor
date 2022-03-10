# Quoridor AI based on minimax with alpha-beta pruning

Wiki: https://en.wikipedia.org/wiki/Quoridor <br/>
YouTube Tutorial Video: https://www.youtube.com/watch?v=6ISruhN0Hc0

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
> **playmove** player position <br/>
  example: playmove white e2 - move white's pawn to E2
  
> **playwall** player position orientation <br/>
  examples:<br/>
  playwall black a3 horizontal(OR h) - black places a horizontal wall starting at A3 <br/>
  playwall white b4 vertical(OR v) - white places a vertical wall starting at B4 <br/>
  
> **genmove** player <br/>
  example: genmove white - engine plays a move for the player specified
 
> **showboard** <br/>
  Draw the board.
  
> **quit** <br/>
  Exit the game.
  
> **undo** n <br/>
  The game goes n moves back.
  
> **boardsize** n <br/>
  The board size is changed to n.
  
> **walls** n <br/>
  The number of walls for each player is changed to n.
  
> **list_commands** <br/>
  List all commands.

**Engine:** <br/>

The engine changes its depth accordingly depending on the boardsize. It's also using a so-called pseudo-depth system (for certain boardsizes) where at the last depth
it doesn't search all of the possible moves, but only key ones (pawn movement and wall placement near the enemy's pawn). This is used to further boost the engine lookahead ability and make the depth an even number. The evaluation uses a system similar to chess's one where if the evaluation is positive it's believed that white hasan advantage,
if it's negative it's believed that black has an advantage and if it's 0, neither player has an advantage. The way it evaluates positions is by using 3 factors. The first
factor is the steps both players need in order to win by using a factor of 1. The second is the number of steps each player needs in order to get to the next row by using a factor of 0.6. The third, and final, factor is the number of walls each player has remaining (factor 0.8). So, the evaluation comes as follows: <br/>
> evaluation = blackDistanceFromWinning-whiteDistanceFromWinning + 0.6*(blackDistanceFromNextRow - whiteDistanceFromNextRow) + 0.8*(whiteWalls - blackWalls) <br/>

The depths for each boardsize are: <br/>
*For boardsizes <= 5: depth = 6 <br/>
*For boardsizes <= 7: depth = 4 <br/>
*For boardsizes <= 9: pseudo-depth = 4 <br/>
*For boardsizes <= 11: depth = 3 <br/>
*For boardsizes <= 17: depth = 2 <br/>
*For boardsizes <= 25: pseudo-depth = 2 <br/>

It's important to note that depth numbers and pseudo-depth were used in order to meet the time requirements for the Quoridor Cup Competition (UoA) so they can be changed in order to make the engine more powerful, albeit, slower. <br/>

Assignment 4, Semester 1, Department of Informatics and Telecommunications - UoA (2022)
> Project made by Pavlos Dais (sdi2100122) and Stavros Prentzas (sdi2100164)
