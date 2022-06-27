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

/*    OffsetX,Y = centre of current view */
    OldX = OffsetX;
    OldY = OffsetY;
    if (MODE_REDRAW == Mode)
    {
        if ((0 == Display) || (100 == Display))
        {
            for (i = 0; i<3; ++i)
            {
                DrawImage(MyRPort_PTR[0],&GadImg1,(HighRes_Width-122), 416+i*28);
            }
            WRITE_RP0((HighRes_Width-64),418,0,JAM1|WRITE_Center             ,3,_PT_Suchen);
            WRITE_RP0((HighRes_Width-64),446,0,JAM1|WRITE_Center             ,3,_PT_Sternenkarte);
            WRITE_RP0((HighRes_Width-64),474,8,JAM1|WRITE_Center|WRITE_Shadow,3,_PT_Rundenende);
        }
        // Display = ActSys;
        SetAPen(MyRPort_PTR[0], 0);
        RectFill(MyRPort_PTR[0], 0, 0, Area_Width-1, Area_Height-1);   // clear main display
        RectFill(MyRPort_PTR[0], (HighRes_Width-118), 9, (HighRes_Width-11), 116); // clear little map
        SetAPen(MyRPort_PTR[0], 10);
        RectFill(MyRPort_PTR[0], (HighRes_Width-65), 62, (HighRes_Width-63), 63); // draw sun-spot inside little map
    }
    Display = ActSys;
    SetAPen(MyRPort_PTR[0],109);
    x = 63-OffsetY-10;      // upper line (little map)
    y = 63-OffsetY+8;       // lower line (little map)
    if (x<9)   { x = 9; }
    if (y>116) { y = 116; }
    Move(MyRPort_PTR[0],(HighRes_Width-118),x);
    Draw(MyRPort_PTR[0],(HighRes_Width- 11),x);
    Move(MyRPort_PTR[0],(HighRes_Width-118),y);
    Draw(MyRPort_PTR[0],(HighRes_Width- 11),y);
    x = (HighRes_Width-64)-OffsetX-10;     // left  line (little map)
    y = (HighRes_Width-64)-OffsetX+8;      // right line (little map)
    if (x<(HighRes_Width-118)) { x = (HighRes_Width-118); }
    if (y>(HighRes_Width- 11)) { y = (HighRes_Width- 11); }
    Move(MyRPort_PTR[0],x,9);
    Draw(MyRPort_PTR[0],x,116);
    Move(MyRPort_PTR[0],y,9);
    Draw(MyRPort_PTR[0],y,116);

    /*       .---.   A 
      Sun   /     \  B
            \     /  C
             '---'   D
    */
    if ((-10 < OffsetY) && (9 > OffsetY) && (-10 < OffsetX) && (9 > OffsetX))  // draw the sun to the main view
    {
        if      (-9 == OffsetX) { Sunleft=48; Sunwidth=16-2; }
        else if (-8 == OffsetX) { Sunleft=16; Sunwidth=48-2; }
        else if ( 7 == OffsetX) { Sunleft=0;  Sunwidth=48; }
        else if ( 8 == OffsetX) { Sunleft=0;  Sunwidth=16; }
        else                    { Sunleft=0;  Sunwidth=64-2; }

        if       ((-8 == OffsetY) || (8 == OffsetY)) {
            BltBitMapRastPort((struct BitMap*) &ImgBitMap7,288+Sunleft, 8-OffsetY,MyRPort_PTR[0],
                                                           Area_CenterX-16+Sunleft+(OffsetX*32),248-OffsetY+(OffsetY*32),Sunwidth,16,192); // A or B
        } else if (-9 != OffsetY) {
            BltBitMapRastPort((struct BitMap*) &ImgBitMap7,288+Sunleft, 0,MyRPort_PTR[0],
                                                           Area_CenterX-16+Sunleft+(OffsetX*32),240+(OffsetY*32),Sunwidth,32,192); // A+B
        }

        if       ((-9 == OffsetY) || (7 == OffsetY)) {
            BltBitMapRastPort((struct BitMap*) &ImgBitMap7,352+Sunleft, 7-OffsetY,MyRPort_PTR[0],
                                                           Area_CenterX-16+Sunleft+(OffsetX*32),279-OffsetY+(OffsetY*32),Sunwidth,16,192); // C or D
        } else if ( 8 != OffsetY) {
            BltBitMapRastPort((struct BitMap*) &ImgBitMap7,352+Sunleft, 0,MyRPort_PTR[0],
                                                           Area_CenterX-16+Sunleft+(OffsetX*32),272+(OffsetY*32),Sunwidth,32,192); // C+D
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
                x = Area_CenterX+((MyWormHole[j].PosX[i]+OffsetX)*32);
                y = Area_CenterY+((MyWormHole[j].PosY[i]+OffsetY)*32);
                if ((0 <= x) && ((Area_Width-32) > x) && (0 <= y) && ((Area_Height-32) > y))
                {
                    RectFill(MyRPort_PTR[0],x+15,y+15,x+17,y+17);
                    if ((31 < x) && ((Area_Width-63) > x) /*&& (31 < y)*/ && ((Area_Height-63) > y))
                    {
                        BelongsTo = SystemFlags[0][MyWormHole[j].System[1-i]-1] & FLAG_CIV_MASK;
                        if (0 == BelongsTo)
                        {
                            BelongsTo = 12;
                        }
                        WRITE_RP0(x+16,y+20,BelongsTo,JAM1|WRITE_Center,0, Save.SystemName.data[MyWormHole[j].System[1-i]-1]);
                    }
                    if (0 != Save.CivPlayer[ActPlayer-1])
                    {
                        MyWormHole[j].CivKnowledge[ActPlayer-1] = FLAG_KNOWN;
                    }
                }
                if (FLAG_KNOWN == MyWormHole[j].CivKnowledge[ActPlayer-1])
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
        if (NULL == PlanetHeader) { continue; }

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
            RectFill(MyRPort_PTR[0], (HighRes_Width-65)+x, 62+y, (HighRes_Width-64)+x, 63+y);
        }
        x = Area_CenterX+((x+OffsetX)*32);
        y = Area_CenterY+((y+OffsetY)*32);
        if ((0 <= x) && ((Area_Width-32) > x) && (0 <= y) && ((Area_Height-32) > y))
        {
            BltBitMapRastPort((struct BitMap*) &ImgBitMap7,PlanetHeader->Class*32,0,MyRPort_PTR[0],x,y,32,32,192);
            if ((31 < x) && ((Area_Width-63) > x) /*&& (31 < y)*/ && ((Area_Height-63) > y))
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
                DrawMode = JAM2;
                if (NULL != PlanetHeader->ProjectPtr)
                {
                    if (1 == PlanetHeader->ProjectPtr->data[PROJECT_SPACEDOCK])
                    {
                        BltBitMapRastPort((struct BitMap*) &ImgBitMap4,544,32,MyRPort_PTR[0],x-18,y+10,15,14,192);
                    }
                    if ((0 != PlanetHeader->ProjectPtr->data[PROJECT_SDI]) ||
                        (0 != PlanetHeader->ProjectPtr->data[PROJECT_SPACEPHALANX]))
                    {
                        DrawMode = JAM2|INVERSVID;
                    }
                }
                WRITE_RP0(x+15,y+32,BelongsTo,(DrawMode|WRITE_Center),0,PlanetHeader->PName);
            }
        }
    }
    Leave = 0;
    MyShipPtr = &SystemHeader[ActSys].FirstShip;
    while ((NULL != MyShipPtr) && (2 > Leave))
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
            if (((0 < MyShipPtr->Owner) && (0 < UseShipPtr->Owner) && (0 <= MyShipPtr->Moving)
                    && (0 < MyShipPtr->SType)) || (TARGET_STARGATE == MyShipPtr->SType))
            {
                x = Area_CenterX+((MyShipPtr->PosX+OffsetX)*32);
                y = Area_CenterY+((MyShipPtr->PosY+OffsetY)*32);
                if (SHIPTYPE_FLEET == MyShipPtr->SType)
                {
                    UseShipPtr = MyShipPtr->TargetShip;
                } else {
                    UseShipPtr = MyShipPtr;
                }
                if ((0 <= x) && ((Area_Width-32) > x) && (0 <= y) && ((Area_Height-32) > y))
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
                WritePixel(MyRPort_PTR[0],(HighRes_Width-65)+MyShipPtr->PosX,62+MyShipPtr->PosY);
                if ((0 <= x) && ((Area_Width-32) > x) && (0 <= y) && ((Area_Height-32) > y) && (TARGET_STARGATE != MyShipPtr->SType))
                {
                    BOX(MyRPort_PTR[0],x,y,x+31,y+31);
                    if (SHIPTYPE_FLEET == MyShipPtr->SType)
                    {
                        BOX(MyRPort_PTR[0],x+2,y+2,x+29,y+29);
                    }
                    if (SHIPFLAG_WATER == MyShipPtr->Flags)
                    {
                        WRITE_RP0(x+ 7,y+9,12,JAM1|WRITE_Shadow,3,"W");
                    }
                    if (TARGET_POSITION == MyShipPtr->Target)
                    {
                        WRITE_RP0(x+10,y+9,12,JAM1|WRITE_Shadow,3,"P");
                    }
                    // show ship condition
                    i = (int) (25.0 * (1.0 - (double) UseShipPtr->Shield / ShipData(UseShipPtr->SType).MaxShield));
                    if (0 < i)
                    {
                        SetAPen(MyRPort_PTR[0],8); //red
                        RectFill(MyRPort_PTR[0], x+3, y+3, x+3+i, y+4);
/*
                        Move(MyRPort_PTR[0],x+3,  y+3);
                        Draw(MyRPort_PTR[0],x+3+i,y+3);
                        Move(MyRPort_PTR[0],x+3,  y+4);
                        Draw(MyRPort_PTR[0],x+3+i,y+4);
*/
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
    WRITE_RP0(Area_CenterX, (HighRes_Height-21),BelongsTo,(JAM1 | WRITE_Center),3,Save.SystemName.data[ActSys]);
    PRINTGLOBALINFOS(ActPlayer-1);
    if (!Save.PlayMySelf) { ScreenToFront(MyScreen[0]); }
}
