#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void DRAWRECT(uint32 x, uint32 y, r_ShipHeader* MyShipPtr, uint32 color)
{
    SetAPen(MyRPort_PTR[0], color);
    // paint on main-screen
    if ((0 < x) && (481 > x) && (0 < y) && (481 > y))
    {
        BOX(MyRPort_PTR[0], x, y, x+31, y+31);
    }
    // paint on minimap-area
    WritePixel(MyRPort_PTR[0], 575+MyShipPtr->PosX, 62+MyShipPtr->PosY);
}
