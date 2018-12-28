#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void DRAWRECT(uint32 x, uint32 y, r_ShipHeader* MyShipPtr)
{
    // paint on main-screen
    if ((x>=1) && (x<=480) && (y>=1) && (y<=480))
    {
        BOXWIN(MyRPort_PTR[0],x,y,x+31,y+31);
    }
    // paint on minimap-area
    WritePixel(MyRPort_PTR[0],575+MyShipPtr->PosX,62+MyShipPtr->PosY);
}
