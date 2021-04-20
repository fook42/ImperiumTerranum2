#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void KILLFLEET(r_ShipHeader* MyShipPtr)
{
    r_ShipHeader*   LastShipPtr;

/* goal of this:
- mark all ships in the fleet with "Owner=0"
- link the target-ship-chain as "nextShip" and move the old nextShip at the end of this
    * needed for cleanup by refreshships-routine

before:    [BS] -> [FSH] -next-> [LS] -...
                      \-target-> [FS1] -next-> [FS2] -...[FSx] -next-> NULL

after:     [BS] -> [FSH] -next-> [FS1] -next-> [FS2] -...[FSx] -next-> [LS] -...

*/

    LastShipPtr = MyShipPtr->NextShip;

    MyShipPtr->NextShip = MyShipPtr->TargetShip;
    MyShipPtr->TargetShip->BeforeShip = MyShipPtr;

    // go to the last ship in the chain... and mark all ships with Owner=0
    while (NULL != MyShipPtr->NextShip)
    {
        MyShipPtr->Owner = 0;
        MyShipPtr = MyShipPtr->NextShip;
    }

    // link the old "nextShip" to the last spot of the chain
    MyShipPtr->NextShip = LastShipPtr;
    LastShipPtr->BeforeShip = MyShipPtr;
}
