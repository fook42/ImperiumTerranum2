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
    while (MyShipPtr != NULL)
    {
        MyShipPtr->Owner = LastShipPtr->Owner;
        MyShipPtr = MyShipPtr->NextShip;
    }
    MyShipPtr->NextShip = LastShipPtr->NextShip;
    if (LastShipPtr->NextShip != NULL)
    {
        LastShipPtr->NextShip->BeforeShip = MyShipPtr;
    }
    LastShipPtr->Owner = 0;
}
