#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../include/structs.h"
#include "../include/commands.h"

int main(int argc, char* argv[])
{
    char* p, m, panic = false;
    // default values
    int boardsize = 9, prev_boardsize = 9, number_of_walls = 10;

    player black, white;
    reset_pawns(boardsize, &white, &black);  // default walues
    black.walls = white.walls = 10;
    
    char** wall_matrix = allocate_memory(boardsize);
    if (wall_matrix == NULL)  // allocation failure
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
        if (buff[0] == '\n') continue;
        if (command_preprocess(buff) == true) continue;  // hash sign
        if (buff[0] == '\0') 
        {
            unsuccessful_response("unknown command");
            continue;
        }
        p = strtok(buff, " "); // command
        
        if ((m = command_num(p)) == 1)  // name
            print_name("SP Quoridor");

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
            if (totalmoves > 0)  // clear history
            {
                stackptr temp = NULL;
                while (history != NULL)
                {
                    temp = history;
                    history = history->next;
                    free(temp);
                }
            }
            if (update_boardsize(&boardsize, &prev_boardsize, &wall_matrix, &white, &black, &history, &totalmoves) == false) panic = true;
        }
        else if (m == 6)  // clear_board
        {
            if (totalmoves > 0)  // clear history
            {
                stackptr temp = NULL;
                while (history != NULL)
                {
                    temp = history;
                    history = history->next;
                    free(temp);
                }
            }
            clear_board(boardsize, wall_matrix, &white, &black, &history, &totalmoves);
        }
        else if (m == 7)  // walls
            update_walls(&white, &black, &number_of_walls);
            
        else if (m == 8) // playmove
            playmove(buff, &white, &black, wall_matrix, boardsize, &history, &totalmoves);

        else if (m == 9)  // playwall
            playwall(buff, &white, &black, wall_matrix, boardsize, &history, &totalmoves);

        else if (m == 10)  // genmove
            genmove(&white, &black, wall_matrix, boardsize, &history, &totalmoves);

        else if (m == 11)  // undo
            undo(wall_matrix, &white, &black, &history, &totalmoves);

        else if (m == 12)  // winner
            winner(white, black, boardsize);

        else if (m == 13)  // showboard
            showboard(wall_matrix, boardsize, &white, &black);   

        else  // command not recognized
            unsuccessful_response("unknown command");
    }

    if (totalmoves > 0)
    {
        // clear history
        stackptr temp = NULL;
        while (history != NULL)
        {
            temp = history;
            history = history->next;
            free(temp);
        }
    }
    
    free(buff);
    free_array(wall_matrix, boardsize);
    free(wall_matrix);
    
    if (panic == true)
    {
        unsuccessful_response("allocation failure");
        return -1;
    }

    return 0;
}
