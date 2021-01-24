#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void SMALLGAMEEXIT()
{
    int     i;

    custom.dmacon = BITCLR | DMAF_AUDIO; // 0x000F

    if (NULL != ImgBitMap4.MemA)
    {
        FreeMem(ImgBitMap4.MemA, ImgBitMap4.MemL); ImgBitMap4.MemA = NULL;
    }

    if (NULL != ImgBitMap7.MemA)
    {
        FreeMem(ImgBitMap7.MemA, ImgBitMap7.MemL); ImgBitMap7.MemA = NULL;
    }

    if (NULL != ImgBitMap8.MemA)
    {
        FreeMem(ImgBitMap8.MemA, ImgBitMap8.MemL); ImgBitMap8.MemA = NULL;
    }

    for (i = 0; i< MODULES; ++i)
    {
        if (NULL != ModMemA[i])
        {
            FreeMem(ModMemA[i], ModMemL[i]);       ModMemA[i] = NULL;
        }
    }
    for (i = 0; i< CACHES; ++i)
    {
        if (NULL != CacheMemA[i])
        {
            FreeMem(CacheMemA[i], CacheMemL[i]);   CacheMemA[i] = NULL;
        }
    }
    for (i = 0; i<(MAXCIVS-1); ++i)
    {
        if (NULL != LogoMemA[i])
        {
            FreeMem(LogoMemA[i], LOGOSIZE);        LogoMemA[i] = NULL;
        }
        if (NULL != LogoSMemA[i])
        {
            FreeMem(LogoSMemA[i], LogoSMemL[i]);   LogoSMemA[i] = NULL;
        }
    }
}
