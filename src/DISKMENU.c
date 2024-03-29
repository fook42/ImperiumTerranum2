#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

#define ACTVERSION  (0x0004)
#define LOAD_OKAY   (0)
#define LOAD_NOMEM  (128)
#define LOAD_NOFILE (255)

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
        if (NULL != memory)
        {
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
                        ActShipPtr->SType = 8;   // "Wostok" is used as default shiptype
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
        } else {
            BeforeShipPtr->NextShip = NULL;
            ActShipPtr = NULL;
        }
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
    char    s[40];
    char*   _s1;
    char*   _s2;
    BPTR    FHandle;
    struct Window* GSN_Window;
    struct RastPort* RPort_PTR;

    SName[0] = 0;   // invalidate the "selected file"-string

    _s1=my_strcpy(s, PathStr[8]);
    (void) my_strcpy(_s1, "IMPT.ID");
    FHandle = OPENSMOOTH(s, MODE_OLDFILE, NULL);
    if (0 == FHandle)
    {
        return;
    } else {
        Close(FHandle);
    }
    _s2=my_strcpy(_s1, "IMPT.0");
    _s2--;
    for(i = 0; i < 6; ++i)
    {
        ID[i][0] = 0;
        *_s2= i+'1';
        do
        {
            FHandle = Open((CONST_STRPTR) s ,MODE_OLDFILE);
            if (0 == FHandle)
            {
                (void) my_strcpy(ID[i], PText[525]);      // = "leer"
            } else {
                (void) Read(FHandle, &SVersion, 4);   // game-version
                if (ACTVERSION == SVersion)
                {
                    (void) Read(FHandle, &SYear, 4);  // year
                    (void) Read(FHandle, &SLevel, 1); // level

                    _s1=my_strcpy(ID[i], _PT_Jahr);
                    *_s1++=':';
                    *_s1++=' ';
                    _s1=dez2out(SYear, 4, _s1);
                    _s1=my_strcpy(_s1, "     Level: ");
                    (void) dez2out(SLevel, 0, _s1);

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
    PLAYSOUND(0,300);

    if (-1 < selected)
    {
        _s1=my_strcpy(my_strcpy(SName, PathStr[8]), "IMPT.");
        *_s1++ = selected + '1';
        *_s1 = 0;
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

uint8 DIS_LOADGAME()
{
    BPTR    FHandle;
    char    s[40];
    int     i, j;
    uint32  actVer;
    APTR    MemPtr;
    r_PlanetHeader* PlanetHeader;
    uint8   exitvalue = LOAD_NOFILE;

    GETSAVENAME(PText[536], s);
    if (0 != s[0])
    {
        FHandle = OPENSMOOTH(s,MODE_OLDFILE, NULL);
        if (0 != FHandle)
        {
            exitvalue = LOAD_OKAY;
            FREESYSTEMMEMORY(); // freemem of SystemHeader-stucts: projects, planetMem, ships.
            (void) Read(FHandle, &actVer, 4);   // should be the ACTVERSION.. but we dont check here - was done in GETSAVENAME
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
                (void) Read(FHandle, &SystemHeader[i], sizeof(r_SystemHeader));
                if ((NULL != SystemHeader[i].PlanetMemA) && (0 < SystemHeader[i].Planets))
                {
                    // load ships if any are stored ...
                    if (NULL != SystemHeader[i].FirstShip.NextShip)
                    {
                        LOADSHIPS(&(SystemHeader[i].FirstShip), FHandle);
                    }

                    // PlanetMemA was set in savegame, but the memory is not allocated.. so we need to do it
                    MemPtr = AllocMem(SystemHeader[i].Planets*sizeof(r_PlanetHeader), MEMF_ANY);
                    if (NULL != MemPtr)
                    {
                        (void) Read(FHandle, MemPtr, SystemHeader[i].Planets*sizeof(r_PlanetHeader));
                        SystemHeader[i].PlanetMemA = (r_PlanetHeader*) MemPtr;
                        for(j = 0; j < SystemHeader[i].Planets; ++j)
                        {
                            PlanetHeader = &(SystemHeader[i].PlanetMemA[j]);
                            if (NULL != PlanetHeader->ProjectPtr)
                            {
                                // in the saved structure an ProjectPtr-Address was set
                                // so we also need to allocate new memory for it - and read the data into this
                                MemPtr = AllocMem(sizeof(ByteArr42), MEMF_CLEAR);
                                if (NULL != MemPtr)
                                {
                                    PlanetHeader->ProjectPtr = (ByteArr42*) MemPtr;
                                    (void) Read(FHandle, (APTR) PlanetHeader->ProjectPtr, sizeof(ByteArr42));
                                }
                                else
                                {
                                    PlanetHeader->ProjectPtr = NULL;
                                    exitvalue = LOAD_NOMEM;
                                    goto leave_loop;
                                }
                            }
                            if (NULL != PlanetHeader->FirstShip.NextShip)
                            {
                                // ships were saved with this Planet.. so restore them
                                LOADSHIPS(&PlanetHeader->FirstShip, FHandle);
                            }
                        }
                    } else {
                        SystemHeader[i].PlanetMemA = NULL;
                        exitvalue = LOAD_NOMEM;
                        goto leave_loop;
                    }
                }
            }
            leave_loop:
            if (LOAD_NOMEM == exitvalue)
            {
                // free some memory.. as we dont get any more yet.
                FREESYSTEMMEMORY();
            } else if (LOAD_OKAY == exitvalue)
            {
                (void) Read(FHandle, &Save, sizeof(r_Save));   // this is incompatible with the original version of IT2
                DECODEDATA();
            
                SETWORLDCOLORS();
                MultiPlayer = false;
                for(i = 1; i < 7; ++i)
                {
                    if (0 != Save.CivPlayer[i])
                    {
                        MultiPlayer = true;
                        break;
                    }
                }
                ActPlayerFlag = GETCIVFLAG(ActPlayer);
            }
            Close(FHandle);
        }
    }
    return (exitvalue);
}

bool DIS_SAVEGAME()
{
    BPTR    FHandle;
    char    s[40];
    int     i, j;
    uint32  l;
    r_PlanetHeader* PlanetHeader;
    bool    exitvalue = FALSE;

    GETSAVENAME(PText[537], s);
    if (0 != s[0])
    {
        FHandle = OPENSMOOTH(s,MODE_NEWFILE, NULL);
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
            exitvalue = TRUE;
        } else {
            puts("No Savefile\n");
        }
    }
    return (exitvalue);
}

bool DIS_DELETEGAME()
{
    char    s[40];
    bool    exitvalue = FALSE;

    GETSAVENAME(PText[538], s);
    if (0 != s[0])
    {
        // @TODO ... nachfragen vor dem Löschen... !!!
        (void) DeleteFile((CONST_STRPTR) s);
        exitvalue = TRUE;
    }
    return (exitvalue);
}

/* DISKMENU displays the "MainMenu"/"Hauptmenue" */
bool DISKMENU(uint8 Autoselect)
{
    bool _DISKMENU = true;
    uint8   loadreturn;
    uint16  ypos;
    int     i;
    bool    leave_dialog = false;
    struct Window* DIS_Window;
    struct RastPort* RPort_PTR;
    const char* Button_Texts[] = {PText[529], PText[530], PText[531],
                                  PText[532], PText[533], PText[534]};
    int button_Textcol, button_Textstyle;

    DIS_Window=MAKEWINDOWBORDER(194,119,123,136,MyScreen[0]);
    if (NULL == DIS_Window)
    {
        return false;
    }
    RPort_PTR = DIS_Window->RPort;


    ypos = 3;
    button_Textcol = 0;
    button_Textstyle = WRITE_Center;
    for(i = 0; i < 6; ++i)
    {
        DrawImage(RPort_PTR,&GadImg1, 4, ypos);
        if (5 == i) { button_Textcol = 8; button_Textstyle |= WRITE_Shadow; }
        WRITE(61, ypos+2, button_Textcol, button_Textstyle, RPort_PTR, 3, Button_Texts[i]);
        ypos += 22;
    }

    do
    {
        Delay(RDELAY);
        if (LMB_PRESSED || (DISKMENU_PROMPT != Autoselect))
        {
            if (((DIS_Window->MouseX>3) && (DIS_Window->MouseX<121)) || (DISKMENU_PROMPT != Autoselect))
            {
                if (((DIS_Window->MouseY>2) && (DIS_Window->MouseY<24)) || (DISKMENU_LOADGAME == Autoselect))
                {
                    /* ----------------- load an old savegame */
                    KLICKWINGAD(RPort_PTR,4,3);
                    loadreturn = DIS_LOADGAME();
                    if (LOAD_OKAY == loadreturn)
                    {
                        // missing this : LastPlayer:=0;  (static variable inside INFORMUSER ... used to be global in pascal)
                        Informed = false;
                        CloseWindow(DIS_Window);
                        DIS_Window = NULL;
                        INFORMUSER();
                        DRAWSTARS(MODE_REDRAW);
                        leave_dialog = true;
                    } else if (LOAD_NOMEM == loadreturn)
                    {
                        CloseWindow(DIS_Window);
                        DIS_Window = NULL;
                        NOMEMMESSAGE();
                        _DISKMENU = false;
                        leave_dialog = true;
                    } else if ((LOAD_NOFILE == loadreturn) && (DISKMENU_LOADGAME == Autoselect))
                    {
                        /* if no file is loaded from the game-start what shall we do? start with a "new" game? */
                    }
                } else if (((DIS_Window->MouseY > 24) && (DIS_Window->MouseY < 46)) || (DISKMENU_SAVEGAME == Autoselect))
                {
                    /* ----------------- save the current game */
                    KLICKWINGAD(RPort_PTR,4,25);
                    if (DIS_SAVEGAME())
                    {
                        leave_dialog = true;
                    }
                } else if ((DIS_Window->MouseY > 46) && (DIS_Window->MouseY < 68))
                {
                    KLICKWINGAD(RPort_PTR,4,47);
                    if (DIS_DELETEGAME())
                    {
                        leave_dialog = true;
                    }
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
        PLAYSOUND(0,300);
    }
    if (NULL != DIS_Window)
    {
        CloseWindow(DIS_Window);
    }
    return _DISKMENU;
}
