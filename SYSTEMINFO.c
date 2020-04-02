#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void SYSTEMINFO(uint8 ActSys)
{
    int     i;
    sint32  l;
    uint8   SysID;
    sint16  x,y;
    bool    b;
    r_ShipHeader*   MyShipPtr;
    r_ShipHeader*   UseShipPtr;
    bool    FleetUsed;
    struct Window* SYS_Window;
    struct RastPort* RPort_PTR;

    b = false;
    if (NULL == ObjPtr)
    {
        return;
    }
    MyShipPtr = (r_ShipHeader*) ObjPtr;
    if (0 == Save.CivPlayer[GETCIVVAR(MyShipPtr->Owner)-1])
    {
        return;
    }

    SYS_Window=MAKEWINDOW(194,119,123,114,MyScreen[0]);
    if (NULL == SYS_Window)
    {
        return;
    }
    RPort_PTR = SYS_Window->RPort;
    MAKEWINBORDER(RPort_PTR,0,0,122,113,12,6,1);

    UseShipPtr = MyShipPtr;
    if (SHIPTYPE_FLEET == MyShipPtr->SType)
    {
        FleetUsed = true;
        MyShipPtr = MyShipPtr->TargetShip;
    } else {
        FleetUsed = false;
    }
    y = 3;
    for(i = 0; i < 5; ++i)
    {
        DrawImage(RPort_PTR,&GadImg1,4,y);
        y += 22;
    }
    WRITE(62, 5,0,WRITE_Center,RPort_PTR,3,_PT_Sterne);
    WRITE(62,27,0,WRITE_Center,RPort_PTR,3,_PT_Sprengen);
    if (TARGET_POSITION == UseShipPtr->Target)
    {
        WRITE(62,49,0,WRITE_Center,RPort_PTR,3,_PT_Abloesen);
    } else {
        WRITE(62,49,0,WRITE_Center,RPort_PTR,3,_PT_Position);
    }
    if (SHIPTYPE_STARGATE != MyShipPtr->SType)
    {
        WRITE(62,71,0,WRITE_Center,RPort_PTR,3,_PT_Bewaessern);
    } else {
        WRITE(62,71,0,WRITE_Center,RPort_PTR,3,_PT_Stargate);
    }
    WRITE(62,93,0,WRITE_Center,RPort_PTR,3,_PT_Warten);
    do
    {
        Delay(RDELAY);
        if (LMB_PRESSED)
        {
            if ((-1 < SYS_Window->MouseX) && (123 > SYS_Window->MouseX))
            {
                if ((2 < SYS_Window->MouseY) && (24 > SYS_Window->MouseY))
                {
                    // travel to different system ...
                    KLICKWINGAD(RPort_PTR,4,3);
                    b = true;
                    CloseWindow(SYS_Window);
                    SYS_Window = NULL;

                    DRAWSTARS(MODE_REDRAW,ActPlayer);
                    SysID = 0;
                    do
                    {
                        Delay(RDELAY);
                        if (LMB_PRESSED)
                        {
                            SysID = LastSystem;
                            PLAYSOUND(1,300);
                        } else {
                            WRITEGALAXYDATA(ActSys,ShipData(MyShipPtr->SType).MaxMove);
                        }
                    }
                    while((RMB_NOTPRESSED) && (0 == SysID));

                    if (RMB_PRESSED) { PLAYSOUND(1,300); }
                    if ((0 < SysID) && (MAXSYSTEMS >= SysID) && (ActSys != SysID))
                    {
                        if (0 == SystemHeader[SysID-1].Planets) { CREATENEWSYSTEM(SysID-1, 1); }
                        MyShipPtr->Target = SysID;
                        MyShipPtr->Source = ActSys;
                        l = abs(SystemX[ActSys-1]-SystemX[SysID-1]) + abs(SystemY[ActSys-1]-SystemY[SysID-1]);
                        l = (l / ShipData(MyShipPtr->SType).MaxMove)+1;
                        if (127 < l) { l = 127; }
                        MyShipPtr->Moving = (-l);
                        if (!FleetUsed)
                        {
                            LINKSHIP(MyShipPtr, &SystemHeader[SysID-1].FirstShip, 1);
                        } else {
                            MyShipPtr->BeforeShip->Moving = MyShipPtr->Moving;
                            MyShipPtr->BeforeShip->Target = MyShipPtr->Target;
                            MyShipPtr->BeforeShip->Source = MyShipPtr->Source;
                            LINKSHIP(MyShipPtr->BeforeShip, &SystemHeader[SysID-1].FirstShip, 1);
                            MyShipPtr = SystemHeader[i-1].FirstShip.NextShip;       // TODO: where does "i" come from?
                        }
                    }

                    DRAWSYSTEM(MODE_REDRAW,ActSys,NULL);
                } else if ((24 < SYS_Window->MouseY) && (46 > SYS_Window->MouseY))
                {
                    // destroy ship ...
                    KLICKWINGAD(RPort_PTR,4,25);
                    b = true;
                    CloseWindow(SYS_Window);
                    SYS_Window = NULL;

                    x = 256+(UseShipPtr->PosX+OffsetX)*32;
                    y = 256+(UseShipPtr->PosY+OffsetY)*32;
                    PLAYSOUND(2,1100);
                    // show explosion-animation
                    if ((x>=0) && (x<481) && (y>=0) && (y<481))
                    {
                        RECT_RP0(0,x,y,x+31,y+31);
                        for(i = 0; i < 16; ++i)
                        {
                            BltBitMapRastPort((struct BitMap*) &ImgBitMap4,i*32,0,MyRPort_PTR[0],x,y,31,31,192);
                            Delay(5);
                        }
                        RECT_RP0(0,x,y,x+31,y+31);
                    }
                    Delay(10);
                    // remove ship/fleet
                    UseShipPtr->Owner = 0;
                    UseShipPtr->SType = 8;
                    UseShipPtr->Moving = 0;
                    if (FleetUsed)
                    {
                        do
                        {
                            MyShipPtr->Owner = 0;
                            MyShipPtr = MyShipPtr->NextShip;
                        }
                        while (NULL != MyShipPtr);
                    }
                    custom.dmacon = BITCLR | DMAF_AUD0 | DMAF_AUD1; // 0x0003
                } else if ((46 < SYS_Window->MouseY) && (68 > SYS_Window->MouseY))
                {
                    // free/stick to position ...
                    KLICKWINGAD(RPort_PTR,4,47);
                    b = true;
                    CloseWindow(SYS_Window);
                    SYS_Window = NULL;

                    if (TARGET_POSITION == UseShipPtr->Target)
                    {
                        UseShipPtr->Target = ActSys;
                        if (UseShipPtr->Moving>0)
                        {
                            MOVESHIP(ActSys,UseShipPtr,false);
                        }
                    } else {
                        UseShipPtr->Target = TARGET_POSITION;
                    }
                } else if ((68 < SYS_Window->MouseY) && (90 > SYS_Window->MouseY))
                {
                    // watering / turn to stargate ...
                    KLICKWINGAD(RPort_PTR,4,69);
                    b = true;
                    CloseWindow(SYS_Window);
                    SYS_Window = NULL;
                    if (SHIPTYPE_STARGATE == MyShipPtr->SType)
                    {
                        if (it_round((double) sqrt((MyShipPtr->PosX*MyShipPtr->PosX)+(MyShipPtr->PosY*MyShipPtr->PosY))) >= 10)
                        {
                            // too far from sun
                            SYS_Window=MAKEWINDOW(70,115,371,86,MyScreen[0]);
                            if (NULL != SYS_Window)
                            {
                                RPort_PTR = SYS_Window->RPort;
                                MAKEWINBORDER(RPort_PTR,0,0,370,86,12,6,1);
                                WRITE(186,10,ActPlayerFlag,WRITE_Center,RPort_PTR,3,PText[481]);
                                WRITE(186,32,ActPlayerFlag,WRITE_Center,RPort_PTR,3,PText[482]);
                                WRITE(186,54,ActPlayerFlag,WRITE_Center,RPort_PTR,3,PText[483]);
                                WAITLOOP(false);
                            }
                        } else if (0 == SystemHeader[ActSys-1].FirstShip.SType)
                        {
                            // install stargate
                            SystemHeader[ActSys-1].FirstShip.SType = TARGET_STARGATE;
                            SystemHeader[ActSys-1].FirstShip.PosX = MyShipPtr->PosX;
                            SystemHeader[ActSys-1].FirstShip.PosY = MyShipPtr->PosY;
                            MyShipPtr->Owner = 0;

                            REFRESHDISPLAY();
                            Delay(15);
                        } else {
                            // stargate is already there
                            REQUEST(PText[485],PText[486],ActPlayerFlag,ActPlayerFlag);
                        }
                    } else {
                        // turn the ship into/out of watering-mode
                        if (0 == MyShipPtr->Ladung)
                        {
                            if (!FleetUsed)
                            {
                                MyShipPtr->Flags = 1-MyShipPtr->Flags;
                                /* SHIPFLAG_NONE <-> SHIPFLAG_WATER */
                                MyShipPtr->Moving = 0;
                                REFRESHDISPLAY();
                            } else {
                                // fleets cant water
                                REQUEST(PText[487],PText[488],ActPlayerFlag,ActPlayerFlag);
                            }
                        } else {
                            // still freight on board
                            REQUEST(PText[489],PText[490],ActPlayerFlag,ActPlayerFlag);
                        }
                    }
                } else if ((SYS_Window->MouseY>=91) && (SYS_Window->MouseY<=111))
                {
                    // wait at current position...
                    KLICKWINGAD(RPort_PTR,4,91);
                    UseShipPtr->Moving = 0;
                    b = true;
                }
            }
        }
        if (RMB_PRESSED)
        {
            PLAYSOUND(1,300);
            b = true;
        }
    }
    while (!b);
    if (NULL != SYS_Window)
    {
        CloseWindow(SYS_Window);
    }
}
