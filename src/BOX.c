#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void BOX(struct RastPort* RPort, WORD Left, WORD Top, WORD Right, WORD Bottom)
{

/*    const WORD BArr[] = {Right, Top,
                         Right, Bottom,
                         Left, Bottom,
                         Left, Top};
    Move(RPort, Left, Top);
    PolyDraw(RPort, (long) 4, BArr);
*/
    Move(RPort, Left,  Top);
    Draw(RPort, Right, Top);
    Draw(RPort, Right, Bottom);
    Draw(RPort, Left,  Bottom);
    Draw(RPort, Left, Top);
}
