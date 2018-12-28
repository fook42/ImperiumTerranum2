#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void BOX(struct Screen* XScreen, uint16 Left, uint16 Top, uint16 Right, uint16 Bottom)
{
    const WORD BArr[] = {Right, Top,
                         Right, Bottom,
                         Left, Bottom,
                         Left, Top};
    Move(&(XScreen->RastPort), Left, Top);
    PolyDraw(&(XScreen->RastPort), (long) 4, BArr);
}
