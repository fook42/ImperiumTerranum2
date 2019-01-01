#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void MAIN_FNC()
{
    int     i, j = 1;
    uint32  l;
    uint8   RawCode;
    char    s[60];
    char    Txt_notEnoughMem[] = {"Not enough memory available!\n"};

    BPTR    FHandle;
    struct MMD0 *SndModulePtr = 0;
    struct NewScreen  NeuScreen = {0,0,116,201,7,0,0,HIRES+LACE,CUSTOMSCREEN+SCREENQUIET,NULL,NULL,NULL,NULL};


    if (!INITLANG()) { return; }
    INITVARS();

    for (i = 0; i<FONTS; i++)
    {
        if (OpenDiskFont(&(CustomTA[i])) == NULL)
        {
            DisplayBeep(NULL);
            // Printf("Can't open font: \"");
            // Printf(CustomTA[i].ta_Name);
            // Printf("\" , size: %ld\n", CustomTA[i].ta_YSize);
        }
    }
    /* read Paths.txt-File **********************************/
    strcpy(s, "Paths.txt");
    FHandle = Open((CONST_STRPTR) s, MODE_OLDFILE);
    if (0 == FHandle)
    {
        // Printf("Kann Datei \"");
        // Printf(s);
        // Printf("\" nicht finden!\n");
        // Printf("Can`t find file \"");
        // Printf(s);
        // Printf("\" !\n");
        return;
    }
    (void)     Seek(FHandle, 0, OFFSET_END);
    PathMemL = Seek(FHandle, 0, OFFSET_BEGINNING);
    PathMemA = AllocMem(PathMemL, MEMF_CLEAR);
    if (NULL == PathMemA)
    {
        puts("Nicht genug Speicher vorhanden!\n");
        puts(Txt_notEnoughMem);
        Close(FHandle);
        return;
    }
    (void) Read(FHandle, PathMemA, PathMemL);
    Close(FHandle);
    CREATEPATHS();

    /************************************/


    HelpID = 0xA0000;
    INITSTDTAGS();
    XScreen = OpenScreenTagList(&NeuScreen, Tags);
    if (NULL == XScreen)
    {
        HelpID = 0x20000;
        INITSTDTAGS();
        XScreen = OpenScreenTagList(&NeuScreen, Tags);
    }
    if (NULL == XScreen)
    {
        puts("Can`t open screen!\n");
        GAMEEXIT();
        return;
    }
    for (i = 1; i<=127; i++) { SetRGB4(&(XScreen->ViewPort),i,0,0,0); };

    //    INITCHANNELS();

    /* Start the Intro ******************/
    MAININTRO();
    /************************************/

    if (!OPENMAINSCREENS())
    {
        GAMEEXIT();
        puts("Can`t open screens!\n");
        return;
    }
    //    WBench = CloseWorkBench();
    ScreenToFront(XScreen);
    if ((!FillITBitMap(&ImgBitMap4, 76, 96, 4)) ||
        (!FillITBitMap(&ImgBitMap7, 64, 64, 7)) ||
        (!FillITBitMap(&ImgBitMap8, 80,192, 8)))
    {
        puts(Txt_notEnoughMem);
        GAMEEXIT();
        return;
    }

    IMemL[0] = 287000;
    IMemA[0] = AllocMem(IMemL[0], MEMF_CHIP+MEMF_CLEAR);
    if (NULL == IMemA[0])
    {
        puts(Txt_notEnoughMem);
        GAMEEXIT();
        return;
    }

    ZeroSound = AllocMem(8, MEMF_CHIP+MEMF_CLEAR);
    if (NULL == ZeroSound)
    {
        puts(Txt_notEnoughMem);
        GAMEEXIT();
        return;
    }
    //    INITCHANNELS();
    INITSOUNDS();
    i = 0;
    /*    if (!WBench)
    {
        WRITE(320,100,1,WRITE_Center,MyScreen[0],4,PText[831]);
        SetRGB4(&(MyScreen[0]->ViewPort),1,15,15,15);
        WRITE(320,130,1,WRITE_Center,MyScreen[0],4,PText[832]);
        ScreenToFront(MyScreen[0]);
        WAITLOOP(false);
        RECT(MyScreen[0],0,0,0,639,511);
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
    DRAWSTARS(MODE_REDRAW,ActPlayer);
    ScreenToFront(MyScreen[0]);

    if (2 == i)
    {
        (void) DISKMENU(DISKMENU_LOADGAME);
    } else {
        STARTROTATEPLANETS();
    }

    /*********************************************
     * Main loop
     *
     *********************************************/
    do
    {
        delay(RDELAY);
        CLOCK();
        ScreenToFront(MyScreen[0]);
        RawCode = GETRAWCODE();
        if (MultiPlayer)
        {
            ShipData(8).MaxMove = 3;
            ShipData(9).MaxMove = 3;
            ShipData(10).MaxMove = 3;
        }

        if ((Save.PlayMySelf) || (Save.CivPlayer[ActPlayer-1] == 0) || (!Informed)
                || ((!MultiPlayer) && (!Informed) && (Year%10 != 0)))
        {
            STARTROTATEPLANETS();
        } else {
            if (LMB_PRESSED || RMB_PRESSED)
            {
                delay(3);
            }
            if ((LMB_PRESSED && (MouseX(0) >= 518) && (MouseX(0) <= 634)
                             && (MouseY(0) >= 472) && (MouseY(0) <= 492))
                || (LMB_PRESSED && RMB_PRESSED)
                || ((RawCode == 64) || (RawCode == 67) || (RawCode == 68)))
            {
                KLICKGAD(518,472);
                STARTROTATEPLANETS();
            }
            if (LMB_PRESSED && (Save.CivPlayer[ActPlayer-1] != 0))
            {
                if ((MouseX(0) >= 0) && (MouseX(0) <= 512)
                        && (MouseY(0) >= 0) && (MouseY(0) <= 512))
                {
                    PLAYSOUND(1,300);
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

                if ((Save.ProjectCosts[0].data[7] <= 0) ||
                    (Save.ProjectCosts[1].data[7] <= 0) ||
                    (Save.ProjectCosts[2].data[7] <= 0) ||
                    (Save.ProjectCosts[3].data[7] <= 0) ||
                    (Save.ProjectCosts[4].data[7] <= 0) ||
                    (Save.ProjectCosts[5].data[7] <= 0) ||
                    (Save.ProjectCosts[6].data[7] <= 0))
                {
                    Bool_var = true;
                }
            }
            else
            {
                if ((Save.ProjectCosts[ActPlayer-1].data[7] <= 0) && (Save.WarState[ActPlayer-1][ActPlayer-1] != LEVEL_DIED))
                {
                    Bool_var = true;

                    for (i = 0; i < (MAXCIVS-2); i++)
                    {
                        if ((Save.CivPlayer[i] != 0) && (Save.Bevoelkerung[i]>10) && (i != (ActPlayer-1)))
                        {
                            Bool_var = false;
                        }
                    }
                    if (Save.CivPlayer[ActPlayer-1] != 0)
                    {
                        strcpy(s, PathStr[4]);
                        strcat(s, "MOD.HappyEnd");
                        FHandle = OPENSMOOTH(s,MODE_OLDFILE);
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

                        MAKEBORDER(MyScreen[0],40,100,470,190,12,6,0);
                        WRITE(256,113,ActPlayerFlag,WRITE_Center,MyScreen[0],4,PText[834]);
                        WRITE(256,137,ActPlayerFlag,WRITE_Center,MyScreen[0],4,PText[835]);
                        WRITE(256,161,ActPlayerFlag,WRITE_Center,MyScreen[0],4,PText[836]);
                        WAITLOOP(Save.PlayMySelf);
                        RECT(MyScreen[0],0,40,100,470,190);
                        REFRESHDISPLAY();
                        REMOVEPLAYER(ActPlayer);
                        DISPLAYSTATE();
                        PLAYERHIGHSCORE(ActPlayer);

                        for (i = 0; i < (MAXCIVS-1); i++)
                        {
                            Save.WarState[ActPlayer-1][i] = LEVEL_DIED;
                            Save.WarState[i][ActPlayer-1] = LEVEL_DIED;
                        }

                        if (NULL != SndModulePtr)
                        {
                            StopPlayer();
                            UnLoadModule(SndModulePtr);
                        }

                        for (i = 0; i < MAXCIVS; i++)
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
                            if (Save.ProjectCosts[i].data[5] <= 0)
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
            for (i = 0; i < (MAXCIVS-1); i++)
            {
                if (Save.ProjectCosts[i].data[7] <= 0)
                {
                    j = i+1;
                    if (i == (MAXCIVS-2))
                    {
                        while (Save.ImperatorState[i] > 2400)
                        {
                            Save.ImperatorState[i] -= rand()%250;
                        }
                    }
                    if ((i == 0) && (Save.ProjectCosts[0].data[5]>0))
                    {
                        ENDSEQUENZ(-2);
                        GAMEEXIT();
                        return;
                    }
                    ENDSEQUENZ(j);
                    DISPLAYSTATE();
                    if (i < (MAXCIVS-2))
                    {
                        Save.ImperatorState[i] = it_round(Save.ImperatorState[i]* (1+Level/30.0));
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
                Save.ImperatorState[ActPlayer-1] = it_round(Save.ImperatorState[ActPlayer-1]* (1+Level/30.0));
                if (Year>0)
                {
                    Save.ImperatorState[ActPlayer-1] -= Year;
                }
                j = 1;
            }
            PLAYSOUND(1,300);
            PLAYERHIGHSCORE(j);
        }
    }
    GAMEEXIT();
}
