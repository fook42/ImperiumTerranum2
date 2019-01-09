#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void KILLFLEET(r_ShipHeader* MyShipPtr)
{
    r_ShipHeader*   LastShipPtr;

    LastShipPtr = MyShipPtr;
    MyShipPtr->NextShip = MyShipPtr->TargetShip;
    MyShipPtr->TargetShip->BeforeShip = MyShipPtr;
    while (NULL != MyShipPtr)
    {
        MyShipPtr->Owner = LastShipPtr->Owner;
        MyShipPtr = MyShipPtr->NextShip;
    }
    MyShipPtr->NextShip = LastShipPtr->NextShip;
    if (NULL != LastShipPtr->NextShip)
    {
        LastShipPtr->NextShip->BeforeShip = MyShipPtr;
    }
    LastShipPtr->Owner = 0;
}
