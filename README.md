# Quoridor engine based on minimax with alpha-beta pruning

Wiki: https://en.wikipedia.org/wiki/Quoridor <br/>
YouTube Tutorial Video: https://www.youtube.com/watch?v=6ISruhN0Hc0 <br/>
Play the game [here](http://quoridor.di.uoa.gr)

## **Rules**
* **Board:**
Quoridor is played on a game board of 81 square cells (9x9) and each player starts with 10 walls. However, the game can be also played in different board sizes with a different number of starting walls.

* **Game setup:**
Each player is represented by a pawn (black or white) which is placed at the center space of the baseline.

* **Gameplay:**
The black player starts first, then players alternate turns. Each player in turn, chooses to move his pawn or to put up one of his walls. If he runs out of walls, the player can only move his pawn.

* **Pawn moves:**
The pawns are moved one square at a time, horizontally or vertically, forwards or backwards. The pawns must get around the walls. When two pawns face each other on neighboring squares which are not separated by a wall, the player whose turn it is can jump the opponent's pawn (and place himself behind him), thus advancing an extra square. If there is a wall behind the said pawn, or the square behind him is out of the board, the player can place his pawn to the left or the right of the other pawn.

* **Positioning of the walls:**
Walls are flat two-cell-wide pieces which can be placed between 2 sets of 2 squares. The walls can be used to facilitate the player's progress or to impede that of the opponent, however, an access to the goal line must always be left open.

* **End of the game:**
The first player who reaches any of the squares opposite of his baseline is the winner.

## **Ingame commands**
The commands follow the protocol that can be viewed in detail [here](http://quoridor.di.uoa.gr/qtp/qtp.html). In short:
> **playmove** player position <br/>
  *example*: playmove white e2 - move white's pawn to E2
  
> **playwall** player position orientation <br/>
  *examples*:<br/>
  playwall black a3 horizontal(**OR** h) - black places a horizontal wall starting at A3 <br/>
  playwall white b4 vertical(**OR** v) - white places a vertical wall starting at B4 <br/>
  
> **genmove** player <br/>
  *example*: genmove white - engine plays a move for white
 
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

## **Engine**
The engine is based on [Minimax](https://en.wikipedia.org/wiki/Minimax) with [Alpha-beta pruning](https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning). 
* The depth where it's running at is calculated based on the board size, current number of walls as well as total moves played. A so-called pseudo-depth system (for certain boardsizes) is also used, where at the last depth we don't search all of the possible moves, but only key ones (pawn movement and wall placement near the enemy's pawn). This is used to further boost the engine lookahead ability and make the depth, when necessary, an even number.
* The evaluation uses a system similar to chess's one, where if the evaluation is positive it is believed that white has an advantageous position, if it is negative it's believed that black is the one with the advantage and if it is 0, neither player has an advantage. The way we evaluate positions is by using 3 pieces of information. The first, is the steps both players need in order to win by using a factor of 1. The second, is the number of steps each player needs in order to get to the next row by using a factor of 0.6. The third, and final, is the number of walls each player has remaining (factor 0.7). So, the evaluation comes as follows: <br/>
> blackDistanceFromWinning-whiteDistanceFromWinning + 0.6*(blackDistanceFromNextRow - whiteDistanceFromNextRow) + 0.7*(whiteWalls - blackWalls) <br/>

## **Referee**
A referee `quoridor_referee.py` that allows two programs that follow the protocol mentioned above to play against each other is also provided. Check the `makefile` for its usage at referee settings section.

## **Compilation**
- `make` to compile </br>
- `make play` to compile and play </br>
- `make clear` to clear excess object files created during compilation </br>
- `make ref` to use the referee, change referee settings at the makefile 

## **About**
The project is based on assignment 4, semester 1, dpt. of Informatics and Telecommunications - UoA (2021), where it won first place at the department's local competition. Other programs, including but not exclusively, from the competition can be found over at the `otherProgs` folder.

## **Authors**
- [Pavlos Dais](https://github.com/pavlosdais)
- [Stavros Prentzas](https://github.com/stavrosprentzas)
