#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

uint8 _GETCOLOR[]={122,122,123,124,2,125,126,11};

void CREATEHIGHSCORE()
{
    BPTR    FHandle;
    uint8   i;
    char    s[30];

    strcpy(s, PathStr[4]);
    strcat(s, "HiScore.dat");
    FHandle = Open((CONST_STRPTR) s,MODE_NEWFILE);
    if (0 == FHandle)
    {
        ScreenToFront(MyScreen[0]);
        return;
    }
    for (i = 0; i<8; i++)
    {
        HiScore.Points[i] = 2800+(8-i)*2150;
        HiScore.CivVar[i] = i+1;
        if (7 == i) { HiScore.CivVar[i] = 1; }
    }
    strcpy(HiScore.Player[0], "Oxygenic");
    strcpy(HiScore.Player[1], "Kha`thak");
    strcpy(HiScore.Player[2], "Tomalak");
    strcpy(HiScore.Player[3], "Megalith");
    strcpy(HiScore.Player[4], "Monolith");
    strcpy(HiScore.Player[5], "The One");
    strcpy(HiScore.Player[6], "Dark Blitter");
    strcpy(HiScore.Player[7], "CyberTrace");
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
    char    s[30];

    SWITCHDISPLAY();
    INITSCREEN(SCREEN_HISCORE);
    WRITE(210,60,127,(WRITE_Center|WRITE_Shadow),MyScreen[1],5,"Imperium Terranum Highscores");
    strcpy(s, PathStr[4]);
    strcat(s, "HiScore.dat");
    FHandle = OPENSMOOTH(s,MODE_OLDFILE);
    if (0 == FHandle)
    {
        CREATEHIGHSCORE();
        FHandle = OPENSMOOTH(s,MODE_OLDFILE);
    }
    if (0 != FHandle)
    {
        (void) Read(FHandle, &HiScore, sizeof(r_HiScore));
        Close(FHandle);
        for(i = 0; i < 8; i++)
        {
            col = GETCOLOR(HiScore.CivVar[i]);
            (void) dez2out( HiScore.Points[i], 0, s);
            WRITE(115,130+i*30,col,(WRITE_Right|WRITE_Shadow), MyScreen[1], 5, s);
            WRITE(135,130+i*30,col,WRITE_Shadow, MyScreen[1], 5, HiScore.Player[i]);
        }
        ScreenToFront(MyScreen[1]);
        do
        {
            delay(RDELAY);
        }
        while (LMB_NOTPRESSED && RMB_NOTPRESSED);
        PLAYSOUND(1,300);
        do
        {
            delay(RDELAY);
        }
        while (LMB_PRESSED && RMB_PRESSED && (!Bool_var));
    }
    ScreenToBack(MyScreen[1]);
}
