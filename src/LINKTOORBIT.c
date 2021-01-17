#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void LINKTOORBIT(r_PlanetHeader* MyPlanetHeader, r_ShipHeader* MyShipPtr, uint8 ActSys)
{
    sint16  x,y;
    if (0 == (MyShipPtr->Ladung & MASK_SIEDLER))
    {
        LINKSHIP(MyShipPtr, &MyPlanetHeader->FirstShip,1);
    }
    if (((SystemFlags[ActPlayer-1][ActSys-1] & FLAG_KNOWN) != 0) || (MyShipPtr->Owner == ActPlayerFlag))
    {
        x = 256+(MyShipPtr->PosX+OffsetX)*32;
        y = 256+(MyShipPtr->PosY+OffsetY)*32;
        if ((x>=0) && (x<=480) && (y>=0) && (y<=480))
        {
            RECT(MyScreen[0],0,x,y,x+31,y+31);
            REFRESHDISPLAY();
            Delay(10);
        }
    }
    MyShipPtr->PosX = 0;
    MyShipPtr->PosY = 0;
}
