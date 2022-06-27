#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void MOVESHIP_DRAWSCENE(uint8 ActSys, r_ShipHeader* MyShipPtr)
{
    if (ActSys != Display)
    {
        OffsetX = -MyShipPtr->PosX-1;
        OffsetY = -MyShipPtr->PosY-1;
        MOVESHIP_x = Area_CenterX+(MyShipPtr->PosX+OffsetX)*32;
        MOVESHIP_y = Area_CenterY+(MyShipPtr->PosY+OffsetY)*32;
        DRAWSYSTEM(MODE_REDRAW,ActSys,NULL);
    } else {
        REFRESHDISPLAY();
    }
    Delay(10);
}
