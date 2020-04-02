#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void GAMEEXIT()
{
    uint8   i;

    SMALLGAMEEXIT();
    for (i = 0; i < IMAGES; ++i)
    {
        if (NULL != IMemA[i])
                            { FreeMem(IMemA[i], IMemL[i]); IMemA[i] = NULL; }
    }
    
    for (i = 0; i < SOUNDS; ++i)
    {
        if (NULL != SoundMemA[i])
                            { FreeMem(SoundMemA[i], SoundSize[i]<<1); SoundMemA[i] = NULL; }
    }

    if (NULL != ZeroSound)  { FreeMem(ZeroSound, 8); ZeroSound = NULL; }

    //    OpenWorkBench();
    CLOSEMYSCREENS();
    FREESYSTEMMEMORY();

    if (NULL != PathMemA)   { FreeMem(PathMemA, PathMemL); PathMemA = NULL; };
    if (NULL != TextMemA)   { FreeMem(TextMemA, TextMemL); TextMemA = NULL; };

    if (NULL != XScreen)
        { CloseScreen(XScreen); };
}
