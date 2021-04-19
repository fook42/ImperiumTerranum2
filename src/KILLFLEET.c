#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void KILLFLEET(r_ShipHeader* MyShipPtr)
{
    r_ShipHeader*   LastShipPtr;

    LastShipPtr = MyShipPtr;

/* -- old, faulty code -- directly adopted from original pascal code --
    MyShipPtr->NextShip = MyShipPtr->TargetShip;
    MyShipPtr->TargetShip->BeforeShip = MyShipPtr;

    while (NULL != MyShipPtr->NextShip)
    {
        MyShipPtr = MyShipPtr->NextShip;
        MyShipPtr->Owner = LastShipPtr->Owner;
    }
// ... @TODO .. this will create a circle between "first ship" and last ship of the fleet 
    MyShipPtr->NextShip = LastShipPtr->NextShip;
    if (NULL != LastShipPtr->NextShip)
    {
        LastShipPtr->NextShip->BeforeShip = MyShipPtr;
    }
*/

    LastShipPtr->Owner = 0;
}
