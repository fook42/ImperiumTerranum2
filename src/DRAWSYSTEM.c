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
    sint16          Sun_PosX, Sunright, Sunwidth;
    sint16          Sun_PosY, Sun_PosY0, Sun_PosY1;
    sint16          Suntop_source_top, Sunbottom_source_top;
    sint16          Suntop_height, Sunbottom_height;
    uint8           gridSize_X, gridSize_Y;
    const uint16    MiniMap_right  = HighRes_Width-11;
    const uint16    MiniMap_center = HighRes_Width-64;
    const uint16    MiniMap_left   = HighRes_Width-118;
 
/*    OffsetX,Y = centre of current view */
    OldX = OffsetX;
    OldY = OffsetY;
    if (MODE_REDRAW == Mode)
    {
        if ((0 == Display) || (100 == Display))
        {
            y = HighRes_Height-96; // 416;
            for (i = 0; i<3; ++i)
            {
                DrawImage(MyRPort_PTR[0],&GadImg1,(HighRes_Width-122), y);
                y+=28;
            }
            WRITE_RP0(MiniMap_center,(HighRes_Height-94),0,JAM1|WRITE_Center             ,3,_PT_Suchen);
            WRITE_RP0(MiniMap_center,(HighRes_Height-66),0,JAM1|WRITE_Center             ,3,_PT_Sternenkarte);
            WRITE_RP0(MiniMap_center,(HighRes_Height-38),8,JAM1|WRITE_Center|WRITE_Shadow,3,_PT_Rundenende);
        }
        // Display = ActSys;
        SetAPen(MyRPort_PTR[0], 0);
        RectFill(MyRPort_PTR[0], 0, 0, Area_Width-1, Area_Height-1);   // clear main display
        RectFill(MyRPort_PTR[0], MiniMap_left, 9, MiniMap_right, 116); // clear little map
        SetAPen(MyRPort_PTR[0], 10);
        RectFill(MyRPort_PTR[0], MiniMap_center-1, 62, MiniMap_center+1, 63); // draw sun-spot inside little map
    }
    Display = ActSys;
    SetAPen(MyRPort_PTR[0],109);

    gridSize_X=(uint8) (Area_Width >>5); // = div 32
    gridSize_Y=(uint8) (Area_Height>>5);

    x = 63-OffsetY-(gridSize_Y/2)-2;    // upper line (little map)
    y = 63-OffsetY+(gridSize_Y/2);      // lower line (little map)
    if (x<9)   { x = 9; }
    if (y>116) { y = 116; }
    Move(MyRPort_PTR[0],MiniMap_left ,x);
    Draw(MyRPort_PTR[0],MiniMap_right,x);
    Move(MyRPort_PTR[0],MiniMap_left ,y);
    Draw(MyRPort_PTR[0],MiniMap_right,y);

    x = MiniMap_center-OffsetX-(gridSize_X/2)-2;    // left  line (little map)
    y = MiniMap_center-OffsetX+(gridSize_X/2);      // right line (little map)
    if (x<MiniMap_left ) { x = MiniMap_left; }
    if (y>MiniMap_right) { y = MiniMap_right; }
    Move(MyRPort_PTR[0],x,9);
    Draw(MyRPort_PTR[0],x,116);
    Move(MyRPort_PTR[0],y,9);
    Draw(MyRPort_PTR[0],y,116);

    /*       .---.   Sun_PosY0
      Sun   /     \   -Suntop_height
            \     /  Sun_PosY1
           | '---'    -Sunbottom_height
     Sun_PosX -Sunwidth
    */

    Sun_PosY0 = 0;
    Sun_PosY1 = 0;
    Suntop_height = 0;
    Sunbottom_height = 0;
    Suntop_source_top = 0;
    Sunbottom_source_top = 0;
    Sun_PosY = Area_CenterY+(OffsetY-1)*32;

    if ((-64 <= Sun_PosY) && (Area_Height > Sun_PosY))
    {
        Suntop_source_top = 32; // => h0 = 0
        Sunbottom_source_top = 0-(Sun_PosY+32);
        if (-32 <= Sun_PosY)
        {
            Suntop_source_top = 0-Sun_PosY;
            Sunbottom_source_top = 0;
            if (0 <= Sun_PosY)
            {
                Suntop_source_top = 0;
            }
        }
        Suntop_height    = 32-Suntop_source_top;
        Sunbottom_height = 32-Sunbottom_source_top;
        if ((Area_Height-64) <= Sun_PosY)
        {
            if ((Area_Height-32) > Sun_PosY)
            {
                Sunbottom_height = Area_Height-(Sun_PosY+32);
            }
            if ((Area_Height-32) <= Sun_PosY)
            {
                Suntop_height = Area_Height-Sun_PosY;
                Sunbottom_height = 0;
            }
        }
    }

    Sun_PosY0 = Sun_PosY   +Suntop_source_top;
    Sun_PosY1 = Sun_PosY+32+Sunbottom_source_top;

    Sunwidth = 0;
    Sunright = Area_CenterX+(OffsetX+1)*32;
    Sun_PosX  = Sunright-64;
    if ((Sunright > 0) && (Sun_PosX < Area_Width))
    {
        if (Sunright > Area_Width)
        {
            Sunwidth = Area_Width-Sun_PosX;
            BltBitMapRastPort((struct BitMap*) &ImgBitMap7,288, Suntop_source_top, MyRPort_PTR[0],
                            Sun_PosX, Sun_PosY0, Sunwidth, Suntop_height,192);
            BltBitMapRastPort((struct BitMap*) &ImgBitMap7,352, Sunbottom_source_top, MyRPort_PTR[0],
                            Sun_PosX, Sun_PosY1, Sunwidth, Sunbottom_height,192);
        } else
        {
            Sunwidth = 64;
            if (Sunright < 64)
            {
                Sunwidth = Sunright;
                Sun_PosX  = 0;
            }
            BltBitMapRastPort((struct BitMap*) &ImgBitMap7,288+64-Sunwidth, Suntop_source_top, MyRPort_PTR[0],
                            Sun_PosX, Sun_PosY0, Sunwidth, Suntop_height,192);
            BltBitMapRastPort((struct BitMap*) &ImgBitMap7,352+64-Sunwidth, Sunbottom_source_top, MyRPort_PTR[0],
                            Sun_PosX, Sun_PosY1, Sunwidth, Sunbottom_height,192);
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
                        WRITE_RP0(x+16,y+20,BelongsTo,JAM1|WRITE_Center,0, Save.SystemName[MyWormHole[j].System[1-i]-1]);
                    }
                    if (0 != Save.CivPlayer[ActPlayer-1])
                    {
                        MyWormHole[j].CivKnowledge[ActPlayer-1] = FLAG_KNOWN;
                    }
                }
                if (FLAG_KNOWN == MyWormHole[j].CivKnowledge[ActPlayer-1])
                {
                    WritePixel(MyRPort_PTR[0],MiniMap_center-1+MyWormHole[j].PosX[i],62+MyWormHole[j].PosY[i]);
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
            RectFill(MyRPort_PTR[0], MiniMap_center-1+x, 62+y, MiniMap_center+x, 63+y);
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
                WritePixel(MyRPort_PTR[0],MiniMap_center-1+MyShipPtr->PosX,62+MyShipPtr->PosY);
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
    WRITE_RP0(Area_CenterX, (HighRes_Height-21),BelongsTo,(JAM1 | WRITE_Center),3,Save.SystemName[ActSys]);
    PRINTGLOBALINFOS(ActPlayer-1);
    if (!Save.PlayMySelf) { ScreenToFront(MyScreen[0]); }
}
