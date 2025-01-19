#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

uint8 GOTONEXTPLANET(uint8 ActSys, r_ShipHeader* MyShipPtr)
{
    uint8           k, btx;
//    uint8           CivVar,CivFlag;
    r_PlanetHeader* MyPlanetHeader;
    r_ShipHeader*   OtherShipPtr;
    int             DistOld,DistNew;

    if (FINDMAQUESSHIP(ActSys,MyShipPtr)) { return ActSys; }

// TODO: not used???
//    CivVar = GETCIVVAR(MyShipPtr->Owner);
//    CivFlag = MyShipPtr->Owner & FLAG_CIV_MASK;
    btx = 0;
    DistOld = 10000;
    for(k = 0; k < SystemHeader[ActSys].Planets; k++)
    {
        MyPlanetHeader = &(SystemHeader[ActSys].PlanetMemA[k]);
        OtherShipPtr = MyPlanetHeader->FirstShip.NextShip;
        while ((NULL != OtherShipPtr) && (0 == OtherShipPtr->Owner))
        {
            OtherShipPtr = OtherShipPtr->NextShip;
        }

        if ((0 != ClassLifeFactor[MyPlanetHeader->Class])
          && (0 == MyPlanetHeader->PFlags) && (NULL == OtherShipPtr))
        {
            DistNew = it_round(abs(MyPlanetHeader->PosX - MyShipPtr->PosX));
            if (it_round(abs(MyPlanetHeader->PosY - MyShipPtr->PosY))>DistNew)
            {
                DistNew = it_round(abs(MyPlanetHeader->PosY-MyShipPtr->PosY));
            }
            if (DistNew<DistOld)
            {
                DistOld = DistNew;
                btx = k+1;
                MyShipPtr->Target = btx;
                MyShipPtr->Source = ActSys+1;
            }
        }
    }
    if (0 != btx)
    {
        LINKSHIP(MyShipPtr, &SystemHeader[ActSys].FirstShip,1);
    } else {
        SystemHeader[ActSys].State |= STATE_ALL_OCC;
        return GOTONEXTSYSTEM(ActSys,MyShipPtr);
    }
    return ActSys;
}

