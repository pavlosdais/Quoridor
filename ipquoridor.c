#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "commands.h"

int main(int argc, char* argv[])
{
    char* p, m, panic;
    // default values
    int boardsize = 9, prev_boardsize = 9, number_of_walls = 10;

    player black, white;
    reset_pawns(boardsize, &white, &black);  // default walues
    black.walls = white.walls = 10;
    
    char** wall_matrix = allocate_memory(boardsize);
    if (wall_matrix == NULL)
    {
        unsuccessful_response("allocation failure");
        return -1;
    }
    
    stackptr history = NULL;
    int totalmoves = 0;
    char* buff = malloc(sizeof(char) * BUFFER_SIZE);
    while (!panic)
    {
        fgets(buff, BUFFER_SIZE, stdin);
        if (command_preprocess(buff) == true) continue;
        if (buff[0] == '\0') 
        {
            unsuccessful_response("unknown command");
            continue;
        }
        p = strtok(buff, " "); // command
        
        if ((m = command_num(p)) == 1)  // name
            print_name("21-122.21-164");

        else if (m == 2)  // known_command
            known_command();

        else if (m == 3)  // list_commands
            list_commands();

        else if (m == 4)  // quit
        {
            successful_response("");
            break;
        }

        else if (m == 5)  // boardsize
        {
            if (update_boardsize(&boardsize, &prev_boardsize, &wall_matrix, &white, &black, &history, &totalmoves) == false) panic = 1;
        }
        else if (m == 6)  // clear_board
            clear_board(boardsize, wall_matrix, white, black, &history, &totalmoves);

        else if (m == 7)  // walls
            update_walls(&white, &black, &number_of_walls);

        else if (m == 8) // playmove
            playmove(buff, &white, &black, wall_matrix, boardsize, &history, &totalmoves);

        else if (m == 9)  // playwall
            playwall(buff, &white, &black, wall_matrix, boardsize, &history, &totalmoves);

        else if (m == 10)  // genmove
        {
            if (genmove(&white, &black, wall_matrix, boardsize, &history, &totalmoves) == false) panic = true;
        }
        
        else if (m == 11)  // undo
            undo(wall_matrix, &white, &black, &history, &totalmoves);

        else if (m == 12)  // winner
            winner(white, black, boardsize);

        else if (m == 13)  // showboard
            showboard(wall_matrix, boardsize, &white, &black);   

        else  // command not recognized
            unsuccessful_response("unknown command");
    }

    // clear history
    stackptr temp = history;
    while (temp != NULL)
    {
        free(temp);
        history = history->next;
        temp = history;
    }
    free(history);
    free(buff);
    free_array(wall_matrix, boardsize);
    free(wall_matrix);
    if (panic == true) return -1;
    return 0;
}
