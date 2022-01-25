#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"

int main(void)
{
    char winner, *p, m, col, orientation, vertex_x, vertex_y, num_of_arguments;
    int i = 0, j, prev_boardsize, number_of_walls;

    char* buff = malloc(sizeof(char) * BUFFER_SIZE);
    
    // default values
    int boardsize = 9;
    player black, white;
    reset_pawns(9, &white, &black);  // default walues
    black.walls = white.walls = 10;
    
    /* 
    Wall_matrix is used to represent if a wall begins next to a specific cell. for example wall_matrix[2][5] informs us
    about whether or not a wall starts next to the cell (3,6) or else F6. For that purpose we use a character.
    If the character is 'b' it means that a horizontal wall of length 2 has been placed below the specific cell and the cell on its right.
    If the character is 'r' it means that a vertical wall of length 2 has been placed on the right of the specific cell and the cell below.
    If not the character is the one with ascii code 0, as initialized by the following calloc. Since no walls cannot stack on top of another 
    or somehow cross, it is not possible that there are walls starting both horizontally below and vertically on the right of the specific cell
    Some examples:
        1.  wall_matrix[3][7] == 'r' means that no horizontal wall starts below H4, but a vertical one does on its right. So there is a vertical 
            wall between H4 and I4, which keeps going between H3 and I3.
        2.  wall_matrix[5][2] == 'b' means that no vertical wall starts on the right of C6, but a horizontal one does below it. So there is a
            horizontal wall between C6 and C5, which keeps going between D6 and D5.
    !It is important to notice that if a cell is 0 it does not mean that there might not be walls below or on its right. For example, even though
    wall_matrix[3][3] might be 0, it does NOT necessarily mean that no wall EXISTS below or on the right of D4, but simply a wall does not START
    there. If wall_matrix[3][2] is 'b', the wall starting below C4 keeps going below D4, or if wall_matrix[4][3] is 'r', the wall starting on the
    right of D5 keeps going on the right of D4. The same applies for cells with 'r'/'b'. Even though a wall does not start
    beneath them / on their right, it is possiblr that a wall starts below their left cell / on the right of the cell above and keeps going adjacently
    to the specific cell.
    */

    char **wall_matrix = allocate_memory(boardsize);

    while (1)
    {
        fgets(buff, BUFFER_SIZE, stdin);
        command_preprocess(buff);

        num_of_arguments = arguments(buff);

        if (buff[0] == '#') continue;

        // command
        p = strtok(buff, " ");

        char m = command_num(p);  // number of the command
        
        if (m == 1)  // name - done
        {
            print_name(" SP Quoridor");
        }
        else if (m == 2)  // known_command - done
        {
            known_command();
        }
        else if (m == 3)  // list_commands - done
        {
            list_commands();
        }
        else if (m == 4)  // quit - done
        {
            successful_response("");
            break;
        }
        else if (m == 5)  // boardsize - done
        {
            update_boardsize(&boardsize, &prev_boardsize, &wall_matrix, &white, &black);
        }
        else if (m == 6)  // clear_board - done
        {
            clear_board(boardsize, wall_matrix, &white, &black);
        }
        else if (m == 7)  // walls - done
        {
            update_walls(&black, &white, &number_of_walls);
        }
        else if (m == 8) // playmove - STAVROS
        {
            // Color
            p = strtok(NULL, " ");
            col = check_color(p);

            // Vertex
            p = strtok(NULL, " ");
            if (strlen(p) != 2)
            {
                unsuccessful_response("illegal move");
                continue;
            }

            vertex_x = p[0];
            vertex_y = p[1];
        }
        else if (m == 9)  // playwall - PAVLOS
        {
            playwall(buff, &white, &black, wall_matrix);
        }
        else if (m == 10)  // genmove
        {
            printf("Entered 10\n");
        }
        else if (m == 11)  // undo
        {
            printf("Entered 11\n");
        }
        else if (m == 12)  // winner
        {
            winner(white, black, boardsize);
        }
        else if (m == 13)  // showboard - done
        {
            showboard(wall_matrix, boardsize, &black, &white);    
        }
        else  // command not recognized
        {
            unsuccessful_response("unknown command");
        }
        
    }
    free_array(wall_matrix, boardsize);
    free(buff);

    return 0;
}
