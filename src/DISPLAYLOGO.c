#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void DISPLAYLOGO(const int DL_ActPlayer, const int LEdge, const int TEdge)
{
    char    FName[60];
    char*   _s;
    BPTR    FHandle;
    uint32  ISize;
    uint8*  ActMem;
    struct Image    stImg = {0,0,128,128,7, NULL ,127,0,NULL};

    if (NULL == LogoMemA[0])
    {
        /* Unlike some compiler memory allocation functions, the Amiga system memory
           allocation functions return memory blocks that are at least longword aligned.
           This means that the allocated memory will always start on an address which
           is at least evenly _divisible by four_. */

        LogoMemA[0] = (uint8*) AllocMem(LOGOSIZE ,MEMF_CHIP | MEMF_CLEAR);
    }
    if (NULL != LogoMemA[0])
    {
        if (NULL == LogoMemA[DL_ActPlayer])
        {
            LogoMemA[DL_ActPlayer] = (uint8*) AllocMem(LOGOSIZE ,MEMF_FAST | MEMF_CLEAR);
            ActMem = LogoMemA[DL_ActPlayer];
            if (NULL == ActMem) { ActMem = LogoMemA[0]; }
            _s=my_strcpy(FName, PathStr[10]);
            *_s++ = '0'+DL_ActPlayer;
            (void) my_strcpy(_s, ".img");
            FHandle = OPENSMOOTH(FName, MODE_OLDFILE);
            if (0 != FHandle)
            {
                (void)  Seek(FHandle, 0, OFFSET_END);
                ISize = Seek(FHandle, 0, OFFSET_BEGINNING);
                (void) Read(FHandle, (APTR) (ActMem+LOGOSIZE-ISize-150), ISize);
                UNPACK(ActMem, (ActMem+LOGOSIZE-ISize-150), LOGOMEMSIZE, 1);
                Close(FHandle);
            }
        }
        if (NULL != LogoMemA[DL_ActPlayer])
        {
            // for this the source & dest need to be longword-aligned.. size should be multiple of 4
            CopyMemQuick(LogoMemA[DL_ActPlayer], LogoMemA[0], LOGOSIZE);
        }
        stImg.ImageData = (UWORD*) LogoMemA[0];
        DrawImage(&(MyScreen[0]->RastPort), &stImg, (WORD) LEdge, (WORD) TEdge);
    }
}
