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
                            { my_FreeMem(IMemA[i], IMemL[i]); IMemA[i] = NULL; }
    }
    
    for (i = 0; i < SOUNDS; ++i)
    {
        if (NULL != SoundMemA[i])
                            { my_FreeMem(SoundMemA[i], SoundSize[i]<<1); SoundMemA[i] = NULL; }
    }

    if (NULL != ZeroSound)  { my_FreeMem(ZeroSound, 8); ZeroSound = NULL; }

    //    OpenWorkBench();
    CLOSEMYSCREENS();
    FREESYSTEMMEMORY();

    if (NULL != PathMemA)   { my_FreeMem(PathMemA, PathMemL); PathMemA = NULL; };
    if (NULL != TextMemA)   { my_FreeMem(TextMemA, TextMemL); TextMemA = NULL; };

    if (NULL != XScreen)
        { CloseScreen(XScreen); };
}
