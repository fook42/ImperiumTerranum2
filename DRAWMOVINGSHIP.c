#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void FASTREFRESH(sint16 OldX, sint16 OldY, r_ShipHeader* XCludeShip)
{
    int     x,y,i;
    r_ShipHeader*   OtherShipPtr;
    r_PlanetHeader* PlanetHeader;
    r_ShipHeader*   UseShipPtr;
    
    OtherShipPtr = &SystemHeader[Display-1].FirstShip;
    do
    {
        if ((OtherShipPtr->PosX>=(OldX-1)) && (OtherShipPtr->PosX<=(OldX+1))
         && (OtherShipPtr->PosY>=(OldY-1)) && (OtherShipPtr->PosY<=(OldY+1)))
        {
            if (((OtherShipPtr != XCludeShip) && (OtherShipPtr->Owner>0) && (OtherShipPtr->Moving >= 0))
             || (OtherShipPtr->SType == TARGET_STARGATE))
            {
                x = 256+((OtherShipPtr->PosX+OffsetX)*32);
                y = 256+((OtherShipPtr->PosY+OffsetY)*32);
                if (OtherShipPtr->SType == SHIPTYPE_FLEET)
                {
                    UseShipPtr = OtherShipPtr->TargetShip;
                } else {
                    UseShipPtr = OtherShipPtr;
                }
                if ((x>=0) && (x<=480) && (y>=0) && (y<=480))
                {
                    BltBitMapRastPort((struct BitMap*) &ImgBitMap4,(UseShipPtr->SType-8)*32+1,33,&(MyScreen[0]->RastPort),x+1,y+1,30,30,192);
                }
            }
        }
        OtherShipPtr = OtherShipPtr->NextShip;
    }
    while (OtherShipPtr != NULL);

    for(i = 0; i < SystemHeader[Display-1].Planets; i++)
    {
        PlanetHeader = &(SystemHeader[Display-1].PlanetMemA[i]);
        if ((it_round(PlanetHeader->PosX)>= (OldX-1)) && (it_round(PlanetHeader->PosX)<=(OldX+1))
          &&(it_round(PlanetHeader->PosY)>= (OldY-1)) && (it_round(PlanetHeader->PosY)<=(OldY+1)))
        {
            x = 256+((it_round(PlanetHeader->PosX)+OffsetX)*32);
            y = 256+((it_round(PlanetHeader->PosY)+OffsetY)*32);
            if ((x>=0) && (x<=480) && (y>=0) && (y<=480))
            {
                BltBitMapRastPort((struct BitMap*) &ImgBitMap7,PlanetHeader->Class*32,0,&(MyScreen[0]->RastPort),x,y,32,32,192);
            }
        }
    }
}

void DRAWMOVINGSHIP(r_ShipHeader* MyShipPtr, uint8 ActSys)
{
    bool b;

    uint8   Step;
    r_ShipHeader*   UseShipPtr;    
    
    if (Display == 0)
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
        if ((MOVESHIP_x>=0) && (MOVESHIP_x<=480) && (MOVESHIP_y>=0) && (MOVESHIP_y<=480))
        {
            FASTREFRESH(MOVESHIP_FromX,MOVESHIP_FromY,MyShipPtr);
            RECT(MyScreen[0],0,MOVESHIP_x,MOVESHIP_y,MOVESHIP_x+31,MOVESHIP_y+31);
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
        if (MyShipPtr->SType == SHIPTYPE_FLEET)
        {
            UseShipPtr = MyShipPtr->TargetShip;
        } else {
            UseShipPtr = MyShipPtr;
        }
        if ((MOVESHIP_x>=0) && (MOVESHIP_x<=480) && (MOVESHIP_y>=0) && (MOVESHIP_y<=480))
        {
            BltBitMapRastPort((struct BitMap*) &ImgBitMap4,(UseShipPtr->SType-8)*32,32,&(MyScreen[0]->RastPort),MOVESHIP_x,MOVESHIP_y,31,31,192);
            SetAPen(&(MyScreen[0]->RastPort),UseShipPtr->Owner);
            DRAWRECT(MOVESHIP_x,MOVESHIP_y,MyShipPtr);
            if (MyShipPtr->Owner != FLAG_OTHER)
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
