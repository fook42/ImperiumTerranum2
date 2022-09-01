#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

const uint8 _GETCOLOR[]={122,122,123,124,2,125,126,11};
const char* const def_HSNames[] = {
        "Oxygenic", "Kha`thak", "Tomalak", "Megalith",
        "Monolith", "The One", "Dark Blitter", "CyberTrace"};

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
        (void) my_strcpy(HiScore.Player[i], def_HSNames[i]);
        HiScore.CivVar[i] = i+1;
        HiScore.Points[i] = points;
        points -= 2150;
    }
    HiScore.CivVar[7] = 1;
    (void) Write(FHandle, &HiScore, sizeof(r_HiScore));
    Close(FHandle);
}

uint8 GETCOLOR(const int Col)
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
    uint8   col;
    int     i, ypos;
    char    s[40];

    SWITCHDISPLAY();
    INITSCREEN(SCREEN_HISCORE);
    WRITE_RP1(210,60,127,(WRITE_Center|WRITE_Shadow),4,"Imperium Terranum Highscores");
    (void) my_strcpy(my_strcpy(s, PathStr[4]), "HiScore.dat");
    FHandle = OPENSMOOTH(s,MODE_OLDFILE, NULL);
    if (0 == FHandle)
    {
        CREATEHIGHSCORE(s);
        FHandle = OPENSMOOTH(s,MODE_OLDFILE, NULL);
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
        while ((LMB_PRESSED || RMB_PRESSED) && (!Bool_var));
    }
    ScreenToBack(MyScreen[1]);
}
