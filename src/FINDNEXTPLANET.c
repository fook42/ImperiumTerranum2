#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

uint8 FINDNEXTPLANET(uint8 ActSys, r_ShipHeader* ShipPtr)
{
    uint8   btx,CivVar,CivFlag;
    r_PlanetHeader* MyPlanetHeader;
    r_ShipHeader*   MyShipPtr;
    r_ShipHeader*   OtherShipPtr;
    uint8   k;
    uint32  DistOld,DistNew,WPerc,WPercLow;


    if (FINDMAQUESSHIP(ActSys,ShipPtr)) { return ActSys; }
    MyShipPtr = ShipPtr;
    CivVar = GETCIVVAR(MyShipPtr->Owner);
    CivFlag = MyShipPtr->Owner & FLAG_CIV_MASK;
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
        if (0 == MyPlanetHeader->Size) { MyPlanetHeader->Size = 1; }
        WPerc    = (uint32)  (MyPlanetHeader->Water    / MyPlanetHeader->Size);
        WPercLow = (uint32) ((MyPlanetHeader->Water-5) / MyPlanetHeader->Size);

        if (
           (((MyShipPtr->Ladung & MASK_SIEDLER )> 0) &&
            (0 != ClassLifeFactor[MyPlanetHeader->Class]) &&
            (MyPlanetHeader->PFlags == 0) &&
            (OtherShipPtr == NULL)
           )

         || ((0 == MyShipPtr->Ladung) &&
             (0 == MyShipPtr->Fracht) &&
             (56 < WPercLow) &&
             (0 != ClassTransFactor[MyPlanetHeader->Class]) &&
             (((MyPlanetHeader->PFlags & FLAG_CIV_MASK) == CivFlag) ||
              ((MyPlanetHeader->PFlags & FLAG_CIV_MASK) == 0) ||
              (Save.WarState[CivVar-1][GETCIVVAR(MyPlanetHeader->PFlags)-1] == LEVEL_WAR)
             )
            )

         || ((0 < MyShipPtr->Fracht) &&
             (55 > WPerc) &&
             (0 != ClassTransFactor[MyPlanetHeader->Class]) &&
             ((MyPlanetHeader->PFlags & FLAG_CIV_MASK) == CivFlag)
            )
           )
        {
            DistNew = it_round(abs(MyPlanetHeader->PosX - MyShipPtr->PosX));
            if (it_round(abs(MyPlanetHeader->PosY - MyShipPtr->PosY)) > DistNew)
            {
                DistNew = it_round(abs(MyPlanetHeader->PosY - MyShipPtr->PosY));
            }
            if (DistNew < DistOld)
            {
                DistOld = DistNew;
                if (btx == 0) { btx = k+1; }
                MyShipPtr->Target = k+1;
                MyShipPtr->Source = 0;
                MyShipPtr->TargetShip = NULL;
            }
        }
    }
    if (btx <= 0)
    {
        SystemHeader[ActSys].State |= STATE_ALL_OCC;
        return GOTONEXTSYSTEM(ActSys, MyShipPtr);
    } else {
        SystemHeader[ActSys].State &= (~STATE_ALL_OCC);
    }
    return ActSys;
}


