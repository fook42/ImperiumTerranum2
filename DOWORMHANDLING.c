#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void DOWORMHANDLING(r_ShipHeader* MyShipPtr, uint8 ActSys, bool Visible)
{
    uint8   j,WormID,PosID,NewSys=1;
    int     i;

    for(i = 0; i < MAXHOLES; ++i)
    {
        for(j = 0; j < 2; ++j)
        {
            if ((MyWormHole[i].System[j] == ActSys)
                && (MyWormHole[i].PosX[j] == MyShipPtr->PosX)
                && (MyWormHole[i].PosY[j] == MyShipPtr->PosY))
            {
                PosID = 1-j;
                WormID = i;
                NewSys = MyWormHole[i].System[1-j];
            }
        }
    }

    MyShipPtr->Source = 0;
    MyShipPtr->Target = 0;
    if (Visible)
    {
        DRAWMOVINGSHIP(MyShipPtr, ActSys);
        if ((MOVESHIP_ToX<0) || (MOVESHIP_ToX>478) || (MOVESHIP_ToY<0) || (MOVESHIP_ToY>478))
        {
            OffsetX = -MyShipPtr->PosX-1;
            OffsetY = -MyShipPtr->PosY-1;
            MOVESHIP_ToX = 256+((MyShipPtr->PosX+OffsetX)*32);
            MOVESHIP_ToY = 256+((MyShipPtr->PosY+OffsetY)*32);
            DRAWSYSTEM(MODE_REDRAW,ActSys,NULL);
        }
        PLAYSOUND(3,280);
        for (i = 15; i>=0; --i)
        {
            BltBitMapRastPort((struct BitMap*) &ImgBitMap7,i*32,32,MyRPort_PTR[0],MOVESHIP_ToX,MOVESHIP_ToY,32,32,192);
            WaitTOF();
            delay(4);
        }
    }
    NewSys--; // to shift the arrays
    if (SystemHeader[NewSys].Planets == 0)
    {
        CREATENEWSYSTEM(NewSys,ActPlayer);
    }
    SystemFlags[ActPlayer-1][NewSys] |= FLAG_KNOWN;
    LINKSHIP(MyShipPtr, &SystemHeader[NewSys].FirstShip, 1);
    MyShipPtr->PosX = MyWormHole[WormID].PosX[PosID];
    MyShipPtr->PosY = MyWormHole[WormID].PosY[PosID];
    do
    {
        MyShipPtr->PosX = MyShipPtr->PosX-1+(rand()%3);
        MyShipPtr->PosY = MyShipPtr->PosY-1+(rand()%3);
    }
    while ((FINDOBJECT(NewSys,256+(MyShipPtr->PosX+OffsetX)*32,256+(MyShipPtr->PosY+OffsetY)*32,MyShipPtr))
        && ((MyShipPtr->PosX<-3) || (MyShipPtr->PosX>3)) && ((MyShipPtr->PosY<-3) || (MyShipPtr->PosY>3)));
    if (Visible)
    {
        for(i = 0; i < 16; ++i)
        {
            BltBitMapRastPort((struct BitMap*) &ImgBitMap7,i*32,32,MyRPort_PTR[0],MOVESHIP_ToX,MOVESHIP_ToY,32,32,192);
            WaitTOF();
            delay(4);
        }
        RECTWIN(MyRPort_PTR[0], 0, MOVESHIP_ToX, MOVESHIP_ToY, MOVESHIP_ToX+31, MOVESHIP_ToY+31);
        REFRESHDISPLAY();
    }
    --(MyShipPtr->Moving);
    AUTOSHIPTRAVEL(NewSys+1,MODE_SHIPS,MyShipPtr);
    if (Visible)
    {
        DRAWSYSTEM(MODE_REDRAW,ActSys,NULL);
    }
    if (0 == MyShipPtr->Moving)
    {
        MyShipPtr->Moving = 1;
    }
}
