#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void DRAWSYSTEM(const int Mode, int ActSys, r_ShipHeader* ActShipPtr)
{
    int             x,y,i;
    r_ShipHeader*   MyShipPtr;
    r_ShipHeader*   UseShipPtr;
    r_PlanetHeader* PlanetHeader;
    uint8           Leave, j;
    uint8           BelongsTo, DrawMode;
    int             Sunleft, Sunwidth;

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
            WRITE_RP0(576,418,0, WRITE_Center              ,3,_PT_Suchen);
            WRITE_RP0(576,446,0, WRITE_Center              ,3,_PT_Sternenkarte);
            WRITE_RP0(576,474,8,(WRITE_Center|WRITE_Shadow),3,_PT_Rundenende);
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

    /*       ,---,   A 
      Sun   /     \  B
            \     /  C
             `---´   D
    */
    if ((OffsetY>-10) && (OffsetY<9) && (OffsetX>-10) && (OffsetX<9))  // draw the sun to the main view
    {
        if      (OffsetX==-9)   { Sunleft=48; Sunwidth=16-2; }
        else if (OffsetX==-8)   { Sunleft=16; Sunwidth=48-2; }
        else if (OffsetX==7)    { Sunleft=0;  Sunwidth=48; }
        else if (OffsetX==8)    { Sunleft=0;  Sunwidth=16; }
        else                    { Sunleft=0;  Sunwidth=64-2; }

        if        (OffsetY == -9) {
        } else if ((OffsetY == -8) || (OffsetY == 8)) {
            BltBitMapRastPort((struct BitMap*) &ImgBitMap7,288+Sunleft, 8-OffsetY,MyRPort_PTR[0],
                                                           240+Sunleft+(OffsetX*32),248-OffsetY+(OffsetY*32),Sunwidth,16,192); // A or B
        } else {
            BltBitMapRastPort((struct BitMap*) &ImgBitMap7,288+Sunleft, 0,MyRPort_PTR[0],
                                                           240+Sunleft+(OffsetX*32),240+(OffsetY*32),Sunwidth,32,192); // A+B
        }

        if        (OffsetY == 8) {
        } else if ((OffsetY == 7) || (OffsetY == -9)) {
            BltBitMapRastPort((struct BitMap*) &ImgBitMap7,352+Sunleft, 7-OffsetY,MyRPort_PTR[0],
                                                           240+Sunleft+(OffsetX*32),279-OffsetY+(OffsetY*32),Sunwidth,16,192); // C or D
        } else {
            BltBitMapRastPort((struct BitMap*) &ImgBitMap7,352+Sunleft, 0,MyRPort_PTR[0],
                                                           240+Sunleft+(OffsetX*32),272+(OffsetY*32),Sunwidth,32,192); // C+D
        }
    }

    SetAPen(MyRPort_PTR[0],12);
    for (j = 0; j < MAXHOLES; ++j)
    {
        if (0 == MyWormHole[i].System[0]) { continue; }

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
                        WRITE_RP0(x+16,y+20,BelongsTo,WRITE_Center,0, Save.SystemName.data[MyWormHole[j].System[1-i]-1]);
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
            x = it_round(PlanetHeader->PosX);
            y = it_round(PlanetHeader->PosY);
            if (MODE_REDRAW == Mode)
            {
                SetAPen(MyRPort_PTR[0],BelongsTo);
                RectFill(MyRPort_PTR[0], 575+x, 62+y, 576+x, 63+y);
            }
            x = 256+((x+OffsetX)*32);
            y = 256+((y+OffsetY)*32);
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
                    DrawMode = 1;
                    if (NULL != PlanetHeader->ProjectPtr)
                    {
                        if (1 == PlanetHeader->ProjectPtr->data[PROJECT_SPACEDOCK])
                        {
                            BltBitMapRastPort((struct BitMap*) &ImgBitMap4,544,32,MyRPort_PTR[0],x-18,y+10,15,14,192);
                        }
                        if ((0 != PlanetHeader->ProjectPtr->data[PROJECT_SDI]) ||
                            (0 != PlanetHeader->ProjectPtr->data[PROJECT_SPACEPHALANX]))
                        {
                            DrawMode = 5;
                        }
                    }
                    WRITE_RP0(x+15,y+32,BelongsTo,(DrawMode|WRITE_Center),0,PlanetHeader->PName);
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
                    RECT_RP0_C0(x,y,x+31,y+31);
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
                        WRITE_RP0(x+ 7,y+9,12,WRITE_Shadow,3,"W");
                    }
                    if (TARGET_POSITION == MyShipPtr->Target)
                    {
                        WRITE_RP0(x+10,y+9,12,WRITE_Shadow,3,"P");
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
    WRITE_RP0(200,491,BelongsTo,0,3,Save.SystemName.data[ActSys]);
    PRINTGLOBALINFOS(ActPlayer-1);
    if (!Save.PlayMySelf) { ScreenToFront(MyScreen[0]); }
}
