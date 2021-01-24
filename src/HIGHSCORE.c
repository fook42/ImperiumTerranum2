#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

uint8 _GETCOLOR[]={122,122,123,124,2,125,126,11};

void CREATEHIGHSCORE(char* HScoreFileName)
{
    BPTR    FHandle;
    uint8   i;
    uint32  points;

    FHandle = Open((CONST_STRPTR) HScoreFileName,MODE_NEWFILE);
    if (0 == FHandle)
    {
        ScreenToFront(MyScreen[0]);
        return;
    }
    points = 20000;
    for (i = 0; i < 8; ++i)
    {
        HiScore.Points[i] = points;
        points -= 2150;
        HiScore.CivVar[i] = i+1;
        if (7 == i) { HiScore.CivVar[i] = 1; }
    }
    (void) my_strcpy(HiScore.Player[0], "Oxygenic");
    (void) my_strcpy(HiScore.Player[1], "Kha`thak");
    (void) my_strcpy(HiScore.Player[2], "Tomalak");
    (void) my_strcpy(HiScore.Player[3], "Megalith");
    (void) my_strcpy(HiScore.Player[4], "Monolith");
    (void) my_strcpy(HiScore.Player[5], "The One");
    (void) my_strcpy(HiScore.Player[6], "Dark Blitter");
    (void) my_strcpy(HiScore.Player[7], "CyberTrace");
    (void) Write(FHandle, &HiScore, sizeof(r_HiScore));
    Close(FHandle);
}

uint8 GETCOLOR(uint8 Col)
{
    if ((Col>0) && (Col<(sizeof(_GETCOLOR)/sizeof(_GETCOLOR[0]))))
    {
        return _GETCOLOR[Col];
    } else {
        return _GETCOLOR[0];
    }
}

void HIGHSCORE()
{
    BPTR    FHandle;
    uint8   i, col;
    uint16  ypos;
    char    s[40];

    SWITCHDISPLAY();
    INITSCREEN(SCREEN_HISCORE);
    WRITE_RP1(210,60,027,(WRITE_Center|WRITE_Shadow),4,"Imperium Terranum Highscores");
    (void) my_strcpy(my_strcpy(s, PathStr[4]), "HiScore.dat");
    FHandle = OPENSMOOTH(s,MODE_OLDFILE);
    if (0 == FHandle)
    {
        CREATEHIGHSCORE(s);
        FHandle = OPENSMOOTH(s,MODE_OLDFILE);
    }
    if (0 != FHandle)
    {
        (void) Read(FHandle, &HiScore, sizeof(r_HiScore));
        Close(FHandle);
        ypos = 130;
        for(i = 0; i < 8; ++i)
        {
            col = GETCOLOR(HiScore.CivVar[i]);
            (void) dez2out( HiScore.Points[i], 0, s);
            WRITE_RP1(115,ypos,col,(WRITE_Right|WRITE_Shadow), 4, s);
            WRITE_RP1(135,ypos,col,             WRITE_Shadow , 4, HiScore.Player[i]);
            ypos += 30;
        }
        ScreenToFront(MyScreen[1]);
        do
        {
            Delay(RDELAY);
        }
        while (LMB_NOTPRESSED && RMB_NOTPRESSED);
        PLAYSOUND(0,300);
        do
        {
            Delay(RDELAY);
        }
        while (LMB_PRESSED && RMB_PRESSED && (!Bool_var));
    }
    ScreenToBack(MyScreen[1]);
}
