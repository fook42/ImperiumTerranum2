#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void DRAWSYSTEM(uint8 Mode, uint8 ActSys, r_ShipHeader* ActShipPtr)
{
    sint16          x,y,i;
    r_ShipHeader*   MyShipPtr;
    r_ShipHeader*   UseShipPtr;
    r_PlanetHeader* PlanetHeader;
    uint8           Leave, j;
    uint8           BelongsTo;

/*    OffsetX,Y = centre of current view */
    OldX = OffsetX;
    OldY = OffsetY;
    if (MODE_REDRAW == Mode)
    {
        if ((0 == Display) || (100 == Display))
        {
            for (i = 0; i<3; ++i)
            {
                DrawImage(MyRPort_PTR[0],&GadImg1,518,416+i*28);
            }
            WRITE(576,418,0, WRITE_Center              ,MyRPort_PTR[0],3,_PT_Suchen);
            WRITE(576,446,0, WRITE_Center              ,MyRPort_PTR[0],3,_PT_Sternenkarte);
            WRITE(576,474,8,(WRITE_Center|WRITE_Shadow),MyRPort_PTR[0],3,_PT_Rundenende);
        }
        // Display = ActSys;
        SetAPen(MyRPort_PTR[0], 0);
        RectFill(MyRPort_PTR[0], 0, 0, 511, 511);   // clear main display
        RectFill(MyRPort_PTR[0], 522, 9, 629, 116); // clear little map
        SetAPen(MyRPort_PTR[0], 10);
        RectFill(MyRPort_PTR[0], 575, 62, 577, 63); // draw sun-spot inside little map
    }
    Display = ActSys;
    SetAPen(MyRPort_PTR[0],109);
    x = 63-OffsetY-10;      // upper line (little map)
    y = 63-OffsetY+8;       // lower line (little map)
    if (x<9)   { x = 9; }
    if (y>116) { y = 116; }
    Move(MyRPort_PTR[0],522,x);
    Draw(MyRPort_PTR[0],629,x);
    Move(MyRPort_PTR[0],522,y);
    Draw(MyRPort_PTR[0],629,y);
    x = 576-OffsetX-10;     // left  line (little map)
    y = 576-OffsetX+8;      // right line (little map)
    if (x<522) { x = 522; }
    if (y>629) { y = 629; }
    Move(MyRPort_PTR[0],x,9);
    Draw(MyRPort_PTR[0],x,116);
    Move(MyRPort_PTR[0],y,9);
    Draw(MyRPort_PTR[0],y,116);


    if ((OffsetY>-9) && (OffsetY<8))     // draw the sun to the main view
    {
        if (OffsetY>-8)
        {
            if ((OffsetX>-9) && (OffsetX<8))
            {
                if (OffsetX>-8)
                {
                    BltBitMapRastPort((struct BitMap*) &ImgBitMap7,288,0,MyRPort_PTR[0],224+(OffsetX*32)+16,224+(OffsetY*32)+16,32,32,192); // upper left hemisphere
                } else {
                    BltBitMapRastPort((struct BitMap*) &ImgBitMap7,304,0,MyRPort_PTR[0],224+(OffsetX*32)+16,224+(OffsetY*32)+16,16,32,192); // upper left hemisphere/2
                }
                if (OffsetX<7)
                {
                    BltBitMapRastPort((struct BitMap*) &ImgBitMap7,320,0,MyRPort_PTR[0],256+(OffsetX*32)+16,224+(OffsetY*32)+16,32,32,192); // upper right hemisphere
                } else {
                    BltBitMapRastPort((struct BitMap*) &ImgBitMap7,320,0,MyRPort_PTR[0],256+(OffsetX*32)+16,224+(OffsetY*32)+16,16,32,192); // upper right hemisphere/2
                }
            }
        }
        if (OffsetY<7)
        {
            if ((OffsetX>-9) && (OffsetX<8))
            {
                if (OffsetX>-8)
                {
                    BltBitMapRastPort((struct BitMap*) &ImgBitMap7,352,0,MyRPort_PTR[0],224+(OffsetX*32)+16,256+(OffsetY*32)+16,32,32,192); // lower left hemisphere
                }
                if (OffsetX<7)
                {
                    BltBitMapRastPort((struct BitMap*) &ImgBitMap7,384,0,MyRPort_PTR[0],256+(OffsetX*32)+16,256+(OffsetY*32)+16,32,32,192); // lower right hemisphere
                }
            }
        }
    }

    if ((-9 < OffsetX) && (8 > OffsetX))
    {
    }

    SetAPen(MyRPort_PTR[0],12);
    for (j = 0; j < MAXHOLES; ++j)
    {
        for (i = 0; i < 2; ++i)
        {
            if (MyWormHole[j].System[i] == ActSys)
            {
                x = 256+((MyWormHole[j].PosX[i]+OffsetX)*32);
                y = 256+((MyWormHole[j].PosY[i]+OffsetY)*32);
                if ((x>=0) && (x<=480) && (y>=0) && (y<=480))
                {
                    RectFill(MyRPort_PTR[0],x+15,y+15,x+17,y+17);
                    if ((x>=32) && (x<=448) /*&& (y>=32)*/ && (y<=448))
                    {
                        BelongsTo = SystemFlags[0][MyWormHole[j].System[1-i]-1] & FLAG_CIV_MASK;
                        if (0 == BelongsTo)
                        {
                            BelongsTo = 12;
                        }
                        WRITE(x+16,y+20,BelongsTo,WRITE_Center,MyRPort_PTR[0],0, Save.SystemName.data[MyWormHole[j].System[1-i]-1]);
                    }
                    if (Save.CivPlayer[ActPlayer-1] != 0)
                    {
                        MyWormHole[j].CivKnowledge[ActPlayer-1] = FLAG_KNOWN;
                    }
                }
                if (MyWormHole[j].CivKnowledge[ActPlayer-1] == FLAG_KNOWN)
                {
                    WritePixel(MyRPort_PTR[0],575+MyWormHole[j].PosX[i],62+MyWormHole[j].PosY[i]);
                }
            }
        }
    }
    ActSys--; // to shift the arrays
    for (i = 0; i < SystemHeader[ActSys].Planets; ++i)
    {
        PlanetHeader = &(SystemHeader[ActSys].PlanetMemA[i]);
        if (NULL != PlanetHeader)
        {
            BelongsTo = PlanetHeader->PFlags;
            if (0 == BelongsTo)
            {
                BelongsTo = 12;
            }
            if (Mode == MODE_REDRAW)
            {
                SetAPen(MyRPort_PTR[0],BelongsTo);
                RectFill(MyRPort_PTR[0], 575+it_round(PlanetHeader->PosX), 62+it_round(PlanetHeader->PosY),
                                         576+it_round(PlanetHeader->PosX), 63+it_round(PlanetHeader->PosY));
            }
            x = 256+((it_round(PlanetHeader->PosX)+OffsetX)*32);
            y = 256+((it_round(PlanetHeader->PosY)+OffsetY)*32);
            if ((x>=0) && (x<=480) && (y>=0) && (y<=480))
            {
                BltBitMapRastPort((struct BitMap*) &ImgBitMap7,PlanetHeader->Class*32,0,MyRPort_PTR[0],x,y,32,32,192);
                if ((x>=32) && (x<=448) /* && (y>=32)*/ && (y<=448))
                {
                    MyShipPtr = PlanetHeader->FirstShip.NextShip;
                    while ((NULL != MyShipPtr) && (0 == MyShipPtr->Owner))
                    {
                        MyShipPtr = MyShipPtr->NextShip;
                    }
                    if (NULL != MyShipPtr)
                    {
                        SetAPen(MyRPort_PTR[0],MyShipPtr->Owner & FLAG_CIV_MASK);
                    } else {
                        SetAPen(MyRPort_PTR[0],0);
                    }
                    DrawEllipse(MyRPort_PTR[0],x+15,y+15,24,23);
                    if (NULL != PlanetHeader->ProjectPtr)
                    {
                        if (1 == PlanetHeader->ProjectPtr->data[25])
                        {
                            BltBitMapRastPort((struct BitMap*) &ImgBitMap4,544,32,MyRPort_PTR[0],x-18,y+10,15,14,192);
                        }
                        if ((0 != PlanetHeader->ProjectPtr->data[34]) ||
                            (0 != PlanetHeader->ProjectPtr->data[40]))
                        {
                            WRITE(x+15,y+32,BelongsTo,(5|WRITE_Center),MyRPort_PTR[0],0,PlanetHeader->PName);
                        } else {
                            WRITE(x+15,y+32,BelongsTo,(1|WRITE_Center),MyRPort_PTR[0],0,PlanetHeader->PName);
                        }
                    } else {
                        WRITE(x+15,y+32,BelongsTo,(1|WRITE_Center),MyRPort_PTR[0],0,PlanetHeader->PName);
                    }
                }
            }
        }
    }
    Leave = 0;
    MyShipPtr = &SystemHeader[ActSys].FirstShip;
    while ((NULL != MyShipPtr) && (Leave<2))
    {
        if (SHIPTYPE_FLEET == MyShipPtr->SType)
        {
            UseShipPtr = MyShipPtr->TargetShip;
            while ((NULL != UseShipPtr) && (0 == UseShipPtr->Owner))
            {
                UseShipPtr = UseShipPtr->NextShip;
            }
        } else {
            UseShipPtr = MyShipPtr;
        }
        if (NULL != UseShipPtr)
        {
            if (((MyShipPtr->Owner>0) && (UseShipPtr->Owner>0) && (MyShipPtr->Moving >= 0)
                    && (MyShipPtr->SType>0)) || (TARGET_STARGATE == MyShipPtr->SType))
            {
                x = 256+((MyShipPtr->PosX+OffsetX)*32);
                y = 256+((MyShipPtr->PosY+OffsetY)*32);
                if (SHIPTYPE_FLEET == MyShipPtr->SType)
                {
                    UseShipPtr = MyShipPtr->TargetShip;
                } else {
                    UseShipPtr = MyShipPtr;
                }
                if ((x>=0) && (x<=480) && (y>=0) && (y<=480))
                {
                    RECTWIN(MyRPort_PTR[0],0,x,y,x+31,y+31);
                    BltBitMapRastPort((struct BitMap*) &ImgBitMap4,(UseShipPtr->SType-8)*32,32,MyRPort_PTR[0],x,y,31,31,192);
                }
                if (TARGET_STARGATE != UseShipPtr->SType)
                {
                    SetAPen(MyRPort_PTR[0],MyShipPtr->Owner);
                } else {
                    SetAPen(MyRPort_PTR[0],12);
                }
                WritePixel(MyRPort_PTR[0],575+MyShipPtr->PosX,62+MyShipPtr->PosY);
                if ((x>=0) && (x<=480) && (y>=0) && (y<=480) && (TARGET_STARGATE != MyShipPtr->SType))
                {
                    BOX(MyRPort_PTR[0],x,y,x+31,y+31);
                    if (SHIPTYPE_FLEET == MyShipPtr->SType)
                    {
                        BOX(MyRPort_PTR[0],x+2,y+2,x+29,y+29);
                    }
                    if (SHIPFLAG_WATER == MyShipPtr->Flags)
                    {
                        WRITE(x+8,y+10,0,0,MyRPort_PTR[0],3,"W");
                        WRITE(x+7,y+9,12,0,MyRPort_PTR[0],3,"W");
                    }
                    if (TARGET_POSITION == MyShipPtr->Target)
                    {
                        WRITE(x+11,y+10,0,0,MyRPort_PTR[0],3,"P");
                        WRITE(x+10,y+9,12,0,MyRPort_PTR[0],3,"P");
                    }
                }
            }
        }
        MyShipPtr = MyShipPtr->NextShip;
        if (1 == Leave) { Leave = 2; }

        if ((0 == Leave) && (NULL == MyShipPtr))
        {
            MyShipPtr = ActShipPtr;
            Leave = 1;
        }
    }
    BelongsTo = SystemFlags[0][ActSys] & FLAG_CIV_MASK;
    if (0 == BelongsTo)
    {
        BelongsTo = 12;
    }
    WRITE(200,491,BelongsTo,0,MyRPort_PTR[0],3,Save.SystemName.data[ActSys]);
    PRINTGLOBALINFOS(ActPlayer-1);
    if (!Save.PlayMySelf) { ScreenToFront(MyScreen[0]); }
}
