#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

bool CHECKSUN(r_ShipHeader* MyShipPtr)
{
    bool _CHECKSUN = false;

    if ((-2 <= MyShipPtr->PosX) && (2 >= MyShipPtr->PosX) &&
        (-2 <= MyShipPtr->PosY) && (2 >= MyShipPtr->PosY))
    {
        if ((-2 > MOVESHIP_FromX) || (2 < MOVESHIP_FromX))
        {
            MyShipPtr->PosX = MOVESHIP_FromX;
        } else if ((-2 > MOVESHIP_FromY) || (2 < MOVESHIP_FromY))
        {
            MyShipPtr->PosY = MOVESHIP_FromY;
        } else {
            MyShipPtr->PosX = MOVESHIP_FromX;
            MyShipPtr->PosY = MOVESHIP_FromY;
        }

        if ((MyShipPtr->PosX == MOVESHIP_FromX) &&
            (MyShipPtr->PosY == MOVESHIP_FromY))
        {
            _CHECKSUN = true;
            MyShipPtr->Moving = 0;
        }
    }
    return _CHECKSUN;
}
