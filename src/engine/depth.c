#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "../../include/typedefs.h"
#include "../../include/structs.h"
#include "../../include/utilities.h"

#define SCOPE 1.5

small_int findBoardsize11(char* pseudo, float* max_time, float* scope, cint total_walls)
{
    *scope = SCOPE;
    *max_time = 9;  // max branch, search time

    *pseudo = true;
    if (total_walls < 8)
    {
        return 6;
    }
    else if (total_walls < 16)
    {
        *pseudo = 0;
    }
    
    return 4;
}

small_int findBoardsize13(char* pseudo, float* max_time, float* scope, cint total_walls)
{
    *scope = SCOPE;
    *max_time = 8.5;  // max branch, search time

    if (total_walls < 14)
    {
        *pseudo = true;
        return 4;
    }
    else if (total_walls < 7)
    {
        return 6;
    }
    
    return 3;
}

small_int findBoardsize15(char* pseudo, float* max_time, float* scope, cint total_walls)
{
    *scope = SCOPE;
    *max_time = 8;  // max branch, search time

    if (total_walls < 13)
    {
        *pseudo = true;
        return 4;
    }
    else if (total_walls < 5)
    {
        return 6;
    }

    return 2;
}

small_int findBoardsizeOther(char* pseudo, float* max_time, float* scope)
{
    *scope = SCOPE;
    *max_time = 9.2;  // max branch, search time

    *pseudo = true;
    return 2;
}

/* The depth of the engine is calculated based on 3 parameters. The board size, the number of total moves played and the total number of available walls.
The strength of the engine is basically determined here, increase the depth numbers to make the engine more powerful, abeit slower */
small_int findDepth(cint boardsize, char* pseudo, float* max_time, float* scope, cint total_moves, cint total_walls)
{
    // first few moves do not really require much depth, just search at depth 1
    if (total_moves + 3 < boardsize)
    {
        *scope = SCOPE;
        return 1;
    }
    
    /* pseudodepth is used to simulate a further search to make the depth even.
    If pseudodepth is true the last step will only check opponent's "most likely"
    responses instead of all possible answers. This means it will check pawn movement
    and wall placement near the enemy pawn to disrupt it's movement */
    
    if (boardsize < 6)
    {
        *scope = 1;
        *max_time = 14;

        if (total_walls < 3)
            return 14;
        else if (total_walls < 4)
            return 8;
        
        *pseudo = true;
        return 8;
    }
    
    else if (boardsize < 8)
    {
        *scope = 1.2;

        if (total_walls < 3)
        {
            *scope = 1;
            *max_time = 12;
            return 8;
        }
        else if (total_walls < 7)
        {
            *scope = 1.3;
            *pseudo = true;
            *max_time = 12;
            return 8;
        }
        else if (total_walls < 9)
        {
            *max_time = 12;
            return 6;
        }
        else if (total_walls < 12)
        {
            *max_time = 12;  // max branch, search time
            *pseudo = 1;
            return 6;
        }
        *max_time = 11;
        return 4;
    }
    
    else if (boardsize < 10)
    {
        if (total_walls < 10)
        {
            *scope = 1.2;
            *max_time = 12;  // max branch, search time
            return 8;
        }
        
        *max_time = 10.1;
        *scope = 1.3;
        *pseudo = true;
        return 6;
    }
    
    else if (boardsize < 12)
        return findBoardsize11(pseudo, max_time, scope, total_walls);
    
    else if (boardsize < 14)
        return findBoardsize13(pseudo, max_time, scope, total_walls);
    
    else if (boardsize < 16)
        return findBoardsize15(pseudo, max_time, scope, total_walls);
    
    else
        return findBoardsizeOther(pseudo, max_time, scope);
}