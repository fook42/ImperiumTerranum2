#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void BOXWIN(struct RastPort* RPort, uint16 Left, uint16 Top, uint16 Right, uint16 Bottom)
{
    const WORD BArr[] = {Right, Top,
                         Right, Bottom,
                         Left, Bottom,
                         Left, Top};
    Move(RPort, Left, Top);
    PolyDraw(RPort, (long) 4, BArr);
}
