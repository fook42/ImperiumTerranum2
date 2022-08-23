#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

int DRAWSHIPS(int i, char (*ShipNames)[15], r_ShipHeader* MyShipPtr, int max_ships, struct RastPort* RPort_PTR)
{
    int     j;
    char    s[14];
    char*   _s;
//    RECT_RP0_C0(22,69,Area_Width-96,Area_Height-56);
    for(j = 0; j < max_ships; j++) { ShipNames[j][0] = 0; }
    j = 0;
    do
    {
        if (SHIPTYPE_FLEET == MyShipPtr->SType)
        {
            DisplayBeep(NULL);
        } else {
            if (0 != MyShipPtr->Owner)
            {
                BltBitMapRastPort((struct BitMap*) &ImgBitMap4,(MyShipPtr->SType-8)*32,32,RPort_PTR,15,7+i*32,32,32,192);
                WRITE(52,15+i*32,12,0,RPort_PTR,3,ProjectName[MyShipPtr->SType]);

                (void) my_strcpy(ShipNames[j], ProjectName[MyShipPtr->SType]);

                _s = dez2out(((MyShipPtr->Ladung & MASK_SIEDLER)>>4), 2, s);
                *_s++ = ' ';
                _s = dez2out((MyShipPtr->Ladung & MASK_LTRUPPS), 2, _s);
                *_s++ = ' ';
                _s = dez2out(MyShipPtr->ShieldBonus, 2, _s);
                *_s++ = ' ';
                _s = dez2out(it_round((MyShipPtr->Shield + MyShipPtr->Tactical*3.0)/ShipData(MyShipPtr->SType).MaxShield*100.0), 3, _s);
                *_s++ = '%';
                *_s = 0;
                WRITE(215,15+i*32,8,0,RPort_PTR,1,s);
                i++;
                j++;
            }
        }
        MyShipPtr = MyShipPtr->NextShip;
    }
    while ((NULL != MyShipPtr) && (i<=max_ships));
    return i;
}

void SETFLEETPOSITION(uint8 ActSys, r_ShipHeader* StShipPtr, r_ShipHeader* MyShipPtr)
{
    MyShipPtr->PosX   = StShipPtr->PosX;
    MyShipPtr->PosY   = StShipPtr->PosY;
    MyShipPtr->Moving = StShipPtr->Moving+1;
    MyShipPtr->Target = 0;
    while (FINDOBJECT(ActSys, (MyShipPtr->PosX+OffsetX)<<5, (MyShipPtr->PosY+OffsetY)<<5, MyShipPtr))
    {
        switch (rand()%4) {
            case 0: ++MyShipPtr->PosX; break;
            case 1: --MyShipPtr->PosX; break;
            case 2: ++MyShipPtr->PosY; break;
            case 3: --MyShipPtr->PosY; break;
            default: { }
        }
    }
}

void ORBITINFO(r_ShipHeader* StShipPtr, char* ReqText, uint8 ActSys, sint8 XPosX, sint8 XPosY)
{
//     bool    MoreThanShown;
    bool    b, FleetUsed;
    uint8   ShipFactor = 0;
    sint16  SelShip;
    r_ShipHeader*   MyShipPtr;
    struct Window* OI_Window;
    struct RastPort* RPort_PTR;
    int     i, j, k;
    int     ypos;
    const int   max_num_ships = 12;
    char    ShipNames[max_num_ships][15];

    if (NULL == StShipPtr) { return; }
    MyShipPtr = StShipPtr;
    while ((0    == MyShipPtr->Owner)
        && (NULL != MyShipPtr->NextShip))
    {
        MyShipPtr = MyShipPtr->NextShip;
    }
    if (ActPlayerFlag  != MyShipPtr->Owner) { return; }
    if (SHIPTYPE_FLEET == MyShipPtr->SType)
    {
        MyShipPtr = MyShipPtr->TargetShip;
        FleetUsed = true;
    } else {
        FleetUsed = false;
    }

    OI_Window=MAKECENTERWINDOW(460,Area_Height-32,MyScreen[0]);
    if (NULL == OI_Window)
    {
        return;
    }
    RPort_PTR = OI_Window->RPort;

// OpenWindow()
//    MAKEBORDER(MyScreen[0],20,30,Area_Width-92,Area_Height-32,12,6,0);
    WRITE( 43, 7,ActPlayerFlag,0,RPort_PTR,3,ReqText);
    WRITE( 16,26,12,0,RPort_PTR,0,PText[408]);
    WRITE(212,26,12,0,RPort_PTR,0,PText[409]);

    i = DRAWSHIPS(1, ShipNames, MyShipPtr, max_num_ships, RPort_PTR);
    if ((i>max_num_ships) && (NULL != MyShipPtr))
    {
//         MoreThanShown = true;
        DrawImage(RPort_PTR,&GadImg1,280,Area_Height-55);
        WRITE(315,Area_Height-52,0,0,RPort_PTR,3,PText[410]);
//     } else {
//         MoreThanShown = false;
    }
    b = false;
    SelShip = 1;
    do
    {
        Delay(RDELAY);
        if ((OI_Window->MouseX>=40) && (OI_Window->MouseX<=350))
        {
            k = (OI_Window->MouseY-5)/32;
            if (k != SelShip)
            {
                ypos = 47;
                for(j = 0; j < max_num_ships; j++)
                {
                    if ((j+1) != k)
                    {
                        WRITE(52,ypos,           12,0,RPort_PTR,3,ShipNames[j]);
                    } else {
                        WRITE(52,ypos,ActPlayerFlag,0,RPort_PTR,3,ShipNames[j]);
                    }
                    ypos += 32;
                }
                SelShip = k;
            }
        }
        if (LMB_PRESSED)
        {
            if ((OI_Window->MouseX>=280) && (OI_Window->MouseX<=396)
              &&(OI_Window->MouseY>=(Area_Height-55)) && (OI_Window->MouseY<=(Area_Height-35)))
            {
                KLICKWINGAD(RPort_PTR,280,Area_Height-55);
                if (NULL == MyShipPtr)
                {
                    MyShipPtr = StShipPtr;
                    if (FleetUsed)
                    {
                        MyShipPtr = MyShipPtr->TargetShip;
                    }
                    i = 1;
                    ShipFactor = 0;
                } else {
                    i = i-max_num_ships;
                    ++ShipFactor;
                }
                i = DRAWSHIPS(i, ShipNames, MyShipPtr, max_num_ships, RPort_PTR);
            } else if ((OI_Window->MouseX>=40) && (OI_Window->MouseX<=350))
            {
                PLAYSOUND(0,300);
                SelShip += ShipFactor*max_num_ships;
                b = true;
            }
        }
    }
    while ((!b) && RMB_NOTPRESSED);
    if (RMB_PRESSED) { PLAYSOUND(0,300); }
    CloseWindow(OI_Window);
// CloseWindow()
    if (b)
    {
        MyShipPtr = StShipPtr;
        while ((0    == MyShipPtr->Owner)
            && (NULL != MyShipPtr->NextShip))
        {
            MyShipPtr = MyShipPtr->NextShip;
        }
        if (FleetUsed)
        {
            MyShipPtr = MyShipPtr->TargetShip;
        }
        i = 1;
        while ((i != SelShip) && (NULL != MyShipPtr))
        {
            if (0 != MyShipPtr->Owner)
                { i++; }
            MyShipPtr = MyShipPtr->NextShip;
        }
        if (NULL != MyShipPtr)
        {
            if (FleetUsed)
            {
                SETFLEETPOSITION(ActSys-1, StShipPtr, MyShipPtr);
                if (MyShipPtr == StShipPtr->TargetShip)
                {
                    StShipPtr->TargetShip = MyShipPtr->NextShip;
                    if (NULL != MyShipPtr->NextShip)
                    {
                        MyShipPtr->NextShip->BeforeShip = StShipPtr;
                    }
                    LINKSHIP(MyShipPtr, &SystemHeader[ActSys-1].FirstShip, 0);
                } else {
                    LINKSHIP(MyShipPtr, &SystemHeader[ActSys-1].FirstShip, 1);
                }
                if (NULL == StShipPtr->TargetShip->NextShip)
                {
                    StShipPtr->TargetShip->PosX = StShipPtr->PosX;
                    StShipPtr->TargetShip->PosY = StShipPtr->PosY;
                    LINKSHIP(StShipPtr->TargetShip, &SystemHeader[ActSys-1].FirstShip, 0);
                    StShipPtr->Owner = 0;
                    StShipPtr->SType = 8;
                }
            } else {
                MyShipPtr->PosX = XPosX;
                MyShipPtr->PosY = XPosY;
                if (FleetUsed)
                {
                    SETFLEETPOSITION(ActSys-1, StShipPtr, MyShipPtr);
                }
                LINKSHIP(MyShipPtr, &SystemHeader[ActSys-1].FirstShip, 1);
            }
            REFRESHDISPLAY();
            if (MyShipPtr->Moving>0)
            {
                DRAWSYSTEM(MODE_REFRESH,ActSys,MyShipPtr);
                MOVESHIP(ActSys,MyShipPtr,false);
            }
        }
    }
    REFRESHDISPLAY();
}
