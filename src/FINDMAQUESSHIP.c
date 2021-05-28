#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

bool FINDMAQUESSHIP(uint8 ActSys, r_ShipHeader* MyShipPtr)
{
    uint8   CivVar, CivVar2;
    r_ShipHeader*   OtherShipPtr;
    uint16  DistOld, DistNew;
    bool    _FINDMAQUESSHIP = false;

    CivVar = GETCIVVAR(MyShipPtr->Owner);
    if ((CIVVAR_NONE != CivVar) && (CIVVAR_MAQUES != CivVar))
    {
        DistOld = 10000;
        OtherShipPtr = SystemHeader[ActSys].FirstShip.NextShip;
        while (NULL != OtherShipPtr)
        {
            CivVar2 = GETCIVVAR(OtherShipPtr->Owner);
            if ((0 <= OtherShipPtr->Moving) && (CIVVAR_MAQUES == CivVar2))
            {
                DistNew = abs(OtherShipPtr->PosX - MyShipPtr->PosX);
                if (abs(OtherShipPtr->PosY - MyShipPtr->PosY) > DistNew)
                {
                    DistNew = abs(OtherShipPtr->PosY-MyShipPtr->PosY);
                }
                if (DistNew < DistOld)
                {
                    DistOld = DistNew;
                    _FINDMAQUESSHIP = true;
                    MyShipPtr->Target = TARGET_ENEMY_SHIP;
                    MyShipPtr->TargetShip = OtherShipPtr;
                }
            }
            OtherShipPtr = OtherShipPtr->NextShip;
        }
    }
    return _FINDMAQUESSHIP;
}
