#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

int MAININTRO_PART3(uint16** SMemA, LONG* SMemL, struct MMD0 **SndModulePtr)
{
    char        s[40];
    char*       _s;
    BPTR        FHandle;
    r_Col_t     Colors[128];
    struct RastPort*    actRastPort;
    uint8       i, k;
    LONG        ISize;
    uint16      dFactor;

    for (i = 0; i<2; i++)
    {
        MyScreen[i] = OPENCINEMA(7);
        if (NULL == MyScreen[i])
        {
            return -21;
        }
        MyRPort_PTR[i] = &(MyScreen[i]->RastPort);
        MyVPort_PTR[i] = &(MyScreen[i]->ViewPort);
    }

    IMemL[0] = 201856;
    IMemL[1] = 21000;
    for (i = 0; i<2; ++i)
    {
        IMemA[i] = (uint8*) AllocMem(IMemL[i], MEMF_CHIP);
        if (NULL == IMemA[i])
        {
            return -22;
        }
    }

    SPAddr(2) = SMemA[1]+(SMemL[1]/2)+6500; SPVol(2) = 64; SPFreq(2) = 380; SPLength(2) = (UWORD) ((SMemL[1]-13000)/4);
    SPAddr(3) = SMemA[1]+6500;              SPVol(3) = 64; SPFreq(3) = 380; SPLength(3) = (UWORD) ((SMemL[1]-13000)/4);

    custom.dmacon = BITSET | DMAF_AUD2 | DMAF_AUD3; // 0x800C
    WaitTOF();
    WaitTOF();

    SPLength(3) = 1;
    SPLength(2) = 1;

    for (i = 255; i > 10; i -= 5)
    {
        SetRGB32(MyVPort_PTR[1], 0, (i*(ULONG)0x01010101), (i*(ULONG)0x01010101), (i*(ULONG)0x01010101));
        WaitTOF();
    }
    SetRGB32(MyVPort_PTR[1],0,0,0,0);
    custom.dmacon = BITCLR | DMAF_AUD2 | DMAF_AUD3; // 0x000C
    if (LMB_PRESSED) { return 1; }

    _s=my_strcpy(s,PathStr[7]);

    (void) my_strcpy(_s, "MOD.Intro");
    FHandle = OPENSMOOTH(s,MODE_OLDFILE, NULL);
    if (0 != FHandle)
    {
        Close(FHandle);
        StopPlayer();
        FreePlayer();
        *SndModulePtr = LoadModule(s);   // module will be loaded directly from disk
        if (GETMIDIPLAYER(*SndModulePtr))
        {
            PlayModule(*SndModulePtr);
        }
    }

    (void) my_strcpy(_s, "Frame4.pal");
    SETDARKCOLOR(s, Colors);
    (void) my_strcpy(_s, "Frame4.img");       // terrain with stars...
    if (!DISPLAYIMAGE(s,0,75,640,360,7,MyScreen[1],MyRPort_PTR[1],0))
    {
        return -23;
    }
    ClipBlit(MyRPort_PTR[1],0,75,MyRPort_PTR[0],0,75,640,360,192);

    (void) my_strcpy(_s, "Frame5.img");       // little earth-image..

    FHandle = OPENSMOOTH(s,MODE_OLDFILE, &ISize);
    if (0 != FHandle)
    {
        (void) Read(FHandle,(APTR) (IMemA[0]+IMemL[0]-ISize-250), ISize);
        UNPACK(IMemA[0],IMemA[0]+IMemL[0]-ISize-250,20160,0);
        Close(FHandle);
    }

    struct BitMap MyBitMap = { 20, 144, 1, 7, 0, \
                            {(PLANEPTR) (IMemA[0]),      (PLANEPTR) (IMemA[0]+2880), (PLANEPTR) (IMemA[0]+5760), \
                             (PLANEPTR) (IMemA[0]+8640), (PLANEPTR) (IMemA[0]+11520),(PLANEPTR) (IMemA[0]+14400), \
                             (PLANEPTR) (IMemA[0]+17280), NULL }};

    // fade in....
    dFactor = 0;
    k = 20;
    do
    {
        ScreenToFront(MyScreen[AScr]);
        AScr=1-AScr;
        dFactor += 0xCD; // 11001101 =~ 0,05  >>12
        for (i = 1; i<128; ++i)
        {
            SetRGB32(MyVPort_PTR[AScr], i, (Colors[i].r*dFactor)<<12,
                                           (Colors[i].g*dFactor)<<12,
                                           (Colors[i].b*dFactor)<<12);
        }
        WaitTOF();
        --k;
    }
    while (0 != k);

    // set all colours to the max. values
    for (i = 1; i<128; ++i)
    {
        SetRGB32(MyVPort_PTR[1-AScr], i, Colors[i].r*(ULONG)0x01010101,
                                         Colors[i].g*(ULONG)0x01010101,
                                         Colors[i].b*(ULONG)0x01010101);

        SetRGB32(MyVPort_PTR[  AScr], i, Colors[i].r*(ULONG)0x01010101,
                                         Colors[i].g*(ULONG)0x01010101,
                                         Colors[i].b*(ULONG)0x01010101);
    }
    ScrollRaster(MyRPort_PTR[AScr],0,-4,0,75,639,434);
    WaitTOF();

    for (i = 0; i<6; ++i)
    {
        ScreenToFront(MyScreen[AScr]);
        AScr = 1-AScr;
        actRastPort = MyRPort_PTR[AScr];
        ScrollRaster(actRastPort,0,-8,0,75,639,434);
        WaitTOF();
        ClipBlit(actRastPort,0,270,actRastPort,0,75,640,8,192);
        if (LMB_PRESSED) { return 1; }
    }

    for (i = 0; i<35; ++i)
    {
        ScreenToFront(MyScreen[AScr]);
        AScr = 1-AScr;
        actRastPort = MyRPort_PTR[AScr];
        ScrollRaster(actRastPort,0,-8,0,75,639,434);
        ClipBlit(actRastPort,0,270,actRastPort,0,75,640,8,192);
        BltBitMapRastPort(&MyBitMap,0,136-i*4,actRastPort,380,75,160,8,192);
        WaitTOF();
        if (LMB_PRESSED) { return 1; }
    }

    ScreenToFront(MyScreen[AScr]);
    AScr = 1-AScr;
    actRastPort = MyRPort_PTR[AScr];
    ScrollRaster(actRastPort,0,-8,0,75,639,434);
    ClipBlit(actRastPort,0,270,actRastPort,0,75,640,8,192);
    BltBitMapRastPort(&MyBitMap,0,0,actRastPort,380,79,160,4,192);
    WaitTOF();

    for (i = 0; i<14; ++i)
    {
        ScreenToFront(MyScreen[AScr]);
        AScr = 1-AScr;
        actRastPort = MyRPort_PTR[AScr];
        ScrollRaster(actRastPort,0,-8,0,75,639,434);
        WaitTOF();
        ClipBlit(actRastPort,0,270,actRastPort,0,75,640,8,192);
        if (LMB_PRESSED) { return 1; }
    }

    Delay(10);

    SetRast(actRastPort, 0); // clear Rastport
    ScreenToFront(MyScreen[AScr]);
    AScr = 1-AScr;
    SetRast(MyRPort_PTR[AScr], 0); // clear Rastport

    return 0;
}
