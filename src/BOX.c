#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void BOX(struct RastPort* RPort, const int Left, const int Top, const int Right, const int Bottom)
{
    Move(RPort, Left,  Top);
    Draw(RPort, Right, Top);
    Draw(RPort, Right, Bottom);
    Draw(RPort, Left,  Bottom);
    Draw(RPort, Left, Top);
}
