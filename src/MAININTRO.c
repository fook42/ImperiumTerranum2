#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

// ------------------------------

void SETDARKCOLOR(char* FName, r_Col_t* Colors)
{
    uint32  AddrX, AddrEnd, ISize;
    uint8   i;
    uint32* ColorID;
    BPTR    FHandle;

    FHandle = OPENSMOOTH(FName, MODE_OLDFILE);
    if (0 != FHandle)
    {
        (void)  Seek(FHandle, 0, OFFSET_END);
        ISize = Seek(FHandle, 0, OFFSET_BEGINNING);
        (void) Read(FHandle, (APTR) IMemA[0], ISize);
        Close(FHandle);
        AddrX = (uint32) IMemA[0];
        AddrEnd = AddrX + ISize;
        do
        {
            ColorID = (uint32*) AddrX;
            AddrX += 4;
        }
        while ((AddrX < AddrEnd) && (_COLOR_CMAP_TEXT_ != *ColorID));

        if (_COLOR_CMAP_TEXT_ == *ColorID)
        {
            AddrX += 4; // skip "IMPT"-string
            i = 0;
            do
            {
                SetRGB32(MyVPort_PTR[0],i,0,0,0);
                SetRGB32(MyVPort_PTR[1],i,0,0,0);

                Colors[i] = *((r_Col_t*) AddrX);
                AddrX += sizeof(r_Col_t);
                ++i;
            }
            while (AddrX < AddrEnd);
        }
        Colors[31].r = 45;
        Colors[31].g = 45;
        Colors[31].b = 62;
    }
}

void INTROEXIT(struct MMD0 *module, uint16** SMemA, uint32* SMemL)
{
    uint8   i;

    SWITCHDISPLAY();
    (void) SetTaskPri(FindTask(NULL),0);

    for (i = 0; i<2; ++i)
    {
        if (NULL != MyScreen[i])
        {
            CloseScreen(MyScreen[i]);
            MyScreen[i] = NULL;
            MyRPort_PTR[i] = NULL;
            MyVPort_PTR[i] = NULL;
        }
    }
    for (i = 0; i<3; ++i)
    {
        if (NULL != SMemA[i])
        {
            FreeMem((APTR) SMemA[i], SMemL[i]);
            SMemA[i] = NULL;
        }
    }
    for (i = 0; i<2; ++i)
    {
        if (NULL != IMemA[i])
        {
            FreeMem((APTR) IMemA[i], IMemL[i]);
            IMemA[i] = NULL;
        }
    }
    if (NULL != module)
    {
        StopPlayer();
        FreePlayer();
        UnLoadModule(module);
    }
    custom.dmacon = BITCLR | DMAF_AUDIO; // 0x000F
}

bool LOADSOUNDS(char* FNamePath, char* FName, uint16** SMemA, LONG* SMemL)
{
    uint8   i;
    LONG    ssize;
    BPTR    FHandle;

    (void) my_strcpy(FName, "Snd0.RAW");
    for (i = 0; i<3; ++i)
    {
        FName[3] = i+'1';
        FHandle = OPENSMOOTH(FNamePath,MODE_OLDFILE);
        if (0 != FHandle)
        {
            (void)  Seek(FHandle, 0, OFFSET_END);
            ssize = Seek(FHandle, 0, OFFSET_BEGINNING);
            SMemL[i] = ssize;
            SMemA[i] = (uint16*) AllocMem(ssize, MEMF_CHIP | MEMF_CLEAR);
            if (NULL == SMemA[i]) { return false; }
            (void) Read(FHandle, (APTR) SMemA[i], SMemL[i]);
            Close(FHandle);
        } else {
            return false;
        }
    }
    return true;
}

void MAININTRO()
{
    uint16*     SMemA[3];
    LONG        SMemL[3];
    char        s[40];
    char*       _s;
    struct MMD0 *SndModulePtr = NULL;

    int         i, part_return;

    //    i = SetTaskPri(FindTask(NULL),120);
    AScr = 0;
    for (i = 0; i<2; ++i) { MyScreen[i] = NULL; IMemA[i] = NULL; MyRPort_PTR[i] = NULL; MyVPort_PTR[i] = NULL; }
    for (i = 0; i<3; ++i) { SMemA[i] = NULL; SMemL[i] = 0; }

    _s=my_strcpy(s,PathStr[7]);
    if (!LOADSOUNDS(s, _s, SMemA, SMemL))
    {
        goto leave_intro;
    }

    part_return = MAININTRO_PART1(SMemA, SMemL);
    if      (0 > part_return) { goto leave_intro; }
    else if (0 < part_return) { goto leave_intro; }

    part_return = MAININTRO_PART2(SMemA, SMemL);
    if      (0 > part_return) { goto leave_intro; }
    else if (0 < part_return) { goto leave_intro; }

    part_return = MAININTRO_PART3(SMemA, SMemL, &SndModulePtr);
    if      (0 > part_return) { goto leave_intro; }
    else if (0 < part_return) { goto leave_intro; }

    part_return = MAININTRO_PART4();
    if      (0 > part_return) { goto leave_intro; }
    else if (0 < part_return) { goto leave_intro; }

leave_intro:
    INTROEXIT(SndModulePtr, SMemA, SMemL);
}
