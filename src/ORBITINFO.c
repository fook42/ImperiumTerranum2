#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

int DRAWSHIPS(int i, char (*ShipNames)[15], r_ShipHeader* MyShipPtr)
{
    int     j;
    char    s[14];
    char*   pos;
    RECT_RP0_C0(22,69,416,456);
    for(j = 0; j < 12; j++) { ShipNames[j][0] = 0; }
    j = 0;
    do
    {
        if (SHIPTYPE_FLEET == MyShipPtr->SType)
        {
            DisplayBeep(NULL);
        } else {
            if (0 != MyShipPtr->Owner)
            {
                BltBitMapRastPort((struct BitMap*) &ImgBitMap4,(MyShipPtr->SType-8)*32,32,MyRPort_PTR[0],35,37+i*32,32,32,192);
                WRITE_RP0(72,45+i*32,12,0,3,Project.data[MyShipPtr->SType]);

                strcpy(ShipNames[j], Project.data[MyShipPtr->SType]);

                pos = dez2out(((MyShipPtr->Ladung & MASK_SIEDLER)>>4), 2, s);
                *pos++=' ';
                pos = dez2out((MyShipPtr->Ladung & MASK_LTRUPPS), 2, pos);
                *pos++=' ';
                pos = dez2out(MyShipPtr->ShieldBonus, 2, pos);
                *pos++=' ';
                pos = dez2out(it_round((MyShipPtr->Shield + MyShipPtr->Tactical*3.0)/ShipData(MyShipPtr->SType).MaxShield*100.0), 3, pos);
                *pos++='%'; *pos=0;
                WRITE_RP0(235,45+i*32,8,0,1,s);
                i++;
                j++;
            }
        }
        MyShipPtr = MyShipPtr->NextShip;
    }
    while ((NULL != MyShipPtr) && (i<=12));
    return i;
}

void SETFLEETPOSITION(uint8 ActSys, r_ShipHeader* StShipPtr, r_ShipHeader* MyShipPtr)
{
    MyShipPtr->PosX   = StShipPtr->PosX;
    MyShipPtr->PosY   = StShipPtr->PosY;
    MyShipPtr->Moving = StShipPtr->Moving+1;
    MyShipPtr->Target = 0;
    while (FINDOBJECT(ActSys, 256+(MyShipPtr->PosX+OffsetX)*32, 256+(MyShipPtr->PosY+OffsetY)*32, MyShipPtr))
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
    int     i, j, k;
    int     ypos;
    char    ShipNames[12][15];

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
// OpenWindow()
    MAKEBORDER(MyScreen[0],20,30,420,480,12,6,0);
    WRITE_RP0(63,37,ActPlayerFlag,0,3,ReqText);
    WRITE_RP0( 36,56,12,0,0,PText[408]);
    WRITE_RP0(232,56,12,0,0,PText[409]);

    i = DRAWSHIPS(1, ShipNames, MyShipPtr);
    if ((i>12) && (NULL != MyShipPtr))
    {
//         MoreThanShown = true;
        DrawImage(MyRPort_PTR[0],&GadImg1,300,457);
        WRITE_RP0(335,460,0,0,3,PText[410]);
//     } else {
//         MoreThanShown = false;
    }
    b = false;
    SelShip = 1;
    do
    {
        Delay(RDELAY);
        if ((MouseX(0)>=60) && (MouseX(0)<=370))
        {
            k = (MouseY(0)-35) >> 5;
            if (k != SelShip)
            {
                ypos = 77;
                for(j = 0; j < 12; j++)
                {
                    if ((j+1) != k)
                    {
                        WRITE_RP0(72,ypos,           12,0,3,ShipNames[j]);
                    } else {
                        WRITE_RP0(72,ypos,ActPlayerFlag,0,3,ShipNames[j]);
                    }
                    ypos += 32;
                }
                SelShip = k;
            }
        }
        if (LMB_PRESSED)
        {
            if ((MouseX(0)>=300) && (MouseX(0)<=416)
              &&(MouseY(0)>=457) && (MouseY(0)<=477))
            {
                KLICKGAD(300,457);
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
                    i = i-12;
                    ++ShipFactor;
                }
                i = DRAWSHIPS(i, ShipNames, MyShipPtr);
            } else if ((MouseX(0)>=60) && (MouseX(0)<=370))
            {
                PLAYSOUND(1,300);
                SelShip += ShipFactor*12;
                b = true;
            }
        }
    }
    while ((!b) && RMB_NOTPRESSED);
    if (RMB_PRESSED) { PLAYSOUND(1,300); }
    RECT_RP0_C0(20,30,422,482);
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
