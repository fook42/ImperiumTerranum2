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
    for(i = 0; i < 5; i++)
    {
        DrawImage(RPort_PTR,&GadImg1,4,3+i*22);
    }
    WRITEWIN(62, 5,0,WRITE_Center,RPort_PTR,4,_PT_Sterne);
    WRITEWIN(62,27,0,WRITE_Center,RPort_PTR,4,_PT_Sprengen);
    if (TARGET_POSITION == UseShipPtr->Target)
    {
        WRITEWIN(62,49,0,WRITE_Center,RPort_PTR,4,_PT_Abloesen);
    } else {
        WRITEWIN(62,49,0,WRITE_Center,RPort_PTR,4,_PT_Position);
    }
    if (SHIPTYPE_STARGATE != MyShipPtr->SType)
    {
        WRITEWIN(62,71,0,WRITE_Center,RPort_PTR,4,_PT_Bewaessern);
    } else {
        WRITEWIN(62,71,0,WRITE_Center,RPort_PTR,4,_PT_Stargate);
    }
    WRITEWIN(62,93,0,WRITE_Center,RPort_PTR,4,_PT_Warten);
    do
    {
        delay(RDELAY);
        if (LMB_PRESSED)
        {
            if ((SYS_Window->MouseX>=0) && (SYS_Window->MouseX<=122))
            {
                if ((SYS_Window->MouseY>=3) && (SYS_Window->MouseY<=23))
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
                        delay(RDELAY);
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
                        l = -(l / ShipData(MyShipPtr->SType).MaxMove)-1;
                        if (l<-127) { l = -127; }
                        MyShipPtr->Moving = l;
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
                } else if ((SYS_Window->MouseY>=25) && (SYS_Window->MouseY<=45))
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
                    if ((x>=0) && (x<=480) && (y>=0) && (y<=480))
                    {
                        RECTWIN(MyRPort_PTR[0],0,x,y,x+31,y+31);
                        for(i = 0; i <= 15; i++)
                        {
                            BltBitMapRastPort((struct BitMap*) &ImgBitMap4,i*32,0,MyRPort_PTR[0],x,y,31,31,192);
                            delay(5);
                        }
                        RECTWIN(MyRPort_PTR[0],0,x,y,x+31,y+31);
                    }
                    delay(10);
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
                } else if ((SYS_Window->MouseY>=47) && (SYS_Window->MouseY<=67))
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
                } else if ((SYS_Window->MouseY>=69) && (SYS_Window->MouseY<=89))
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
							if (NULL == SYS_Window)
							{
								return;
							}
							RPort_PTR = SYS_Window->RPort;
							MAKEWINBORDER(RPort_PTR,0,0,370,86,12,6,1);
                            WRITEWIN(186,10,ActPlayerFlag,WRITE_Center,RPort_PTR,4,PText[481]);
                            WRITEWIN(186,32,ActPlayerFlag,WRITE_Center,RPort_PTR,4,PText[482]);
                            WRITEWIN(186,54,ActPlayerFlag,WRITE_Center,RPort_PTR,4,PText[483]);
                            WAITLOOP(false);
                        } else if (0 == SystemHeader[ActSys-1].FirstShip.SType)
                        {
							// install stargate
                            SystemHeader[ActSys-1].FirstShip.SType = TARGET_STARGATE;
                            SystemHeader[ActSys-1].FirstShip.PosX = MyShipPtr->PosX;
                            SystemHeader[ActSys-1].FirstShip.PosY = MyShipPtr->PosY;
                            MyShipPtr->Owner = 0;

							REFRESHDISPLAY();
                            delay(15);
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
