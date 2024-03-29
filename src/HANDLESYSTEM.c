char _Txt_nextPage[40];

#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"


//void DRAWSHIPS(uint8 CivFlag, uint8 stSys)
uint8 HANDLESYSTEM_DRAWSHIPS(sint8 Mode, uint8 stSys, uint8* PSys, r_ShipHeader** ShipPos)
{
    uint8   i;
    int     y, z;
    char    s[60];
    char*   _s;
    r_ShipHeader* MyShipPtr;

    RECT_RP0_C0(0,0,511,511);
    y = 0;
    z = 0;
    for(i =  1; i < 36; ++i) { PSys[i] = 0; }
    for(i = 36; i < 38; ++i) { PSys[i] = 1; }

    for (i = stSys; i <= Save.Systems; ++i)
    {
        MyShipPtr = SystemHeader[i-1].FirstShip.NextShip;
        while (NULL != MyShipPtr)
        {
            if (ActPlayerFlag == MyShipPtr->Owner)
            {
                if (((-1 == Mode) && (TARGET_POSITION == MyShipPtr->Target)) ||
                    ((-2 == Mode) && (SHIPFLAG_WATER  == MyShipPtr->Flags) && (0 <= MyShipPtr->Moving)) ||
                    ((-3 == Mode) && (0 > MyShipPtr->Moving)))
                {
                    ++z;
                    if (36 == z)
                    {
                        // LastSys = i;
                        WRITE_RP0(100,497,12,0,2, _Txt_nextPage);
                        return i;
                    }
                    ShipPos[z] = MyShipPtr;
                    PSys[z] = i;

                    if (SHIPTYPE_FLEET == MyShipPtr->SType)
                    {
                        WRITE_RP0(40,y,ActPlayerFlag,0,2, _PT_Flotte);
                    } else {
                        WRITE_RP0(40,y,ActPlayerFlag,0,2, ProjectName[MyShipPtr->SType]);
                        if (-3 == Mode) {
                            _s = my_strcpy(s, Save.SystemName[MyShipPtr->Source-1]);
                            *_s++ = ' ';
                            *_s++ = '-';
                            *_s++ = '>';
                            *_s++ = ' ';
                            _s = my_strcpy(_s, Save.SystemName[MyShipPtr->Target-1]);
                            *_s++ = ' ';
                            *_s++ = '(';
                            _s = dez2out(-MyShipPtr->Moving,0,_s);
                            *_s++ = ' ';
                            _s = my_strcpy(_s, _PT_Jahre);
                            *_s++ = ')';
                            *_s = 0;
                            WRITE_RP0(135,y,12,0,2,s);
                        } else {
                            _s=my_strcpy(s, PText[456]);
                            *_s++ = ':';
                            *_s++ = ' ';
                            (void) dez2out(((MyShipPtr->Ladung & MASK_SIEDLER)>>4), 0, _s);
                            WRITE_RP0(135,y,12,0,2,s);

                            _s=my_strcpy(s, PText[457]);
                            *_s++ = ':';
                            *_s++ = ' ';
                            (void) dez2out(MyShipPtr->Ladung & MASK_LTRUPPS, 0, _s);
                            WRITE_RP0(230,y,12,0,2,s);

                            _s=my_strcpy(s, PText[459]);
                            *_s++ = ':';
                            *_s++ = ' ';
                            _s = dez2out(it_round(MyShipPtr->Fracht / ShipData(MyShipPtr->SType).MaxLoad*100.0), 0, _s);
                            *_s++ = ' ';
                            *_s++ = '%';
                            *_s = 0;
                            WRITE_RP0(365,y,12,0,2,s);
                        }
                    }
                    y = y+14;
                }
            }
            MyShipPtr = MyShipPtr->NextShip;
        }
    }
    //    LastSys = 1;
    WRITE_RP0(100,497,12,0,2,_Txt_nextPage);
    return 1;
}

// displays a list of known planets when searched
uint8 DRAWPLANETS(uint8 CivFlag, uint8 stSys, uint8* PSys, uint8* PNum, uint8* PCol)
{
    uint8   i,j;
    uint16  y,z;
    r_PlanetHeader* MyPlanet;
    char    s[60];
    char*   _s;

    RECT_RP0_C0(0,0,511,511);
    y = 0;
    z = 0;
    for(i =  1; i < 36; ++i) { PSys[i] = 0; }
    for(i = 36; i < 38; ++i) { PSys[i] = 1; }

    for (i = (stSys-1); i< Save.Systems; ++i)
    {
        if ((SystemFlags[ActPlayer-1][i] & FLAG_KNOWN) == FLAG_KNOWN)
        {
            for(j = 0; j < SystemHeader[i].Planets; ++j)
            {
                MyPlanet = &(SystemHeader[i].PlanetMemA[j]);

                if ((((MyPlanet->PFlags & FLAG_CIV_MASK) == CivFlag)       && (0 != CivFlag)) ||
                    (((MyPlanet->PFlags & FLAG_CIV_MASK) != ActPlayerFlag) && (0 == CivFlag) && (0 < MyPlanet->PFlags)))
                {
                    ++z;
                    if (36 == z)
                    {
                        WRITE_RP0(100,497,12,0,2, _Txt_nextPage);
                        return (i+1);
                    }
                    PSys[z] = i+1;
                    PNum[z] = j+1;
                    PCol[z] = MyPlanet->PFlags & FLAG_CIV_MASK;
                    if (PCol[z] == 0) { PCol[z] = 1; }
                    WRITE_RP0(50,y,PCol[z],0,2,MyPlanet->PName);
                    if (CivFlag == ActPlayerFlag)
                    {
                        switch (MyPlanet->ProjectID) {
                            case PROJECT_CLEAR_BIOPHERE:  _s = PText[163]; break;
                            case PROJECT_REPAIR_INFRA:    _s = PText[164]; break;
                            case PROJECT_REPAIR_INDUSTRY: _s = PText[165]; break;
                            case PROJECT_NONE:            _s = _Txt_Separator; break;
                            default:                      _s = ProjectName[MyPlanet->ProjectID];
                        }
                        WRITE_RP0(170,y,12,0,2, _s);

                        _s = dez2out(MyPlanet->Population, 0, s);
                        (void) my_strcpy(_s, " Mio");
                        WRITE_RP0(465,y,12,WRITE_Right,2,s);
                    } else {
                        _s=my_strcpy(s, _PT_Groesse);
                        *_s++ = ':';
                        *_s++ = ' ';
                        (void) float2out( (MyPlanet->Size/10.0), 0, 2, _s);
                        WRITE_RP0(170,y,12,0,2,s);

                        if (MyPlanet->Ethno == ActPlayerFlag)
                        {
                            _s=my_strcpy(s, GETCIVADJ(ActPlayer));
                            *_s++ = ' ';
                            (void) my_strcpy(_s, PText[182]);
                            WRITE_RP0(350,y,12,0,2,s);
                        }
                    }
                    y = y+14;
                }
            }
        }
    }
    WRITE_RP0(100,497,12,0,2,_Txt_nextPage);
    return 1;
}

void SEARCHOBJECT(uint8* ActSys)
{
    sint8   Mode = 0;
    bool    b = false;
    r_ShipHeader* MyShipPtr;
    r_ShipHeader* ShipPos[38];
    uint8   PSys[38], PNum[38], PCol[38];
    uint8   LastSys;
    uint8   ThisP,LastP;
    r_PlanetHeader* MyPlanet;
    char*   _s;
    int     i;
    int     ypos;
    struct Window*   SEO_Window;
    struct RastPort* RPort_PTR;

    ThisP = 1;
    LastP = 0;
    LastSys = 1;
    _s=my_strcpy(_Txt_nextPage, _PT_Naechste_Seite);
    (void) my_strcpy(_s, "      >>>");

    SEO_Window=MAKEWINDOWBORDER(194,119,123,158,MyScreen[0]);
    RPort_PTR = SEO_Window->RPort;


    ypos = 3;
    for(i = 0; i < 6; i++)
    {
        ypos += 22;
        if (2 == i) { continue; }
        DrawImage(RPort_PTR, &GadImg1, 4, ypos);
    }
    WRITE(61,  5,12,WRITE_Center,RPort_PTR,3,_PT_Planeten);
    WRITE(61, 27, 0,WRITE_Center,RPort_PTR,3,_PT_eigene);
    WRITE(61, 49, 0,WRITE_Center,RPort_PTR,3,_PT_fremde);

    WRITE(61, 71,12,WRITE_Center,RPort_PTR,3,_PT_Schiffe);
    WRITE(61, 93, 0,WRITE_Center,RPort_PTR,3,_PT_Position);
    WRITE(61,115, 0,WRITE_Center,RPort_PTR,3,_PT_Bewaessern);
    WRITE(61,137, 0,WRITE_Center,RPort_PTR,3, PText[587]);

    while (LMB_PRESSED) { };

    do
    {
        Delay(RDELAY);
        if (LMB_PRESSED)
        {
            if ((SEO_Window->MouseX>5) && (SEO_Window->MouseX<121))
            {
                if ((SEO_Window->MouseY>24) && (SEO_Window->MouseY<46))
                {
                    KLICKWINGAD(RPort_PTR,4,25);
                    Mode = (sint8) ActPlayerFlag;   // own planets
                    b = true;
                } else if ((SEO_Window->MouseY>46) && (SEO_Window->MouseY<68))
                {
                    KLICKWINGAD(RPort_PTR,4,47);
                    Mode = 0;                       // foreign planets
                    b = true;
                } else if ((SEO_Window->MouseY>90) && (SEO_Window->MouseY<112))
                {
                    KLICKWINGAD(RPort_PTR,4,91);
                    Mode = -1;                      // fixed ships
                    b = true;
                } else if ((SEO_Window->MouseY>112) && (SEO_Window->MouseY<134))
                {
                    KLICKWINGAD(RPort_PTR,4,113);
                    Mode = -2;                      // watering ships
                    b = true;
                } else if ((SEO_Window->MouseY>134) && (SEO_Window->MouseY<156))
                {
                    KLICKWINGAD(RPort_PTR,4,135);
                    Mode = -3;                      // traveling ships
                    b = true;
                }
            }
            while (LMB_PRESSED) { };
        }
    }
    while ((!b) && RMB_NOTPRESSED);
    CloseWindow(SEO_Window);
    if (RMB_PRESSED)
    {
        PLAYSOUND(0,300);
        return;
    }
    b = false;
    if (0 > Mode)
    {
        // search ships ---
        do
        {
            Delay(RDELAY);
            LastSys = HANDLESYSTEM_DRAWSHIPS(Mode, 1, PSys, ShipPos);
            do
            {
                Delay(RDELAY);
                do
                {
                    Delay(RDELAY);
                    ThisP = (MouseY(0) / 14)+1;
                    if ((PSys[ThisP] != 0) && (ThisP != LastP)
                        && (MouseX(0)>=0) && (MouseX(0)<=511))
                    {
                        if ((LastP<36) && (PSys[LastP] != 0))
                        {
                            MyShipPtr = ShipPos[LastP];
                            if (SHIPTYPE_FLEET == MyShipPtr->SType)
                            {
                                _s = _PT_Flotte;
                            } else {
                                _s = ProjectName[MyShipPtr->SType];
                            }
                            WRITE_RP0(40,(LastP-1)*14,ActPlayerFlag,1,2, _s);
                        } else {
                            WRITE_RP0(100,497,12,1,2,_Txt_nextPage);
                        }
                        if (ThisP<36)
                        {
                            MyShipPtr = ShipPos[ThisP];
                            if (SHIPTYPE_FLEET == MyShipPtr->SType)
                            {
                                _s = _PT_Flotte;
                            } else {
                                _s = ProjectName[MyShipPtr->SType];
                            }
                            WRITE_RP0(40,(ThisP-1)*14,ActPlayerFlag,5,2, _s);
                        } else {
                            WRITE_RP0(100,497,12,5,2,_Txt_nextPage);
                        }
                        LastP = ThisP;
                    } else {
                        ThisP = LastP;
                    }
                }
                while(RMB_NOTPRESSED && LMB_NOTPRESSED);
                if (LMB_PRESSED)
                {
                    PLAYSOUND(0,300);
                    if (ThisP >= 36)
                    {
                        LastSys = HANDLESYSTEM_DRAWSHIPS(Mode, LastSys, PSys, ShipPos);
                    } else {
                        if (0 != PSys[ThisP])
                        {
                            if ((SystemFlags[ActPlayer-1][PSys[ThisP]-1] & FLAG_KNOWN) == FLAG_KNOWN)
                            {
                                OffsetX = -ShipPos[ThisP]->PosX-1;
                                OffsetY = -ShipPos[ThisP]->PosY-1;
                                *ActSys = PSys[ThisP];
                            }
                        }
                        DRAWSYSTEM(MODE_REDRAW,*ActSys,NULL);
                        b = true;
                    }
                }
            }
            while (RMB_NOTPRESSED && (!b));
        }
        while (RMB_NOTPRESSED && (!b));
    } else {
        // search planets ---
//        do
//        {
            Delay(RDELAY);
            LastSys = DRAWPLANETS(Mode, 1, PSys, PNum, PCol);
            do
            {
                Delay(RDELAY);
                do
                {
                    Delay(RDELAY);
                    ThisP = (MouseY(0) / 14)+1;
                    if ((0 != PSys[ThisP]) && (ThisP != LastP)
                        && (MouseX(0)>=0) && (MouseX(0)<=511))
                    {
                        if ((36 > LastP) && (0 != PSys[LastP]))
                        {
                            MyPlanet = &(SystemHeader[PSys[LastP]-1].PlanetMemA[PNum[LastP]-1]);
                            WRITE_RP0(50,(LastP-1)*14,PCol[LastP],1,2,MyPlanet->PName);
                        } else {
                            WRITE_RP0(100,497,12,1,2,_Txt_nextPage);
                        }
                        if (36 > ThisP)
                        {
                            MyPlanet = &(SystemHeader[PSys[ThisP]-1].PlanetMemA[PNum[ThisP]-1]);
                            WRITE_RP0(50,(ThisP-1)*14,PCol[ThisP],5,2,MyPlanet->PName);
                        } else {
                            WRITE_RP0(100,497,12,5,2,_Txt_nextPage);
                        }
                        LastP = ThisP;
                    } else {
                        ThisP = LastP;
                    }
                }
                while(RMB_NOTPRESSED && LMB_NOTPRESSED);
                if (LMB_PRESSED)
                {
                    PLAYSOUND(0,300);
                    if (37 < ThisP)
                    {
                        LastSys = DRAWPLANETS(Mode, LastSys, PSys, PNum, PCol);
                    } else {
                        if (0 != PSys[ThisP])
                        {
                            OffsetX = -it_round(MyPlanet->PosX)-1;
                            OffsetY = -it_round(MyPlanet->PosY)-1;
                            *ActSys = PSys[ThisP];
                        }
                        DRAWSYSTEM(MODE_REDRAW,*ActSys,NULL);
                        b = true;
                    }
                }
            }
            while (RMB_NOTPRESSED && (!b));
//        }
//        while (RMB_NOTPRESSED && (!b));
    }
    if (RMB_PRESSED)
    {
        PLAYSOUND(0,300);
        DRAWSYSTEM(MODE_REDRAW,*ActSys,NULL);
    }
}


void HANDLESYSTEM(uint8* ActSys, r_ShipHeader* ShipPtr)
{
    r_ShipHeader*   MyShipPtr;
    r_PlanetHeader* MyPlanetHeader;
    char    s[60];
    char*   _s;
    UWORD   RawCode;

    OffsetX = 0;
    OffsetY = 0;
    if (Display != *ActSys)
    {
        DRAWSYSTEM(MODE_REDRAW,*ActSys,NULL);
    }

    do
    {
        Delay(RDELAY);
        ScreenToFront(MyScreen[0]);
        CLOCK();
        RawCode = GETRAWCODE();
        if ((Save.PlayMySelf) || (Save.CivPlayer[ActPlayer-1] == 0) || (!Informed)
          || ((!MultiPlayer) && (!Informed) && ((Year % 10) != 0)))
        {
            STARTROTATEPLANETS();
        } else {
            if (LMB_PRESSED || RMB_PRESSED) { Delay(3); }
            if ((LMB_PRESSED && (MouseX(0)>=518) && (MouseX(0)<=634)
                             && (MouseY(0)>=472) && (MouseY(0)<=492))
             || (LMB_PRESSED && RMB_PRESSED) || (64 == RawCode) || (67 == RawCode) || (68 == RawCode))
            {
                KLICKGAD(518,472);
                STARTROTATEPLANETS();
            }

            if (LMB_PRESSED && (!Save.PlayMySelf))
            {
                if ((MouseX(0)>=518) && (MouseX(0)<=634))   // click to gadget-area
                {
                    if ((MouseY(0)>8) && (MouseY(0)<118)) // click to mini map
                    {
                        PLAYSOUND(0,300);
                        OffsetX = 576-MouseX(0);
                        OffsetY =  63-MouseY(0);
                        DRAWSYSTEM(MODE_REDRAW,*ActSys,NULL);
                    } else if ((MouseY(0)>=416) && (MouseY(0)<=436))    // click to "search"
                    {
                        KLICKGAD(518,416);
                        SEARCHOBJECT(ActSys);
                    }
                } else if ((MouseX(0)>=0) && (MouseX(0)<=511)
                        && (MouseY(0)>=0) && (MouseY(0)<=511))      // click to main view
                {
                    PLAYSOUND(0,300);
                    if (FINDOBJECT(*ActSys-1, MouseX(0)-256, MouseY(0)-256, NULL))
                    {
                        switch (ObjType) {
                            case TYPE_PLANET: PLANETINFO(*ActSys, (r_PlanetHeader*) ObjPtr); break;
                            case TYPE_SHIP: {
                                                MyShipPtr = (r_ShipHeader*) ObjPtr;
                                                if (MyShipPtr->SType == SHIPTYPE_FLEET)
                                                {
                                                    ORBITINFO(MyShipPtr, _PT_Flotte, *ActSys,0,0);
                                                } else {
                                                    SHIPINFO(*ActSys, MyShipPtr);
                                                }
                                            } break;
                            default: { }
                        }
                    }
                }
            } else if (RMB_PRESSED)
            {
                if ((MouseX(0)>=518) && (MouseX(0)<=634)
                 && (MouseY(0)>=  9) && (MouseY(0)<=117))           // click to mini map
                {
                    if ((MouseY(0)>=  9) && (MouseY(0)<= 29))
                    {
                        if (OffsetY<42)  { OffsetY += 2; }
                    }

                    if ((MouseY(0)>= 97) && (MouseY(0)<=117))
                    {
                        if (OffsetY>-42) { OffsetY -= 2; }
                    }

                    if ((MouseX(0)>=518) && (MouseX(0)<=538))
                    {
                        if (OffsetX<42)  { OffsetX += 2; }
                    }

                    if ((MouseX(0)>=614) && (MouseX(0)<=634))
                    {
                        if (OffsetX>-42) { OffsetX -= 2; }
                    }
                    DRAWSYSTEM(MODE_REDRAW,*ActSys,NULL);
                }
                if ((MouseX(0)>=0) && (MouseX(0)<=511)
                 && (MouseY(0)>=0) && (MouseY(0)<=511))             // click to main view
                {
                    PLAYSOUND(0,300);
                    if (FINDOBJECT(*ActSys-1, MouseX(0)-256, MouseY(0)-256, NULL))
                    {
                        switch (ObjType) {
                            case TYPE_PLANET: {
                                                MyPlanetHeader = (r_PlanetHeader*) ObjPtr;
                                                _s=my_strcpy(s, PText[645]);
                                                *_s++ = ' ';
                                                (void) my_strcpy(_s, MyPlanetHeader->PName);
                                                ORBITINFO(MyPlanetHeader->FirstShip.NextShip, s, *ActSys, it_round(MyPlanetHeader->PosX), it_round(MyPlanetHeader->PosY));
                                            } break;
                            case TYPE_SHIP: SYSTEMINFO(*ActSys, (r_ShipHeader*) ObjPtr); break;
                            default: { }
                        }
                    }
                }
            }
        }
        CLEARINTUITION();
    }
    while ((0 != Display) && (0 != (SystemFlags[ActPlayer-1][*ActSys-1] & FLAG_KNOWN))
        && (LMB_NOTPRESSED || (MouseX(0)<518) || (MouseX(0)>634)
                           || (MouseY(0)<444) || (MouseY(0)>464))
        && (!Save.PlayMySelf));

    if (0 != Display)
    {
        KLICKGAD(518,444);
        RECT_RP0_C0(522,9,629,117);
        DRAWSTARS(MODE_REDRAW);
    }
}
