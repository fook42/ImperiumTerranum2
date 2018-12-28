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
    if ((CivVar!=0) && (CivVar!=9))
    {
        DistOld = 10000;
//        if (SystemHeader[ActSys].FirstShip.NextShip != NULL)
//        {
            OtherShipPtr = SystemHeader[ActSys].FirstShip.NextShip;
            while (OtherShipPtr != NULL)
            {
                CivVar2 = GETCIVVAR(OtherShipPtr->Owner);
                if ((OtherShipPtr->Moving >= 0) && (CivVar2 == 9))
                {
                    DistNew = abs(OtherShipPtr->PosX - MyShipPtr->PosX);
                    if (abs(OtherShipPtr->PosY - MyShipPtr->PosY)>DistNew)
                    {
                        DistNew = abs(OtherShipPtr->PosY-MyShipPtr->PosY);
                    }
                    if (DistNew<DistOld)
                    {
                        DistOld = DistNew;
                        _FINDMAQUESSHIP = true;
                        MyShipPtr->Target = TARGET_ENEMY_SHIP;
                        MyShipPtr->TargetShip = OtherShipPtr;
                    }
                }
                OtherShipPtr = OtherShipPtr->NextShip;
            }
//        }
    }
    return _FINDMAQUESSHIP;
}
