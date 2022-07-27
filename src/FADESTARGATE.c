#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void FADESTARGATE(uint8 SysID, uint8 ActSys, r_ShipHeader* MyShipPtr)
{
    r_ShipHeader*   UseShipPtr;
    uint16  i;
    uint8   Color,Cx,Cy;

    if (MyShipPtr->SType == SHIPTYPE_FLEET)
    {
        UseShipPtr = MyShipPtr->TargetShip;
    } else {
        UseShipPtr = MyShipPtr;
    }

    if ((((SystemFlags[ActPlayer-1][ActSys-1] & FLAG_KNOWN) == FLAG_KNOWN) && (Save.CivPlayer[ActPlayer-1] != 0))
     || (Save.CivPlayer[GETCIVVAR(MyShipPtr->Owner)-1] != 0))
    {
        if ((SystemFlags[ActPlayer-1][ActSys-1] & FLAG_KNOWN) != 0)
        {
            DRAWMOVINGSHIP(MyShipPtr, ActSys);
        }
        PLAYSOUND(2,250);
        if ((MOVESHIP_ToX>=0) && (MOVESHIP_ToX<=478) && (MOVESHIP_ToY>=0) && (MOVESHIP_ToY<=478))
        {
            for(i = 1; i <= 1800; i++)
            {
                Cx = (rand()%29)+1;
                Cy = (rand()%29)+1;
                BltBitMapRastPort((struct BitMap*) &ImgBitMap4,576+Cx,32+Cy,&(MyScreen[0]->RastPort),MOVESHIP_ToX+Cx,MOVESHIP_ToY+Cy,1,1,192);
                Color = ReadPixel(&(MyScreen[0]->RastPort),MOVESHIP_ToX+Cx,MOVESHIP_ToY+Cy);
                SetAPen(&(MyScreen[0]->RastPort),Color);
                RectFill(&(MyScreen[0]->RastPort), MOVESHIP_ToX+Cx, MOVESHIP_ToY+Cy, MOVESHIP_ToX+Cx+1, MOVESHIP_ToY+Cy+1);
            }
        }
    }
    LINKSHIP(MyShipPtr, &SystemHeader[SysID-1].FirstShip, 1);
    Delay(5);
    REFRESHDISPLAY();
    Delay(5);
    MyShipPtr->PosX = SystemHeader[SysID-1].FirstShip.PosX;
    MyShipPtr->PosY = SystemHeader[SysID-1].FirstShip.PosY;
    OffsetX = -MyShipPtr->PosX-1;
    OffsetY = -MyShipPtr->PosY-1;
    MOVESHIP_x = 256+(MyShipPtr->PosX+OffsetX)*32;
    MOVESHIP_y = 256+(MyShipPtr->PosY+OffsetY)*32;
    if ((((SystemFlags[ActPlayer-1][ActSys-1] & FLAG_KNOWN) == FLAG_KNOWN) && (Save.CivPlayer[ActPlayer-1] != 0))
     || (Save.CivPlayer[GETCIVVAR(MyShipPtr->Owner)-1] != 0))
    {
        if ((MOVESHIP_x>=0) && (MOVESHIP_x<=478) && (MOVESHIP_y>=0) && (MOVESHIP_y<=478))
        {
            MyShipPtr->Moving = -MyShipPtr->Moving;
            if (MyShipPtr->Moving == 0)
            {
                MyShipPtr->Moving = -1;
            }
            DRAWSYSTEM(MODE_REDRAW,SysID,NULL);
            Delay(5);
            if ((MOVESHIP_x>=0) && (MOVESHIP_x<=478) && (MOVESHIP_y>=0) && (MOVESHIP_y<=478))
            {
                for(i = 1; i <= 1800; i++)
                {
                    Cx = (rand()%29)+1;
                    Cy = (rand()%29)+1;
                    BltBitMapRastPort((struct BitMap*) &ImgBitMap4,(UseShipPtr->SType-8)*32+Cx,32+Cy,&(MyScreen[0]->RastPort),MOVESHIP_x+Cx,MOVESHIP_y+Cy,2,1,192);
                    Color = ReadPixel(&(MyScreen[0]->RastPort),MOVESHIP_x+Cx,MOVESHIP_y+Cy);
                    SetAPen(&(MyScreen[0]->RastPort),Color);
                    RectFill(&(MyScreen[0]->RastPort),MOVESHIP_x+Cx,MOVESHIP_y+Cy,MOVESHIP_x+Cx+1,MOVESHIP_y+Cy+1);
                }
            }
            Delay(5);
            MyShipPtr->Moving = -MyShipPtr->Moving;
            DRAWSYSTEM(MODE_REFRESH,SysID,NULL);
            Delay(5);
        }
    }
    MOVESHIP_FromX = MyShipPtr->PosX;
    MOVESHIP_FromY = MyShipPtr->PosY;
    do
    {
        switch (rand()%4) {
            case 0: MyShipPtr->PosX++; break;
            case 1: MyShipPtr->PosY++; break;
            case 2: MyShipPtr->PosX--; break;
            case 3: MyShipPtr->PosY--; break;
            default: { }
        }
    }
    while(FINDOBJECT(SysID-1,(MyShipPtr->PosX+OffsetX)*32,(MyShipPtr->PosY+OffsetY)*32,MyShipPtr));
    MOVESHIP_ToX = 256+(MyShipPtr->PosX+OffsetX)*32;
    MOVESHIP_ToY = 256+(MyShipPtr->PosY+OffsetY)*32;
    if ((((SystemFlags[ActPlayer-1][ActSys-1] & FLAG_KNOWN) == FLAG_KNOWN) && (Save.CivPlayer[ActPlayer-1] != 0))
     || (Save.CivPlayer[GETCIVVAR(MyShipPtr->Owner)-1] != 0))
    {
         DRAWMOVINGSHIP(MyShipPtr, ActSys);
    }
    if (MyShipPtr->Flags == SHIPFLAG_WATER)
    {
        SysID = FINDNEXTPLANET(SysID,MyShipPtr);
    }
    MyShipPtr->Moving = MyShipPtr->Moving-1;
    if (MyShipPtr->Owner == FLAG_OTHER)
    {
        MyShipPtr->Moving = 0;
    }
    AUTOSHIPTRAVEL(SysID,MODE_SHIPS,MyShipPtr);
    if (MyShipPtr->Owner == 0) { return; }
    if ((((SystemFlags[ActPlayer-1][ActSys-1] & FLAG_KNOWN) == FLAG_KNOWN) && (Save.CivPlayer[ActPlayer-1] != 0))
     || (Save.CivPlayer[GETCIVVAR(MyShipPtr->Owner)-1] != 0))
    {
        DRAWSYSTEM(MODE_REDRAW,ActSys,NULL);
    }
}
