#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void PLAYERHIGHSCORE(uint8 PHS_ActPlayer)
{
    char    s[50];
    BPTR    FHandle;
    uint8   i;
    uint32  ex_Points;
    uint8   ex_CivVar;

    strcpy(s, PathStr[4]);
    strcat(s, "HiScore.dat");

    FHandle = OPENSMOOTH(s, MODE_OLDFILE);
    if (0 == FHandle)
    {
        CREATEHIGHSCORE();
        FHandle = OPENSMOOTH(s, MODE_OLDFILE);
    }
    if (0 != FHandle)
    {
        (void) Read(FHandle, &HiScore, sizeof(r_HiScore));
        Close(FHandle);
        if (HiScore.Points[7] < Save.ImperatorState[PHS_ActPlayer-1])
        {
            FHandle = OPENSMOOTH(s, MODE_NEWFILE);
            HiScore.Points[7] = Save.ImperatorState[PHS_ActPlayer-1];
            HiScore.CivVar[7] = PHS_ActPlayer;
            if ((!Save.PlayMySelf) && (Save.CivPlayer[PHS_ActPlayer-1] != 0))
            {
                GETPLAYERNAME(PHS_ActPlayer, HiScore.Player[7]);
            } else {
                strcpy(HiScore.Player[7], GETCIVNAME(PHS_ActPlayer+1));
            }
            for (i = 7; i>=1; i--)
            {
                if (HiScore.Points[i] > HiScore.Points[i-1])
                {
                    ex_Points          =HiScore.Points[i];
                    HiScore.Points[i]  =HiScore.Points[i-1];
                    HiScore.Points[i-1]=ex_Points;

                    ex_CivVar          =HiScore.CivVar[i];
                    HiScore.CivVar[i]  =HiScore.CivVar[i-1];
                    HiScore.CivVar[i-1]=ex_CivVar;

                    strcpy(s,                 HiScore.Player[i]);
                    strcpy(HiScore.Player[i], HiScore.Player[i-1]);
                    strcpy(HiScore.Player[i-1],s);
                }
            }
            (void) Seek(FHandle,0,OFFSET_BEGINNING);
            (void) Write(FHandle, &HiScore, sizeof(r_HiScore));
            Close(FHandle);
        }
        HIGHSCORE();
    }
}
