#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../include/structs.h"
#include "../include/commands.h"

#define STARTING_WALLS 10

int main(int argc, char* argv[])
{
    // initialize board
    int prev_boardsize = 9;
    gameState game_state = game_state_init(STARTING_WALLS, prev_boardsize);

    stackptr history = NULL;

    char* p, m, panic = false;
    
    char* buff = malloc(sizeof(char) * BUFFER_SIZE);
    assert(buff != NULL);  // allocation failure
    
    while (!panic)
    {
        // printf("Position id: %llx\n", game_state->position_id);
        fgets(buff, BUFFER_SIZE, stdin);
        if (buff[0] == '\n') continue;
        if (command_preprocess(buff) == true) continue;  // hash sign
        if (buff[0] == '\0')
        {
            unsuccessful_response("unknown command");
            continue;
        }
        p = strtok(buff, " ");  // command
        
        if ((m = command_num(p)) == 1)  // the name of the program
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
            if (game_state->totalmoves > 0)  // clear history
            {
                stackptr temp = NULL;
                while (history != NULL)
                {
                    temp = history;
                    history = history->next;
                    free(temp);
                }
            }
            if (update_boardsize(game_state, &prev_boardsize, &history) == false) panic = true;
        }
        else if (m == 6)  // clear_board
        {
            if (game_state->totalmoves > 0)  // clear history
            {
                stackptr temp = NULL;
                while (history != NULL)
                {
                    temp = history;
                    history = history->next;
                    free(temp);
                }
            }
            clear_board(game_state);
        }
        else if (m == 7)  // walls
            update_walls(game_state);
            
        else if (m == 8) // playmove
        {
            playmove(game_state, &history);
        }

        else if (m == 9)  // playwall
            playwall(game_state, &history);

        else if (m == 10)  // genmove
            genmove(game_state, &history);

        else if (m == 11)  // undo
            undo(game_state, &history);

        else if (m == 12)  // winner
            winner(game_state);

        else if (m == 13)  // showboard
            showboard(game_state);
        
        else  // command not recognized
            unsuccessful_response("unknown command");
    }

    if (game_state->totalmoves > 0)
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
    destroy_game(game_state);
    free(buff);
    
    if (panic == true)
    {
        unsuccessful_response("allocation failure");
        return 1;
    }

    return 0;
}
