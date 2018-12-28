#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

#define ACTVERSION  (0x0004)


void LOADSHIPS(r_ShipHeader* ShipPtr, BPTR FHandle)
{
    r_ShipHeader* ActShipPtr;
    r_ShipHeader* BeforeShipPtr;
    r_ShipHeader* FleetShipPtr;
    r_ShipHeader* BFleetShipPtr;
    APTR    memory;
    bool    FirstFShip;

    BeforeShipPtr = ShipPtr;
    ActShipPtr = ShipPtr->NextShip;
    while (NULL != ActShipPtr)
    {
        memory = AllocMem(sizeof(r_ShipHeader), MEMF_ANY);
        if (NULL == memory)
        {
            BeforeShipPtr->NextShip = NULL;
            ActShipPtr = NULL;
            return;
        }
        (void) Read(FHandle, memory, sizeof(r_ShipHeader));
        ActShipPtr = (r_ShipHeader*) memory;
        BeforeShipPtr->NextShip = ActShipPtr;
        ActShipPtr->BeforeShip = BeforeShipPtr;
        BeforeShipPtr = ActShipPtr;
        if ((SHIPTYPE_FLEET == ActShipPtr->SType)
                   && (NULL != ActShipPtr->TargetShip))
        {
            FirstFShip = true;
            do
            {
                memory = AllocMem(sizeof(r_ShipHeader), MEMF_ANY);
                if (NULL == memory)
                {
                    ActShipPtr->TargetShip = NULL;
                    ActShipPtr->SType = 8;
                    return;
                }
                (void) Read(FHandle, memory, sizeof(r_ShipHeader));
                FleetShipPtr = (r_ShipHeader*) memory;
                if (FirstFShip)
                {
                    FleetShipPtr->BeforeShip = ActShipPtr;
                    ActShipPtr->TargetShip = FleetShipPtr;
                } else {
                    BFleetShipPtr->NextShip = FleetShipPtr;
                    FleetShipPtr->BeforeShip = BFleetShipPtr;
                }
                BFleetShipPtr = FleetShipPtr;
                FleetShipPtr = FleetShipPtr->NextShip;
                FirstFShip = false;
            }
            while (NULL != FleetShipPtr);
        }
        ActShipPtr = ActShipPtr->NextShip;
    }
}

void SAVESHIPS(r_ShipHeader* ShipPtr, BPTR FHandle)
{
    r_ShipHeader*   ActShipPtr;
    r_ShipHeader*   FleetShipPtr;

    ActShipPtr = ShipPtr->NextShip;
    do
    {
        (void) Write(FHandle, ActShipPtr, sizeof(r_ShipHeader));
        if ((SHIPTYPE_FLEET == ActShipPtr->SType)
                   && (NULL != ActShipPtr->TargetShip))
        {
            FleetShipPtr = ActShipPtr->TargetShip;
            do
            {
                (void) Write(FHandle, FleetShipPtr, sizeof(r_ShipHeader));
                FleetShipPtr = FleetShipPtr->NextShip;
            }
            while (NULL != FleetShipPtr);
        }
        ActShipPtr = ActShipPtr->NextShip;
    }
    while (NULL != ActShipPtr);
}

/* problem: GETSAVENAME liefert string zurück.. nicht pointer...
* loesung: uebergabe ZielString-pointer (SName)
*/

void GETSAVENAME(char* Title, char* SName)
{
    char    ID[6][30];
    uint32  Y,Version;
    uint8   Lev;
    sint16  i;
    sint16  mouse_over, selected;
    int     path_len;
    char    s[40];
    char*   pos;
    int     stringlen;
    BPTR    FHandle;
    struct Window* GSN_Window;
    struct RastPort* RPort_PTR;

    SName[0] = 0;   // invalidate the "selected file"-string

    path_len = strlen(PathStr[8]);
    memcpy(s, PathStr[8], path_len+1);
    strcpy(s+path_len, "IMPT.ID");
    FHandle = OPENSMOOTH(s, MODE_OLDFILE);
    if (0 == FHandle)
    {
        return;
    } else {
        Close(FHandle);
    }
    strcpy(s+path_len, "IMPT.0");
    for(i = 0; i < 6; i++)
    {
        ID[i][0] = 0;
        s[strlen(s)-1] = i + '1';
        do
        {
            FHandle = Open((CONST_STRPTR) s ,MODE_OLDFILE);
            if (0 == FHandle)
            {
                strcpy(ID[i], PText[525]);      // = "leer"
            } else {
                (void) Read(FHandle, &Version, 4);  // game-version
                if (Version == ACTVERSION)
                {
                    (void) Read(FHandle, &Y, 4);    // year
                    (void) Read(FHandle, &Lev, 1);  // level

                    stringlen=strlen(_PT_Jahr);
                    memcpy(ID[i], _PT_Jahr, stringlen);
                    ID[i][stringlen++]=':';
                    ID[i][stringlen++]=' ';
                    pos = dez2out(Y, 4, ID[i]+stringlen);
                    strcpy(pos, "     Level: ");
                    stringlen = strlen(ID[i]);
                    (void)dez2out(Lev, 0, ID[i]+stringlen);

                    Close(FHandle);
                } else {
                    Close(FHandle);
                    (void) DeleteFile((CONST_STRPTR) s);
                }
            }
        }
        while (ID[i][0] == 0);
    }

    GSN_Window=MAKEWINDOW(100,80,311,191,MyScreen[0]);
    if (NULL == GSN_Window)
    {
        return;
    }
    RPort_PTR = GSN_Window->RPort;
    MAKEWINBORDER(RPort_PTR,0,   0,310,190,12,6,2);
    MAKEWINBORDER(RPort_PTR,10, 30,300,145,6,12,0);
    MAKEWINBORDER(RPort_PTR,10,155,300,180,6,12,0);
    WRITEWIN(155,7,ActPlayerFlag,(WRITE_Center|WRITE_Shadow),RPort_PTR,4,Title);
    for(i = 0; i < 6; i++)
        { WRITEWIN(20,35+i*18,ActPlayerFlag,0,RPort_PTR,4,ID[i]); }
    selected = 99;
    do
    {
        mouse_over = (sint16) (((GSN_Window->MouseY)-35) / 18);
        if ((0 <= mouse_over) && (5 >= mouse_over))
        {
            if (selected != mouse_over)
            {
                for(i = 0; i < 6; i++)
                {
                    if (i == mouse_over)
                    {
                        WRITEWIN(20,35+i*18,           12,0,RPort_PTR,4,ID[i]);
                        RECTWIN(RPort_PTR,0,12,157,298,178);
                        WRITEWIN(20,160,12,1,RPort_PTR,4,ID[i]);
                        selected = mouse_over;
                    } else {
                        WRITEWIN(20,35+i*18,ActPlayerFlag,0,RPort_PTR,4,ID[i]);
                    }
                }
            }
        }
    }
    while ((0 > selected) || (5 < selected) || LMB_NOTPRESSED);
    PLAYSOUND(1,300);

    memcpy(SName, PathStr[8], path_len+1);
    strcpy(SName+path_len, "IMPT.0");
    SName[strlen(SName)-1] = selected + '1';
    CloseWindow(GSN_Window);
    return;
}

void ENCODEDATA()
{
    uint8   i;
    for(i = 0; i < MAXCIVS; i++)
    {
        Save.Staatstopf[i] = ((Save.Staatstopf[i] ^ 0x17031973)+0x13605185) ^ 0xFA5375AF;
    }
}

void DECODEDATA()
{
    uint8   i;
    for(i = 0; i < MAXCIVS; i++)
    {
        Save.Staatstopf[i] = ((Save.Staatstopf[i] ^ 0xFA5375AF)-0x13605185) ^ 0x17031973;
    }
}

void NOMEMMESSAGE()
{
    REQUEST(PText[526],PText[527],8,8);
}

bool DISKMENU(uint8 Autoselect)
{
    bool _DISKMENU = true;
    r_PlanetHeader* PlanetHeader;
    uint32  l;
    int     i, j, k;
    char    s[40];
    BPTR    FHandle;
    bool    leave_dialog = false;
    struct Window* DIS_Window;
    struct RastPort* RPort_PTR;

    DIS_Window=MAKEWINDOW(194,119,123,136,MyScreen[0]);
    if (NULL == DIS_Window)
    {
        return false;
    }
    RPort_PTR = DIS_Window->RPort;
    MAKEWINBORDER(RPort_PTR,0,0,122,135,12,6,1);

    for(i = 0; i < 6; i++)
    {
        DrawImage(RPort_PTR,&GadImg1,4,3+i*22);
    }
    WRITEWIN(61,  5,0,WRITE_Center,RPort_PTR,4,PText[529]);
    WRITEWIN(61, 27,0,WRITE_Center,RPort_PTR,4,PText[530]);
    WRITEWIN(61, 49,0,WRITE_Center,RPort_PTR,4,PText[531]);
    WRITEWIN(61, 71,0,WRITE_Center,RPort_PTR,4,PText[532]);
    WRITEWIN(61, 93,0,WRITE_Center,RPort_PTR,4,PText[533]);
    WRITEWIN(61,115,8,WRITE_Center,RPort_PTR,4,PText[534]);

    do
    {
        delay(RDELAY);
        if (LMB_PRESSED || (DISKMENU_PROMPT != Autoselect))
        {
            if (((DIS_Window->MouseX>=4) && (DIS_Window->MouseX<=120)) || (DISKMENU_PROMPT != Autoselect))
            {
                if (((DIS_Window->MouseY>=3) && (DIS_Window->MouseY<=23)) || (DISKMENU_LOADGAME == Autoselect))
                {
                /* ----------------- load an old savegame */
                    KLICKWINGAD(RPort_PTR,4,3);
                    leave_dialog = true;
                    GETSAVENAME(PText[536], s);
                    if (0 != s[0])
                    {
                        FHandle = OPENSMOOTH(s,MODE_OLDFILE);
                        if (0 != FHandle)
                        {
                            FREESYSTEMMEMORY();
                            (void) Read(FHandle, &l, 4);   // should be the ACTVERSION.. but we dont check here - was done in GETSAVENAME
                            (void) Read(FHandle, &Year, 4);
                            (void) Read(FHandle, &Level, 1);
                            (void) Read(FHandle, &ActPlayer, 1);
                            (void) Read(FHandle, &MyWormHole[0], MAXHOLES*sizeof(r_WormHole));
                            (void) Read(FHandle, &SystemX[0], MAXSYSTEMS*sizeof(SystemX[0]));
                            (void) Read(FHandle, &SystemY[0], MAXSYSTEMS*sizeof(SystemY[0]));
                            (void) Read(FHandle, &SystemFlags[0][0], MAXSYSTEMS*MAXCIVS*sizeof(SystemFlags[0][0]));
                            (void) Read(FHandle, &MaquesShips, 4);

                            for(i = 0; i < MAXSYSTEMS; i++)
                            {
                                (void) Read(FHandle, &SystemHeader[i],sizeof(r_SystemHeader));
                                if ((SystemHeader[i].PlanetMemA > 0) && (SystemHeader[i].Planets>0))
                                {
                                    if (NULL != SystemHeader[i].FirstShip.NextShip)
                                    {
                                        LOADSHIPS(&SystemHeader[i].FirstShip, FHandle);
                                    }
                                    SystemHeader[i].PlanetMemA = (r_PlanetHeader*) AllocMem(SystemHeader[i].Planets*sizeof(r_PlanetHeader), MEMF_ANY);
                                    if (NULL == SystemHeader[i].PlanetMemA)
                                    {
                                        NOMEMMESSAGE();
                                        Close(FHandle); // was missing...
                                        CloseWindow(DIS_Window);
                                        return _DISKMENU;
                                    }
                                    (void) Read(FHandle, SystemHeader[i].PlanetMemA, SystemHeader[i].Planets*sizeof(r_PlanetHeader));
                                    for(j = 0; j < SystemHeader[i].Planets; j++)
                                    {
                                        PlanetHeader = &(SystemHeader[i].PlanetMemA[j]);
                                        if (NULL != PlanetHeader->ProjectPtr)
                                        {
                                            l = (uint32) AllocMem(sizeof(ByteArr42), MEMF_CLEAR);
                                            if (0 == l)
                                            {
                                                NOMEMMESSAGE();
                                                for(k = 1; k <= SystemHeader[i].Planets; k++)
                                                {
                                                    if (NULL != PlanetHeader->ProjectPtr)
                                                    {
                                                        FreeMem((APTR) (PlanetHeader->ProjectPtr), sizeof(ByteArr42));
                                                    }
                                                    PlanetHeader->ProjectPtr = NULL;
                                                }
                                                Close(FHandle);
                                                CloseWindow(DIS_Window);
                                                return _DISKMENU;
                                            }
                                            PlanetHeader->ProjectPtr = (ByteArr42*) l;
                                            (void) Read(FHandle, (APTR) PlanetHeader->ProjectPtr, sizeof(ByteArr42));
                                        }
                                        if (NULL != PlanetHeader->FirstShip.NextShip)
                                        {
                                            LOADSHIPS(&PlanetHeader->FirstShip, FHandle);
                                        }
                                    }
                                }
                            }
                            (void) Read(FHandle, &Save, sizeof(r_Save));   // this is incompatible with the original version of IT2
                            DECODEDATA();
                            Close(FHandle);

                            SETWORLDCOLORS();
                            MultiPlayer = false;
                            for(i = 1; i < 7; i++)
                            {
                                if (Save.CivPlayer[i] != 0)
                                {
                                    MultiPlayer = true;
                                }
                            }
                            ActPlayerFlag = GETCIVFLAG(ActPlayer);
                            Informed = false;
                            CloseWindow(DIS_Window);
                            DIS_Window = NULL;
                            INFORMUSER();
                            DRAWSTARS(MODE_REDRAW, ActPlayer);
                        }
                    }
                } else if (((DIS_Window->MouseY>=25) && (DIS_Window->MouseY<=45)) || (DISKMENU_SAVEGAME == Autoselect))
                {
                /* ----------------- save the current game */
                    KLICKWINGAD(RPort_PTR,4,25);
                    leave_dialog = true;
                    GETSAVENAME(PText[537], s);
                    if (0 != s[0])
                    {
                        FHandle = OPENSMOOTH(s,MODE_NEWFILE);
                        if (0 != FHandle)
                        {
                            l = ACTVERSION;
                            (void) Write(FHandle, &l, 4);
                            (void) Write(FHandle, &Year, 4);
                            (void) Write(FHandle, &Level, 1);
                            (void) Write(FHandle, &ActPlayer, 1);
                            (void) Write(FHandle, &MyWormHole[0], MAXHOLES*sizeof(r_WormHole));
                            (void) Write(FHandle, &SystemX[0], MAXSYSTEMS*sizeof(SystemX[0]));
                            (void) Write(FHandle, &SystemY[0], MAXSYSTEMS*sizeof(SystemY[0]));
                            (void) Write(FHandle, &SystemFlags[0][0], MAXSYSTEMS*MAXCIVS*sizeof(SystemFlags[0][0]));
                            (void) Write(FHandle, &MaquesShips, 4);
                            for(i = 0; i < MAXSYSTEMS; i++)
                            {
                                (void) Write(FHandle, &SystemHeader[i],sizeof(r_SystemHeader));
                                if ((SystemHeader[i].PlanetMemA>0) && (SystemHeader[i].Planets>0))
                                {
                                    if (NULL != SystemHeader[i].FirstShip.NextShip)
                                        { SAVESHIPS(&SystemHeader[i].FirstShip, FHandle); }
                                    (void) Write(FHandle, SystemHeader[i].PlanetMemA, SystemHeader[i].Planets*sizeof(r_PlanetHeader));
                                    for(j = 0; j < SystemHeader[i].Planets; j++)
                                    {
                                        PlanetHeader = &(SystemHeader[i].PlanetMemA[j]);
                                        if (NULL != PlanetHeader->ProjectPtr)
                                            { (void) Write(FHandle, PlanetHeader->ProjectPtr, sizeof(ByteArr42)); }
                                        if (NULL != PlanetHeader->FirstShip.NextShip)
                                            { SAVESHIPS(&PlanetHeader->FirstShip, FHandle); }
                                    }
                                }
                            }
                            ENCODEDATA();
                            (void) Write(FHandle, &Save, sizeof(r_Save));   // this is incompatible with the original version of IT2
                            DECODEDATA();
                            Close(FHandle);
                        } else {
                            puts("No Savefile\n");
                        }
                    }
                } else if ((DIS_Window->MouseY>=47) && (DIS_Window->MouseY<=67))
                {
                    KLICKWINGAD(RPort_PTR,4,47);
                    GETSAVENAME(PText[538], s);
                    if (0 != s[0])
                    {
                        (void) DeleteFile((CONST_STRPTR) s);
                    }
                    leave_dialog = true;
                } else if ((DIS_Window->MouseY>=69) && (DIS_Window->MouseY<=89))
                {
                    KLICKWINGAD(RPort_PTR,4,69);
                    OPTIONMENU(0);
                    ScreenToFront(MyScreen[0]);
                } else if ((DIS_Window->MouseY>=91) && (DIS_Window->MouseY<=111))
                {
                    KLICKWINGAD(RPort_PTR,4,91);
                    HIGHSCORE();
                } else if ((DIS_Window->MouseY>=113) && (DIS_Window->MouseY<=133))
                {
                    KLICKWINGAD(RPort_PTR,4,113);
                    _DISKMENU = false;
                    leave_dialog = true;
                }
            }
        }
    }
    while ((!leave_dialog) && RMB_NOTPRESSED && (DISKMENU_PROMPT == Autoselect));
    if (RMB_PRESSED)
    {
        PLAYSOUND(1,300);
    }
    if (NULL != DIS_Window)
    {
        CloseWindow(DIS_Window);
    }
    return _DISKMENU;
}
