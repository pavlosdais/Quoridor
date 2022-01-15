#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function Prototypes
int mode(char *ans);
char valid_number_of_walls(int walls);
void unsuccessful_response(char *msg);
void list_commands();

/*
    Commands:
    -Adminstrative
    1 - name
    2 - known_command
    3 - list_commands
    4 - quit
    -Setup
    5 - boardsize
    6 - clear_board
    7 - walls
    -Core Play
    8 - playmove
    9 - playwall
    10 - genmove
    11 - undo
    -Tournament
    12 - winner
    -Debug
    13 - showboard
*/
int main(void)
{
    char winner, buff[80], *p, m, *parameters;
    int black_walls = -1, white_walls = -1, walls, i;
    
    while (1)
    {
        scanf("%s", buff);
        p = strtok_r(buff, " ", &parameters);
        m = mode(p);
        if (m == 1)  // name
        {
            printf("Entered 1\n");
        }
        else if (m == 2)  // known_command
        {
            printf("Entered 2\n");
        }
        else if (m == 3)  // list_commands
        {
            printf("Entered 3\n");
            list_commands();
        }
        else if (m == 4)  // quit
        {
            break;
        }
        else if (m == 5)  // boardsize
        {
            printf("Entered 5\n");
        }
        else if (m == 6)  // clear_board
        {
            printf("Entered 6\n");
        }
        else if (m == 7)  // walls
        {
            printf("Entered 7\n");
            while(p != NULL)
            {
                printf("%s\n", p);
                p = strtok(NULL, " ");
            }
        }
        else if (m == 8)  // playmove
        {
            printf("Entered 8\n");
        }
        else if (m == 9)  // playwall
        {
            printf("Entered 9\n");
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
            printf("Entered 12\n");
        }
        else if (m == 13)  // showboard
        {
            printf("Entered 13\n");
        }
        else  // command not recognized
        {
            unsuccessful_response("unknown command");
        }

        /*
        while(p != NULL)
        {
            //printf("%s\n", p);
            p = strtok(NULL, " ");
        }
        */
    }
    return 0;
}

int mode(char *ans)
{
    if (strcmp("name", ans) == 0) return 1;
    else if (strcmp("known_command", ans) == 0) return 2;
    else if (strcmp("list_commands", ans) == 0) return 3;
    else if (strcmp("quit", ans) == 0) return 4;
    else if (strcmp("boardsize", ans) == 0) return 5;
    else if (strcmp("playwall", ans) == 0) return 6;
    else if (strcmp("walls", ans) == 0) return 7;
    else if (strcmp("playmove", ans) == 0) return 8;
    else if (strcmp("playwall", ans) == 0) return 9;
    else if (strcmp("genmove", ans) == 0) return 10;
    else if (strcmp("undo", ans) == 0) return 11;
    else if (strcmp("winner", ans) == 0) return 12;
    else if (strcmp("showboard", ans) == 0) return 13;
}

char valid_number_of_walls(int walls)
{
    if (walls % 2 == 1)  // valid
        return 1; 
    return 0;  // non-valid
}

void unsuccessful_response(char *msg)
{
    printf("? %s\n\n", msg);
    fflush(stdout);
}

void list_commands()
{
    printf("protocol_version\nname\nversion\nknown_command\nlist_commands\nquit\nboardsize\n");
    printf("clear_board\nwalls\nplaymove\nplaywall\ngenmove\nundo\nwinner\nshowboard\n");
}

