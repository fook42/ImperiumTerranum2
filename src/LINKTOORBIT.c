#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void LINKTOORBIT(r_PlanetHeader* MyPlanetHeader, r_ShipHeader* MyShipPtr, const int ActSys)
{
    sint16  x,y;
    if (0 == (MyShipPtr->Ladung & MASK_SIEDLER))
    {
        LINKSHIP(MyShipPtr, &MyPlanetHeader->FirstShip,1);
    }
    if (((SystemFlags[ActPlayer-1][ActSys] & FLAG_KNOWN) != 0) || (MyShipPtr->Owner == ActPlayerFlag))
    {
        x = Area_CenterX+(MyShipPtr->PosX+OffsetX)*32;
        y = Area_CenterY+(MyShipPtr->PosY+OffsetY)*32;
        if ((x>=0) && (x<=(Area_Width-32)) && (y>=0) && (y<=(Area_Height-32)))
        {
            RECT_RP0_C0(x,y,x+31,y+31);
            REFRESHDISPLAY();
            Delay(10);
        }
    }
    MyShipPtr->PosX = 0;
    MyShipPtr->PosY = 0;
}
