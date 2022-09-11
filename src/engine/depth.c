#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "../../include/typedefs.h"

/* The depth of the engine is calculated based on 3 parameters. The board size, the number of total moves played and the total number of available walls.
The strength of the engine is basically determined here, increase the depth numbers to make the engine more powerful, abeit slower */
small_int findDepth(cint boardsize, char* pseudo, float* max_time, cint total_moves, cint total_walls)
{
    /* pseudodepth is used to simulate a further search to make the depth even.
    If pseudodepth is true the last step will only check opponent's "most likely"
    responses instead of all possible answers. This means it will check pawn movement
    and wall placement near the enemy pawn to disrupt it's movement */

    *max_time = 9.2;  // max branch, search time

    // first few moves do not really require much depth, just search at depth 1
    if (total_moves + 3 < boardsize)
        return 1;
    
    small_int depth;
    if (boardsize < 6)  // boardsize = 5
    {
        *max_time = 12;
        depth = 8;
        if (total_walls < 3)
        {
            *max_time = 15;
            depth = 12;
        }
        else if (total_walls < 5)
            depth = 10;
    }
    else if (boardsize < 8)  // boardsize = 7
    {
        depth = 4;
        if (total_walls > 1)
        {
            if (total_walls < 4)
               depth = 8; 
            else if (total_walls < 6)
            {
                *pseudo = 1;
                depth = 8;
            }
            else if (total_walls < 9)
                depth = 6;
            else if (total_walls < 13)
            {
                *pseudo = 1;
                depth = 6;
            }
        }
    }
    else if (boardsize < 10)  // boardsize = 9
    {
        depth = 4;
        if (total_walls > 1 && total_walls < 7)
        {
            // pseudo-depth 6
            depth = 6;
            *pseudo = 1;
        }
    }
    else if (boardsize < 12)  // boardsize = 11
    {
        *pseudo = 1;
        depth = 4;
        if (total_walls < 8)
            depth = 6;
        else if (total_walls < 16)
            *pseudo = 0;
    }
    else if (boardsize < 14)  // boardsize = 13
    {
        depth = 3;
        if (total_walls < 14)
        {
            *pseudo = 1;
            depth = 4;
        }
        else if (total_walls < 7)
            depth = 6;
    }
    else if (boardsize < 16)  // boardsize = 15
    {
        depth = 2;
        if (total_walls < 13)
        {
            *pseudo = 1;
            depth = 4;
        }
        else if (total_walls < 5)
            depth = 6;
    }
    else if (boardsize < 20)  // higher boardsizes
    {
        *pseudo = 1;
        depth = 2;
    }
    return depth;
}
