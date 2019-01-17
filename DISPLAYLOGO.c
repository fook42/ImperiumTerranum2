#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void DISPLAYLOGO(uint8 DL_ActPlayer, uint16 LEdge, uint16 TEdge)
{
    char    FName[60];
    BPTR    FHandle;
    uint32  ISize;
    uint8   l;
    APTR    ActMem;
    struct Image    stImg = {0,0,128,128,7, NULL ,127,0,NULL};

    if (NULL == LogoMemA[0])
    {
        LogoMemA[0] = AllocMem(LOGOSIZE ,MEMF_CHIP+MEMF_CLEAR);
    }
    if (NULL != LogoMemA[0])
    {
        if (NULL == LogoMemA[DL_ActPlayer])
        {
            LogoMemA[DL_ActPlayer] = AllocMem(LOGOSIZE ,MEMF_FAST+MEMF_CLEAR);
            ActMem = LogoMemA[DL_ActPlayer];
            if (NULL == ActMem) { ActMem = LogoMemA[0]; }
            l = strlen(PathStr[10]);
            memcpy(FName, PathStr[10], l);
            FName[l] = DL_ActPlayer+'0';
            strcpy(FName+l+1, ".img");
            FHandle = OPENSMOOTH(FName, MODE_OLDFILE);
            if (0 != FHandle)
            {
                (void) Seek(FHandle, 0, OFFSET_END);
                ISize = Seek(FHandle, 0, OFFSET_BEGINNING);
                (void) Read(FHandle, (APTR) (ActMem+LOGOSIZE-ISize-150), ISize);
                UNPACK(ActMem, (APTR) (ActMem+LOGOSIZE-ISize-150), LOGOMEMSIZE, 1);
                Close(FHandle);
            }
        }
        if (NULL != LogoMemA[DL_ActPlayer])
        {
            CopyMemQuick(LogoMemA[DL_ActPlayer], LogoMemA[0], LOGOSIZE);
        }
        stImg.ImageData = LogoMemA[0];
        DrawImage(&(MyScreen[0]->RastPort), &stImg, LEdge, TEdge);
    }
}
