#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void FINDENEMYOBJECT(uint8 ActSys, r_ShipHeader* ShipPtr)
{
    uint8   btx,CivVar,CivVar2;
    r_PlanetHeader* MyPlanetHeader;
    r_ShipHeader*   MyShipPtr;
    r_ShipHeader*   OtherShipPtr;
    uint32  DistOld,DistNew;
    uint8   k;
    ByteArr42*  ActPProjects;

    MyShipPtr = ShipPtr;
    CivVar = GETCIVVAR(MyShipPtr->Owner);
    if (0 == CivVar)
    {
        return;
    }
    btx = 0;
    DistOld = 10000;
    for(k = 0; k < SystemHeader[ActSys-1].Planets; k++)
    {
        MyPlanetHeader = &(SystemHeader[ActSys-1].PlanetMemA[k]);
        ActPProjects = MyPlanetHeader->ProjectPtr;
        CivVar2 = GETCIVVAR(MyPlanetHeader->PFlags);
        if ((0 != CivVar2) && (CivVar2 != CivVar))
        {
            if ((Save.WarState[CivVar-1][CivVar2-1]==LEVEL_WAR) ||
                (Save.WarState[CivVar-1][CivVar2-1]==LEVEL_COLDWAR))
            {
                if (((MyShipPtr->Ladung & MASK_LTRUPPS)>0) || (MyPlanetHeader->FirstShip.NextShip != NULL)
                    || (ActPProjects->data[34]>0) || (ActPProjects->data[40]>0) || (CivVar == 9))
                {
                    DistNew = it_round((double) abs(MyPlanetHeader->PosX-MyShipPtr->PosX));
                    if (it_round((double) abs(MyPlanetHeader->PosY - MyShipPtr->PosY)) > DistNew)
                    {
                        DistNew = it_round((double) abs(MyPlanetHeader->PosY-MyShipPtr->PosY));
                    }
                    if (DistNew <= DistOld)
                    {
                        DistOld = DistNew;
                        if (btx == 0) { btx = k+1; }
                        MyShipPtr->Target = k+1;
                        MyShipPtr->Source = ActSys;
                    }
                }
            }
        }
    }
    if ((btx>0) && (MyShipPtr->Owner == FLAG_MAQUES)) { return; }
    if (SystemHeader[ActSys-1].FirstShip.NextShip != NULL)
    {
        OtherShipPtr = SystemHeader[ActSys-1].FirstShip.NextShip;
        if (MyShipPtr->Owner != FLAG_MAQUES)
        {
            while (OtherShipPtr != NULL)
            {
                CivVar2 = GETCIVVAR(OtherShipPtr->Owner);
                if ((OtherShipPtr->Moving >= 0) && (CivVar2!=CivVar) && (CivVar2!=0))
                {
                    if ((Save.WarState[CivVar-1][CivVar2-1]==LEVEL_WAR) || (Save.WarState[CivVar-1][CivVar2-1]==LEVEL_COLDWAR))
                    {
                        DistNew = abs(OtherShipPtr->PosX - MyShipPtr->PosX);
                        if (abs(OtherShipPtr->PosY - MyShipPtr->PosY) > DistNew)
                        {
                            DistNew = abs(OtherShipPtr->PosY - MyShipPtr->PosY);
                        }
                        if (DistNew < DistOld)
                        {
                            DistOld = DistNew;
                            btx = 1;
                            MyShipPtr->Target = TARGET_ENEMY_SHIP;
                            MyShipPtr->TargetShip = OtherShipPtr;
                        }
                    }
                }
                OtherShipPtr = OtherShipPtr->NextShip;
            }
        }
        if (0 == btx)
        {
            MyShipPtr->Target = 1;
            FINDENEMYSYSTEM(ActSys,CivVar-1,MyShipPtr);
        }
    }
}
