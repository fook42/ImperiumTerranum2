#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void MAIN_FNC()
{
    int     i, j = 1;
    uint32  l;
    UWORD   RawCode;
    char    s[60];
    char*   _s;
    char    Txt_notEnoughMem[] = {"Not enough memory available!\n"};

    BPTR    FHandle;
    struct MMD0 *SndModulePtr = 0;
    struct NewScreen  NeuScreen = {0,0,116,201,7,0,0,HIRES+LACE,CUSTOMSCREEN|SCREENQUIET,NULL,NULL,NULL,NULL};


    if (!INITLANG()) { return; }
    INITVARS();

    for (i = 0; i<FONTS; ++i)
    {
        if (OpenDiskFont(&(CustomTA[i])) == NULL)
        {
            DisplayBeep(NULL);
            Printf("Can't open font: \"");
            Printf(CustomTA[i].ta_Name);
            Printf("\" , size: %ld\n", CustomTA[i].ta_YSize);
            GAMEEXIT();
            return;
        }
    }
    /* read Paths.txt-File **********************************/
    if (!READPATHS())
    {
        GAMEEXIT();
        return;
    }
    CREATEPATHS();

    /************************************/
    // HelpID = 0xA0000+0x9004;
    INITSTDTAGS();
    XScreen = OpenScreenTagList(&NeuScreen, Tags);
    if (NULL == XScreen)
    {
        ScreenModeID_HighRes = 0x20000+0x9004; // PAL:HighRes Interlace (640x512)
        ScreenModeID_LowRes  = 0x20000+0x1000; // PAL:LowRes (320x256)
        INITSTDTAGS();
        XScreen = OpenScreenTagList(&NeuScreen, Tags);
    }
    if (NULL == XScreen)
    {
        puts("Can`t open screen!\n");
        GAMEEXIT();
        return;
    }
    for (i = 1; i<128; ++i) { SetRGB32(&(XScreen->ViewPort),i,0,0,0); };

    /* Start the Intro ******************/
    MAININTRO();
    /************************************/

    if (!OPENMAINSCREENS())
    {
        puts("Can`t open screens!\n");
        GAMEEXIT();
        return;
    }
/*    WBench = CloseWorkBench();
*/
    ScreenToFront(XScreen);
    // Alloc 29184 (76*96*4) + 28672 (64*64*7) + 122880 (80*192*8) Bytes.
    if ((!FillITBitMap(&ImgBitMap4, 76, 96, 4)) ||
        (!FillITBitMap(&ImgBitMap7, 64, 64, 7)) ||
        (!FillITBitMap(&ImgBitMap8, 80,192, 8)))
    {
        puts(Txt_notEnoughMem);
        GAMEEXIT();
        return;
    }

    IMemL[0] = 287000;
    IMemA[0] = (uint8*) AllocMem(IMemL[0], MEMF_CHIP | MEMF_CLEAR);
    if (NULL == IMemA[0])
    {
        puts(Txt_notEnoughMem);
        GAMEEXIT();
        return;
    }

    ZeroSound = (UWORD*) AllocMem(8, MEMF_CHIP | MEMF_CLEAR);
    if (NULL == ZeroSound)
    {
        puts(Txt_notEnoughMem);
        GAMEEXIT();
        return;
    }
    INITSOUNDS();
    i = 0;
/*    if (!WBench)
    {
        SetRGB4(MyVPort_PTR[0],1,15,15,15);
        WRITE_RP0(320,100,1,WRITE_Center,3,PText[831]);
        WRITE_RP0(320,130,1,WRITE_Center,3,PText[832]);
        ScreenToFront(MyScreen[0]);
        WAITLOOP(false);
        SetRast(MyRPort_PTR[0], 0);
    }
*/
    SWITCHDISPLAY();
    if (!INITDESK(1))
    {
        puts("Intuition-Error\n");
        GAMEEXIT();
        return;
    }

    i = FIRSTMENU();

    if (!INITSTARS())
    {
        puts("initstars error\n");
        GAMEEXIT();
        return;
    }
    DRAWSTARS(MODE_REDRAW);
    ScreenToFront(MyScreen[0]);

    if (2 == i)
    {
        if (!DISKMENU(DISKMENU_LOADGAME))
        {
            puts("loadgame error\n");
            GAMEEXIT();
            return;
        }
    } else {
        STARTROTATEPLANETS();
    }

    // if in Multiplayer mode,
    // the first ships (Wostok, Mir, Spaceshuttle) can move further (3 instead of 1/2)
    if (MultiPlayer)
    {
        ShipData[0].MaxMove = 3;
        ShipData[1].MaxMove = 3;
        ShipData[2].MaxMove = 3;
    }

    /*********************************************
     * Main loop
     *
     *********************************************/
    do
    {
        Delay(RDELAY);
        CLOCK();
        ScreenToFront(MyScreen[0]);
        RawCode = GETRAWCODE();

        if ((Save.PlayMySelf) || (Save.CivPlayer[ActPlayer-1] == 0) || (!Informed)
                || ((!MultiPlayer) && (!Informed) && (Year%10 != 0)))
        {
            STARTROTATEPLANETS();
        } else {
            if (LMB_PRESSED || RMB_PRESSED)
            {
                Delay(3);
            }
            if ((LMB_PRESSED && (MouseX(0) >= 518) && (MouseX(0) <= 634)
                             && (MouseY(0) >= 472) && (MouseY(0) <= 492))
                || (LMB_PRESSED && RMB_PRESSED)
                || ((64 == RawCode) || (67 == RawCode) || (68 == RawCode)))
            {
                KLICKGAD(518,472);
                STARTROTATEPLANETS();
            }
            if (LMB_PRESSED && (Save.CivPlayer[ActPlayer-1] != 0))
            {
                if ((MouseX(0) >= 0) && (MouseX(0) <= 512)
                        && (MouseY(0) >= 0) && (MouseY(0) <= 512))
                {
                    PLAYSOUND(0,300);
                    if ((SystemFlags[ActPlayer-1][LastSystem-1] & FLAG_KNOWN) == FLAG_KNOWN)
                    {
                        HANDLESYSTEM(&LastSystem,NULL);
                    }
                } else if ((MouseX(0) >= 518) && (MouseX(0) <= 634))
                {
                    if ((MouseY(0) >= 444) && (MouseY(0) <= 464))
                    {
                        KLICKGAD(518,444);
                        if (!DISKMENU(DISKMENU_PROMPT))
                        {
                            GAMEEXIT();
                            return;
                        }
                    } else if ((MouseY(0) >= 416) && (MouseY(0) <= 436))
                    {
                        KLICKGAD(518,416);
                        REGIERUNG();
                    }
                }
            } else
            {
                WRITEGALAXYDATA(0,0);
            }
        }
        Bool_var = false;
        if (Save.WorldFlag == WFLAG_FIELD)
        {
            if (Save.Systems <= 1)
            {
                Bool_var = true;
            }
        } else {
            l = 0;
            for (i = 0; i < MAXCIVS; i++)
            {
                if ((Save.CivPlayer[i] != 0) && (i != (ActPlayer-1)))
                {
                    l += Save.Bevoelkerung[i];
                }
            }
            if (((Save.Bevoelkerung[0]+Save.Bevoelkerung[1]+Save.Bevoelkerung[2]+Save.Bevoelkerung[3]+
                  Save.Bevoelkerung[4]+Save.Bevoelkerung[5]+Save.Bevoelkerung[6]+Save.Bevoelkerung[7]-
                  Save.Bevoelkerung[ActPlayer-1]) <= 20)
                    && (0 == l))
            {
                Bool_var = true;
                if (MultiPlayer)
                {
                    ENDSEQUENZ(-3);
                    GAMEEXIT();
                    return;
                }
            }
            if (!MultiPlayer)
            {
                if (0 == Save.Bevoelkerung[0])
                {
                    ENDSEQUENZ(-1);
                    GAMEEXIT();
                    return;
                }

                for (i = 0; i < 7; ++i)
                {
                    if (Save.ProjectCosts[i][7] <= 0)
                    {
                        Bool_var = true;
                    }
                }
            }
            else
            {
                if ((Save.ProjectCosts[ActPlayer-1][7] <= 0) && (Save.WarState[ActPlayer-1][ActPlayer-1] != LEVEL_DIED))
                {
                    Bool_var = true;

                    for (i = 0; i < (MAXCIVS-2); ++i)
                    {
                        if ((0 != Save.CivPlayer[i]) && (10 < Save.Bevoelkerung[i]) && (i != (ActPlayer-1)))
                        {
                            Bool_var = false;
                        }
                    }
                    if (0 != Save.CivPlayer[ActPlayer-1])
                    {
                        _s=my_strcpy(s, PathStr[4]);
                        (void) my_strcpy(_s, "MOD.HappyEnd");
                        FHandle = OPENSMOOTH(s,MODE_OLDFILE, NULL);
                        if (0 != FHandle)
                        {
                            Close(FHandle);
                            StopPlayer();
                            FreePlayer();
                            SndModulePtr = LoadModule(s);
                            if (GETMIDIPLAYER(SndModulePtr))
                            {
                                PlayModule(SndModulePtr);
                            }
                        }

                        // OpenWin
                        MAKEWINBORDER(MyRPort_PTR[0],40,100,470,190,12,6,0);
                        WRITE_RP0(256,113,ActPlayerFlag,WRITE_Center,3,PText[834]);
                        WRITE_RP0(256,137,ActPlayerFlag,WRITE_Center,3,PText[835]);
                        WRITE_RP0(256,161,ActPlayerFlag,WRITE_Center,3,PText[836]);
                        WAITLOOP(Save.PlayMySelf);
                        RECT_RP0_C0(40,100,470,190);
                        REFRESHDISPLAY();
                        // CloseWin

                        REMOVEPLAYER(ActPlayer-1);
                        DISPLAYSTATE();
                        PLAYERHIGHSCORE(ActPlayer);

                        for (i = 0; i < (MAXCIVS-1); ++i)
                        {
                            Save.WarState[ActPlayer-1][i] = LEVEL_DIED;
                            Save.WarState[i][ActPlayer-1] = LEVEL_DIED;
                        }

                        if (NULL != SndModulePtr)
                        {
                            StopPlayer();
                            UnLoadModule(SndModulePtr);
                            FreePlayer();
                        }

                        for (i = 0; i < MAXCIVS; ++i)
                        {
                            Save.ImperatorState[i] -= 500;
                        }

                        if (   (Save.WorldFlag != WFLAG_DCON)
                            && (Save.WorldFlag != WFLAG_CEBORC)
                            && (Save.WorldFlag != WFLAG_JAHADR))
                        {
                            Save.ImperatorState[7] = 1000;
                        }

                        if (Bool_var)
                        {
                            GAMEEXIT();
                            return;
                        }
                    } else {
                        Bool_var = false;
                        for (i = 0; i < MAXCIVS; i++)
                        {
                            if (Save.ProjectCosts[i][5] <= 0)
                            {
                                Bool_var = true;
                            }
                        }
                        if (!Bool_var)
                        {
                            ENDSEQUENZ(-2);
                        } else {
                            ENDSEQUENZ(1);
                            PLAYERHIGHSCORE(ActPlayer);
                        }
                        GAMEEXIT();
                        return;
                    }
                }
            }
        }
        CLEARINTUITION();
    }
    while (!Bool_var);

    Screen2 = 0;
    if ((Save.WorldFlag == WFLAG_FIELD) && (Save.Systems <= 1))
    {
        ENDSEQUENZ(-1);
    } else if (Bool_var)
    {
        if (!MultiPlayer)
        {
            for (i = 0; i < (MAXCIVS-1); ++i)
            {
                if (0 >= Save.ProjectCosts[i][7])
                {
                    j = i+1;
                    if ((MAXCIVS-2) == i)
                    {
                        while (2400 < Save.ImperatorState[(MAXCIVS-2)])
                        {
                            Save.ImperatorState[(MAXCIVS-2)] -= rand()%250;
                        }
                    }
                    if ((i == 0) && (Save.ProjectCosts[0][5]>0))
                    {
                        ENDSEQUENZ(-2);
                        GAMEEXIT();
                        return;
                    }
                    ENDSEQUENZ(j);
                    DISPLAYSTATE();
                    if (i < (MAXCIVS-2))
                    {
                        Save.ImperatorState[i] += it_round(Save.ImperatorState[i]* (Level/30.0));
                        if (Year>0)
                        {
                            Save.ImperatorState[ActPlayer-1] -= Year;
                        }
                    }
                }
            }

            if ( (Save.Bevoelkerung[1]+Save.Bevoelkerung[2]+Save.Bevoelkerung[3]+Save.Bevoelkerung[4]
                        +Save.Bevoelkerung[5]+Save.Bevoelkerung[6]+Save.Bevoelkerung[7]) <= 20)
            {
                ENDSEQUENZ(-3);
                DISPLAYSTATE();
                Save.ImperatorState[ActPlayer-1] += it_round(Save.ImperatorState[ActPlayer-1]* (Level/30.0));
                if (Year>0)
                {
                    Save.ImperatorState[ActPlayer-1] -= Year;
                }
                j = 1;
            }
            PLAYSOUND(0,300);
            PLAYERHIGHSCORE(j);
        }
    }
    GAMEEXIT();
}
