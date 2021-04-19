#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void FASTREFRESH(sint16 OldX, sint16 OldY, r_ShipHeader* XCludeShip)
{
    int     x,y,i;
    int     locDisplay = Display-1;
    r_ShipHeader*   OtherShipPtr;
    r_PlanetHeader* PlanetHeader;
    r_ShipHeader*   UseShipPtr;
    
    OtherShipPtr = &SystemHeader[locDisplay].FirstShip;
    while (NULL != OtherShipPtr)
    {
        if ((OtherShipPtr->PosX > (OldX-2)) && (OtherShipPtr->PosX < (OldX+2))
         && (OtherShipPtr->PosY > (OldY-2)) && (OtherShipPtr->PosY < (OldY+2)))
        {
            if (((XCludeShip != OtherShipPtr) && (0 < OtherShipPtr->Owner) && (0 <= OtherShipPtr->Moving))
             || (TARGET_STARGATE == OtherShipPtr->SType))
            {
                x = 256+((OtherShipPtr->PosX + OffsetX)*32);
                y = 256+((OtherShipPtr->PosY + OffsetY)*32);
                if (SHIPTYPE_FLEET == OtherShipPtr->SType)
                {
                    UseShipPtr = OtherShipPtr->TargetShip;
                } else {
                    UseShipPtr = OtherShipPtr;
                }
                if ((0 <= x) && (481 > x) && (0 <= y) && (481 > y))
                {
                    BltBitMapRastPort((struct BitMap*) &ImgBitMap4,(UseShipPtr->SType-8)*32+1,33,MyRPort_PTR[0],x+1,y+1,30,30,192);
                }
            }
        }
        OtherShipPtr = OtherShipPtr->NextShip;
    }

    for(i = 0; i < SystemHeader[locDisplay].Planets; ++i)
    {
        PlanetHeader = &(SystemHeader[locDisplay].PlanetMemA[i]);
        if ((it_round(PlanetHeader->PosX) > (OldX-2)) && (it_round(PlanetHeader->PosX) < (OldX+2))
          &&(it_round(PlanetHeader->PosY) > (OldY-2)) && (it_round(PlanetHeader->PosY) < (OldY+2)))
        {
            x = 256+((it_round(PlanetHeader->PosX) + OffsetX)*32);
            y = 256+((it_round(PlanetHeader->PosY) + OffsetY)*32);
            if ((x>=0) && (x<481) && (y>=0) && (y<481))
            {
                BltBitMapRastPort((struct BitMap*) &ImgBitMap7,PlanetHeader->Class*32,0,MyRPort_PTR[0],x,y,32,32,192);
            }
        }
    }
}

void DRAWMOVINGSHIP(r_ShipHeader* MyShipPtr, uint8 ActSys)
{
    bool b;
    uint8   Step;
    r_ShipHeader*   UseShipPtr;

    if (0 == Display)
    {
        DRAWSYSTEM(MODE_REDRAW,ActSys,NULL);
    }
    if ((MyShipPtr->PosX == MOVESHIP_FromX) && (MyShipPtr->PosY == MOVESHIP_FromY))
    {
        return;
    }
    b = true;
    if (Save.FastMove)
    {
        Step = 4;
    } else {
        Step = 2;
    }
    do
    {
        if ((MOVESHIP_x>=0) && (MOVESHIP_x<481) && (MOVESHIP_y>=0) && (MOVESHIP_y<481))
        {
            FASTREFRESH(MOVESHIP_FromX,MOVESHIP_FromY,MyShipPtr);
            RECT_RP0_C0(MOVESHIP_x,MOVESHIP_y,MOVESHIP_x+31,MOVESHIP_y+31);
        }
        if (MOVESHIP_ToX < MOVESHIP_x)
        {
            MOVESHIP_x -= Step;
        } else if (MOVESHIP_ToX > MOVESHIP_x)
        {
            MOVESHIP_x += Step;
        }
        if (MOVESHIP_ToY < MOVESHIP_y)
        {
            MOVESHIP_y -= Step;
        } else if (MOVESHIP_ToY > MOVESHIP_y)
        {
            MOVESHIP_y += Step;
        }
        if (SHIPTYPE_FLEET == MyShipPtr->SType)
        {
            UseShipPtr = MyShipPtr->TargetShip;
        } else {
            UseShipPtr = MyShipPtr;
        }
        if ((MOVESHIP_x>=0) && (MOVESHIP_x<481) && (MOVESHIP_y>=0) && (MOVESHIP_y<481))
        {
            BltBitMapRastPort((struct BitMap*) &ImgBitMap4,(UseShipPtr->SType-8)*32,32,MyRPort_PTR[0],MOVESHIP_x,MOVESHIP_y,31,31,192);
            DRAWRECT(MOVESHIP_x,MOVESHIP_y, MyShipPtr, UseShipPtr->Owner);
            if (FLAG_OTHER != MyShipPtr->Owner)
            {
                WaitBlit();
                WaitBlit();
            }
            WaitBlit();
            WaitTOF();
        } else {
            b = false;
        }
    }
    while ((MOVESHIP_x != MOVESHIP_ToX) || (MOVESHIP_y != MOVESHIP_ToY));
    if (((MOVESHIP_x<1) || (MOVESHIP_x>478) || (MOVESHIP_y<1) || (MOVESHIP_y>478)) && (MyShipPtr->Moving>0) && (b))
    {
        OffsetX = -MyShipPtr->PosX-1;
        OffsetY = -MyShipPtr->PosY-1;
        MOVESHIP_x = 256+((MyShipPtr->PosX+OffsetX)*32);
        MOVESHIP_y = 256+((MyShipPtr->PosY+OffsetY)*32);
        MOVESHIP_ToX = MOVESHIP_x;
        MOVESHIP_ToY = MOVESHIP_y;
        DRAWSYSTEM(MODE_REDRAW,Display,NULL);
    } else {
        REFRESHDISPLAY();
    }
}
