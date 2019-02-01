#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void FINDENEMYSYSTEM(uint8 ActSys, uint8 CivVar, r_ShipHeader* ShipPtr)
{
    sint32  l, SysEntfernung;
    uint8   i, j;
    uint8   SysID;
    r_ShipHeader*   MyShipPtr;

    MyShipPtr = ShipPtr;
    SysEntfernung = 255;
    SysID = 0;
    for(i = 1; i <= Save.Systems; i++)
    {
        if (i != ActSys)
        {
            l = abs(SystemX[ActSys-1]-SystemX[i-1]);
            if (abs(SystemY[ActSys-1]-SystemY[i-1]) > l)
            {
                l = abs(SystemY[ActSys-1]-SystemY[i-1]);
            }
            if ((((SystemFlags[ActPlayer-1][i-1] & FLAG_CIV_MASK) != 0)
                && ((SystemFlags[0][i-1] & FLAG_CIV_MASK) != 0))
             || (MyShipPtr->Flags == SHIPFLAG_WATER))
            {
                if ((l<SysEntfernung) && (MyShipPtr->Flags == SHIPFLAG_WATER)
                    && (0 == (rand()%3)))
                {
                    SysEntfernung = l;
                    SysID = i;
                    if ((SystemHeader[ActSys-1].FirstShip.SType == TARGET_STARGATE)
                     && (SystemHeader[ SysID-1].FirstShip.SType == TARGET_STARGATE))
                    {
                        SysEntfernung = 255;
                    }
                } else if ((l<SysEntfernung) && ((SystemFlags[0][i-1] & FLAG_CIV_MASK) != 0)
                        && ((Save.WarState[CivVar-1][GETCIVVAR(SystemFlags[0][i-1])-1]==LEVEL_WAR)
                          ||(Save.WarState[CivVar-1][GETCIVVAR(SystemFlags[0][i-1])-1]==LEVEL_COLDWAR)))
                {
                    SysEntfernung = l;
                    SysID = i;
                }
            }
        }
    }
    if (SysID == 0)
    {
        SysID = (rand()%Save.Systems)+1;
    }

    if (SystemHeader[SysID-1].Planets == 0)
    {   CREATENEWSYSTEM(SysID-1,CivVar); }

    if ((SystemHeader[ActSys-1].FirstShip.SType == TARGET_STARGATE)
     && (SystemHeader[ SysID-1].FirstShip.SType == TARGET_STARGATE))
    {
        MyShipPtr->Target = TARGET_ENEMY_SHIP;
        MyShipPtr->TargetShip = &SystemHeader[ActSys-1].FirstShip;
        MyShipPtr->Source = SysID;
        return;
    }
    for(i = 0; i < MAXHOLES; i++)
    {
        if (MyWormHole[i].CivKnowledge[CivVar-1] == FLAG_KNOWN)
        {
            for(j = 0; j < 2; j++)
            {
                if ((MyWormHole[i].System[j] == ActSys) && ((SystemFlags[0][MyWormHole[i].System[1-j]-1] & FLAG_CIV_MASK) != 0))
                {
                    if ((Save.WarState[CivVar-1][GETCIVVAR(SystemFlags[0][MyWormHole[i].System[1-j]-1])-1]==LEVEL_WAR)
                      ||(Save.WarState[CivVar-1][GETCIVVAR(SystemFlags[0][MyWormHole[i].System[1-j]-1])-1]==LEVEL_COLDWAR))
                    {
                        SysID = MyWormHole[i].System[1-j];
                        /* *** Feindliche Systeme mit Wurmloch bevorzugt angreifen *** */
                    }
                }
            }
        }
    }
    for(i = 0; i < MAXHOLES; i++)
    {
        if (MyWormHole[i].CivKnowledge[CivVar-1] == FLAG_KNOWN)
        {
            for(j = 0; j < 2; j++)
            {
                if ((MyWormHole[i].System[j] == ActSys) && (MyWormHole[i].System[1-j] == SysID))
                {
                    MyShipPtr->Target = -(i+1);
                    MyShipPtr->Source = -(j+1);
                    return;
                }
            }
        }
    }

    MyShipPtr->Source = ActSys;
    MyShipPtr->Target = SysID;
    l = -(SysEntfernung / ShipData(MyShipPtr->SType).MaxMove)-1;
    if (l<-127)
    {   l = -127; }
    MyShipPtr->Moving = l;
    LINKSHIP(MyShipPtr, &SystemHeader[SysID-1].FirstShip, 1);
}
