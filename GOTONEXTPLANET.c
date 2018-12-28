#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

uint8 GOTONEXTPLANET(uint8 ActSys, r_ShipHeader* MyShipPtr)
{
    uint8           k,btx;
//    uint8           CivVar,CivFlag;
    r_PlanetHeader* MyPlanetHeader;
    r_ShipHeader*   OtherShipPtr;
    int             DistOld,DistNew;

    if (FINDMAQUESSHIP(ActSys-1,MyShipPtr)) { return ActSys; }

// TODO: not used???
//    CivVar = GETCIVVAR(MyShipPtr->Owner);
//    CivFlag = MyShipPtr->Owner & FLAG_CIV_MASK;
    btx = 0;
    DistOld = 10000;
    for(k = 0; k < SystemHeader[ActSys-1].Planets; k++)
    {
        MyPlanetHeader = &(SystemHeader[ActSys-1].PlanetMemA[k]);
        OtherShipPtr = MyPlanetHeader->FirstShip.NextShip;
        while ((OtherShipPtr != NULL) && (OtherShipPtr->Owner == 0))
        {
            OtherShipPtr = OtherShipPtr->NextShip;
        }
        if (((MyPlanetHeader->Class == CLASS_DESERT) || (MyPlanetHeader->Class == CLASS_HALFEARTH)
           ||(MyPlanetHeader->Class == CLASS_EARTH)  || (MyPlanetHeader->Class == CLASS_ICE)
           ||(MyPlanetHeader->Class == CLASS_STONES) || (MyPlanetHeader->Class == CLASS_WATER))
          && (MyPlanetHeader->PFlags == 0) && (OtherShipPtr == NULL))
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
                MyShipPtr->Target = k+1;
                MyShipPtr->Source = ActSys;
            }
        }
    }
    if (btx != 0 )
    {
        LINKSHIP(MyShipPtr, &SystemHeader[ActSys-1].FirstShip,1);
    } else {
        SystemHeader[ActSys-1].State |= STATE_ALL_OCC;
        return GOTONEXTSYSTEM(ActSys,MyShipPtr);
    }
    return ActSys;
}

