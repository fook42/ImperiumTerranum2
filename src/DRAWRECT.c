#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void DRAWRECT(const int x, const int y, const r_ShipHeader* MyShipPtr, const int color)
{
    SetAPen(MyRPort_PTR[0], color);
    // paint on main-screen
    if ((0 < x) && ((Area_Width-31) > x) && (0 < y) && ((Area_Height-31) > y))
    {
        Move(MyRPort_PTR[0], x,    y);
        Draw(MyRPort_PTR[0], x+31, y);
        Draw(MyRPort_PTR[0], x+31, y+31);
        Draw(MyRPort_PTR[0], x,    y+31);
        Draw(MyRPort_PTR[0], x,    y);
    }
    // paint on minimap-area
    WritePixel(MyRPort_PTR[0], (HighRes_Width-65)+MyShipPtr->PosX, 62+MyShipPtr->PosY);
}
