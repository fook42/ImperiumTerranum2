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
    uint32  SYear, SVersion;
    uint8   SLevel;

    uint16  ypos;
    sint16  i;
    sint16  mouse_over, selected;
    volatile bool   mouse_b;
    volatile sint16 mouse_y;
    int     path_len;
    char    s[40];
    char*   pos;
    uint8   file_slen;
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
    file_slen=strlen(s)-1;
    for(i = 0; i < 6; ++i)
    {
        ID[i][0] = 0;
        s[file_slen] = i + '1';
        do
        {
            FHandle = Open((CONST_STRPTR) s ,MODE_OLDFILE);
            if (0 == FHandle)
            {
                strcpy(ID[i], PText[525]);      // = "leer"
            } else {
                (void) Read(FHandle, &SVersion, 4);   // game-version
                if (ACTVERSION == SVersion)
                {
                    (void) Read(FHandle, &SYear, 4);  // year
                    (void) Read(FHandle, &SLevel, 1); // level

                    stringlen=strlen(_PT_Jahr);
                    memcpy(ID[i], _PT_Jahr, stringlen);
                    ID[i][stringlen++]=':';
                    ID[i][stringlen++]=' ';
                    pos = dez2out(SYear, 4, ID[i]+stringlen);

                    strcpy(pos, "     Level: ");
                    stringlen = strlen(ID[i]);
                    (void)dez2out(SLevel, 0, ID[i]+stringlen);

                    Close(FHandle);
                } else {
                    Close(FHandle);
                    (void) DeleteFile((CONST_STRPTR) s);
                }
            }
        }
        while (0 == ID[i][0]);
    }

    GSN_Window=MAKEWINDOW(100,80,311,226,MyScreen[0]);
    if (NULL == GSN_Window)
    {
        return;
    }
    RPort_PTR = GSN_Window->RPort;
    MAKEWINBORDER(RPort_PTR,0,   0,310,225,12,6,2);
    MAKEWINBORDER(RPort_PTR,10, 30,300,145,6,12,0);
    MAKEWINBORDER(RPort_PTR,10,155,300,180,6,12,0);
    MAKEWINBORDER(RPort_PTR,10,190,300,215,12,6,1);

    WRITE(155,7,ActPlayerFlag,(WRITE_Center|WRITE_Shadow),RPort_PTR,3,Title);
    ypos = 35;
    for(i = 0; i < 6; ++i)
    {
        WRITE( 20, ypos, ActPlayerFlag, 0, RPort_PTR,3, ID[i]);
        ypos += 18;
    }
    WRITE(155,195,8,(WRITE_Center|WRITE_Shadow),RPort_PTR,3,_PT_ENDE);
    selected = 99;
    do
    {
        mouse_y = GSN_Window->MouseY;
        mouse_b = LMB_PRESSED;
        mouse_over = (sint16) ((mouse_y-35) / 18);
        if ((0 <= mouse_over) && (6 > mouse_over))
        {
            if (selected != mouse_over)
            {
                ypos = 35;
                for(i = 0; i < 6; ++i)
                {
                    if (i == mouse_over)
                    {
                        WRITE(20, ypos,           12,0,RPort_PTR,3,ID[i]);
                        RECTWIN(RPort_PTR,0,12,157,298,178);
                        WRITE(20,160,12,1,RPort_PTR,3,ID[i]);
                        selected = mouse_over;
                    } else {
                        WRITE(20, ypos,ActPlayerFlag,0,RPort_PTR,3,ID[i]);
                    }
                    ypos += 18;
                }
            }
        } else if ((mouse_y > 190) && (mouse_y < 215))
        {
            if (mouse_b)
            {
                MAKEWINBORDER(RPort_PTR,10,190,300,215,6,12,1);
                while (LMB_PRESSED) {};
                selected = -1;
            }
        }
    }
    while ((-1 > selected) || (5 < selected) || (!mouse_b));
    PLAYSOUND(1,300);

    if (-1 < selected)
    {
        memcpy(SName, PathStr[8], path_len+1);
        strcpy(SName+path_len, "IMPT.0");
        SName[strlen(SName)-1] = selected + '1';
    }
    CloseWindow(GSN_Window);
    return;
}

void ENCODEDATA()
{
    uint8   i;
    for(i = 0; i < MAXCIVS; ++i)
    {
        Save.Staatstopf[i] = ((Save.Staatstopf[i] ^ 0x17031973)+0x13605185) ^ 0xFA5375AF;
    }
}

void DECODEDATA()
{
    uint8   i;
    for(i = 0; i < MAXCIVS; ++i)
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
    uint16  ypos;
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

    ypos = 3;
    for(i = 0; i < 6; ++i)
    {
        DrawImage(RPort_PTR,&GadImg1,4, ypos);
        ypos += 22;
    }
    WRITE(61,  5,0,WRITE_Center,RPort_PTR,3,PText[529]);
    WRITE(61, 27,0,WRITE_Center,RPort_PTR,3,PText[530]);
    WRITE(61, 49,0,WRITE_Center,RPort_PTR,3,PText[531]);
    WRITE(61, 71,0,WRITE_Center,RPort_PTR,3,PText[532]);
    WRITE(61, 93,0,WRITE_Center,RPort_PTR,3,PText[533]);
    WRITE(61,115,8,WRITE_Center,RPort_PTR,3,PText[534]);

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
                            FREESYSTEMMEMORY(); // freemem of SystemHeader-stucts: projects, planetMem, ships.
                            (void) Read(FHandle, &l, 4);   // should be the ACTVERSION.. but we dont check here - was done in GETSAVENAME
                            (void) Read(FHandle, &Year, 4);
                            (void) Read(FHandle, &Level, 1);
                            (void) Read(FHandle, &ActPlayer, 1);
                            (void) Read(FHandle, &MyWormHole[0], MAXHOLES*sizeof(r_WormHole));
                            (void) Read(FHandle, &SystemX[0], MAXSYSTEMS*sizeof(SystemX[0]));
                            (void) Read(FHandle, &SystemY[0], MAXSYSTEMS*sizeof(SystemY[0]));
                            (void) Read(FHandle, &SystemFlags[0][0], MAXSYSTEMS*MAXCIVS*sizeof(SystemFlags[0][0]));
                            (void) Read(FHandle, &MaquesShips, 4);

                            for(i = 0; i < MAXSYSTEMS; ++i)
                            {
                                (void) Read(FHandle, &SystemHeader[i],sizeof(r_SystemHeader));
                                if ((NULL != SystemHeader[i].PlanetMemA) && (0 < SystemHeader[i].Planets))
                                {
                                    if (NULL != SystemHeader[i].FirstShip.NextShip)
                                    {
                                        LOADSHIPS(&SystemHeader[i].FirstShip, FHandle);
                                    }
                                    // PlanetMemA is set, but the memory is Free'd already.. so we need to reallocate it
                                    SystemHeader[i].PlanetMemA = (r_PlanetHeader*) AllocMem(SystemHeader[i].Planets*sizeof(r_PlanetHeader), MEMF_ANY);
                                    if (NULL == SystemHeader[i].PlanetMemA)
                                    {
                                        Close(FHandle); // was missing...
                                        CloseWindow(DIS_Window);
                                        NOMEMMESSAGE();
                                        return _DISKMENU;
                                    }
                                    (void) Read(FHandle, SystemHeader[i].PlanetMemA, SystemHeader[i].Planets*sizeof(r_PlanetHeader));
                                    for(j = 0; j < SystemHeader[i].Planets; ++j)
                                    {
                                        PlanetHeader = &(SystemHeader[i].PlanetMemA[j]);
                                        if (NULL != PlanetHeader->ProjectPtr)
                                        {
                                            l = (uint32) AllocMem(sizeof(ByteArr42), MEMF_CLEAR);
                                            if (0 == l)
                                            {
                                                Close(FHandle);
                                                CloseWindow(DIS_Window);
                                                for(k = 0; k < SystemHeader[i].Planets; ++k)
                                                {
                                                    if (NULL != PlanetHeader->ProjectPtr)
                                                    {
                                                        FreeMem((APTR) (PlanetHeader->ProjectPtr), sizeof(ByteArr42));
                                                    }
                                                    PlanetHeader->ProjectPtr = NULL;
                                                }
                                                NOMEMMESSAGE();
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
                            for(i = 1; i < 7; ++i)
                            {
                                if (0 != Save.CivPlayer[i])
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
                } else if (((DIS_Window->MouseY > 24) && (DIS_Window->MouseY < 46)) || (DISKMENU_SAVEGAME == Autoselect))
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
                            for(i = 0; i < MAXSYSTEMS; ++i)
                            {
                                (void) Write(FHandle, &SystemHeader[i],sizeof(r_SystemHeader));
                                if ((NULL != SystemHeader[i].PlanetMemA) && (0 < SystemHeader[i].Planets))
                                {
                                    if (NULL != SystemHeader[i].FirstShip.NextShip)
                                    {
                                        SAVESHIPS(&SystemHeader[i].FirstShip, FHandle);
                                    }
                                    (void) Write(FHandle, SystemHeader[i].PlanetMemA, SystemHeader[i].Planets*sizeof(r_PlanetHeader));
                                    for(j = 0; j < SystemHeader[i].Planets; ++j)
                                    {
                                        PlanetHeader = &(SystemHeader[i].PlanetMemA[j]);
                                        if (NULL != PlanetHeader->ProjectPtr)
                                        {
                                            (void) Write(FHandle, PlanetHeader->ProjectPtr, sizeof(ByteArr42));
                                        }
                                        if (NULL != PlanetHeader->FirstShip.NextShip)
                                        {
                                            SAVESHIPS(&PlanetHeader->FirstShip, FHandle);
                                        }
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
                } else if ((DIS_Window->MouseY > 46) && (DIS_Window->MouseY < 68))
                {
                    KLICKWINGAD(RPort_PTR,4,47);
                    GETSAVENAME(PText[538], s);
                    if (0 != s[0])
                    {
                        (void) DeleteFile((CONST_STRPTR) s);
                    }
                    leave_dialog = true;
                } else if ((DIS_Window->MouseY > 68) && (DIS_Window->MouseY < 90))
                {
                    KLICKWINGAD(RPort_PTR,4,69);
                    OPTIONMENU(0);
                    ScreenToFront(MyScreen[0]);
                } else if ((DIS_Window->MouseY > 90) && (DIS_Window->MouseY < 112))
                {
                    KLICKWINGAD(RPort_PTR,4,91);
                    HIGHSCORE();
                } else if ((DIS_Window->MouseY > 112) && (DIS_Window->MouseY < 134))
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
