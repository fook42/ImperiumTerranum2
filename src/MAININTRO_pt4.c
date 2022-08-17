#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

const char* SArr[] = {"",
    "Software & Design",            "Oxygenic",
    "Art Director",                 "Cybertrace",
    "Music by",                     "Ludwig v.Beethoven   N.N. Ikonnikow   Richard Wagner",
    "Special Effects",              "Oxygenic",
    "Credits go to",                "Adam Benjamin   Rikard Cederlund",
    "Jakob Gaardsted   Andy Jones", "George Moore",
    "Surround-Sounds created with", "WaveTracer DS�",
    "Colors in Technicolor�",       "Panaflex� Camera and Lenses by Panavision�",
    "Ported to C & improved by",    "Fook42"};
const uint8 FArr[] = {0, 3,4, 3,4, 3,4, 3,4, 3,4, 4,4, 3,4, 3,3, 3,4};

const struct worldcolors_t Frame6_Colours[] = { {0xEEEEEEEE,0xEEEEEEEE,0xFFFFFFFF},
                                                {0xDDDDDDDD,         0,         0},
                                                {0xFFFFFFFF,         0,0xFFFFFFFF},
                                                {0xBBBBBBBB,0xBBBBBBBB,0xFFFFFFFF},
                                                {0x99999999,0x99999999,0xBBBBBBBB} };

int MAININTRO_PART4()
{
    char        s[40];
    char*       _s;
    PLANEPTR    MyRastPtr = NULL;
    APTR        IntroAreaMem = NULL;
    struct TmpRas       MyTmpRas;
    struct AreaInfo     MyAI;
    struct RastPort*    actRastPort;

    double ShipX[] = { 390, 264,252,186,186,192,256,290,354,360,360,294,282, 252, 256, 290,294, 282,294,294, 258,265,273,281,288, 185,185,190,190, 356,356,361,361,0,0,0,0,0,0,0,0};
    double ShipY[] = { 315, 174,275,250,188,225,234,234,225,188,250,275,174, 275, 243, 243,275, 174,275,275, 269,342,269,342,269, 187,172,172,187, 187,172,172,187,0,0,0,0,0,0,0,0};
    double ShipZ[] = {-2.5,  -5, -1, -5, -5, -5, -1, -1, -5, -5, -5, -1, -5,  -1,-1.1,-1.1, -1,  -5, -8, -1,  -2, -2, -2, -2, -2,  -5, -5, -5, -5,  -5, -5, -5, -5,0,0,0,0,0,0,0,0};

    sint16      LEdge[2], TEdge[2];
    uint32      l, ISize;
    double      FacSin, FacCos;
    double      SizeFactor;
    double      Factor;
    double      store;
    uint8       i, k;
    sint16      SXdata0int, SYdata0int;

    int         ret_code = -1;

     _s=my_strcpy(s,PathStr[7]);

    (void) my_strcpy(_s, "Frame6.pal");
    (void) SETCOLOR(MyScreen[  AScr],s);
    (void) SETCOLOR(MyScreen[1-AScr],s);
    for (i = 0; i < 5; ++i)
    {
        SetRGB32(MyVPort_PTR[  AScr], 123+i, Frame6_Colours[i].r, Frame6_Colours[i].g, Frame6_Colours[i].b);
        SetRGB32(MyVPort_PTR[1-AScr], 123+i, Frame6_Colours[i].r, Frame6_Colours[i].g, Frame6_Colours[i].b);
    }

    (void) my_strcpy(_s, "Frame6.img");
    if (!DISPLAYIMAGE(s,0,75,640,360,7,MyScreen[AScr],0)) { ret_code = -31; goto leave_part; }
    ScreenToFront(MyScreen[AScr]);
    struct BitMap MyBitMap = (struct BitMap) { 80, 360, 1, 7, 0, \
                            {(PLANEPTR) (IMemA[0]),       (PLANEPTR) (IMemA[0]+28800), (PLANEPTR) (IMemA[0]+57600), \
                             (PLANEPTR) (IMemA[0]+86400), (PLANEPTR) (IMemA[0]+115200),(PLANEPTR) (IMemA[0]+144000), \
                             (PLANEPTR) (IMemA[0]+172800)}};
    BltBitMapRastPort(&MyBitMap,0,0,MyRPort_PTR[1-AScr],0,75,640,360,192);
    WaitTOF();
    ScreenToFront(MyScreen[1-AScr]);

    MyRastPtr = AllocRaster(640, 360);
    if (NULL == MyRastPtr)
    {
        ret_code = -32;
        goto leave_part;
    }
    InitTmpRas(&MyTmpRas,MyRastPtr,21000);
    IntroAreaMem = AllocMem(IntroAreaSize, MEMF_CHIP);
    if (NULL == IntroAreaMem)
    {
        ret_code = -33;
        goto leave_part;
    }
    InitArea(&MyAI, IntroAreaMem, 200);
    MyRPort_PTR[0]->TmpRas = &MyTmpRas;
    MyRPort_PTR[1]->TmpRas = &MyTmpRas;
    MyRPort_PTR[0]->AreaInfo = &MyAI;
    MyRPort_PTR[1]->AreaInfo = &MyAI;

    for (i = 1; i<41; ++i)
    {
        ShipX[i] = 273-ShipX[i];
        ShipY[i] = 257-ShipY[i];
    }

    // rotate around x-axis
    FacSin = sin(1.35);
    FacCos = cos(1.35);
    for (i = 1; i<41; ++i)
    {
        store = ShipY[i];
        ShipY[i] = store*FacCos - ShipZ[i]*FacSin;
        ShipZ[i] = store*FacSin + ShipZ[i]*FacCos;
    }

    // rotate around y-axis
    FacSin = sin(-0.44);
    FacCos = cos(-0.44);
    for (i = 1; i<41; ++i)
    {
        store = ShipX[i];
        ShipX[i] = store*FacCos - ShipZ[i]*FacSin;
        ShipZ[i] = store*FacSin + ShipZ[i]*FacCos;
    }

    // rotate around z-axis
    FacSin = sin(-0.08);
    FacCos = cos(-0.08);
    for (i = 1; i<41; ++i)
    {
        store = ShipX[i];
        ShipX[i] = store*FacCos - ShipY[i]*FacSin;
        ShipY[i] = store*FacSin + ShipY[i]*FacCos;
    }

    if (LMB_PRESSED)
        { ret_code = 1; goto leave_part; }

    Factor     = 0.0074;
    SizeFactor = 0.009;
    LEdge[0] = 0;
    TEdge[0] = 75;
    LEdge[1] = 0;
    TEdge[1] = 75;
    l = 1;
    ISize = 0;
    do
    {
        WaitTOF();
        ScreenToFront(MyScreen[AScr]);
        AScr = 1-AScr;
        actRastPort = MyRPort_PTR[AScr];
        ShipX[0] = ShipX[0]-Factor-Factor-0.22;
        ShipY[0] = ShipY[0]-Factor;
        SXdata0int = it_round(ShipX[0]);
        SYdata0int = it_round(ShipY[0]);

        BltBitMapRastPort(&MyBitMap,LEdge[AScr],TEdge[AScr]-75,actRastPort,LEdge[AScr],TEdge[AScr],160,80,192);
        BltBitMapRastPort(&MyBitMap,20,145,actRastPort,20,220,600,55,192);

        LEdge[AScr] = SXdata0int-90;
        if (LEdge[AScr]<0) { LEdge[AScr] = 0; }

        TEdge[AScr] = SYdata0int-60;
        if (TEdge[AScr]<75) { TEdge[AScr] = 75; }

        store = ShipX[0];
        for (i = 1; i<41; ++i)
        {
            if (store < (ShipX[i]*SizeFactor))
            {
                FacSin = 1+ ((abs(store-ShipX[i]*SizeFactor))/store);
                ShipX[i] = store/SizeFactor;
                ShipY[i] = ShipY[i]/FacSin;
            }
        }

        SetAPen(actRastPort,124);
        /*** Antrieb ***/
        AreaMove(actRastPort,SXdata0int-it_round(ShipX[20]*SizeFactor),SYdata0int-it_round(ShipY[20]*SizeFactor));
        for (k = 21; k<25; ++k) { AreaDraw(actRastPort,SXdata0int-it_round(ShipX[k]*SizeFactor),SYdata0int-it_round(ShipY[k]*SizeFactor)); }
        AreaEnd(actRastPort);

        SetAPen(actRastPort,126);
        /*** BodenPlatte hell ***/
        AreaMove(actRastPort,SXdata0int -it_round(ShipX[13]*SizeFactor),SYdata0int -it_round(ShipY[13]*SizeFactor));
        for (k = 14; k<17; ++k) { AreaDraw(actRastPort,SXdata0int -it_round(ShipX[k]*SizeFactor),SYdata0int -it_round(ShipY[k]*SizeFactor)); }
        AreaEnd(actRastPort);

        /*** Seitenwand ***/
        AreaMove(actRastPort,SXdata0int -it_round(ShipX[17]*SizeFactor),SYdata0int -it_round(ShipY[17]*SizeFactor));
        for (k = 18; k<20; ++k) { AreaDraw(actRastPort,SXdata0int -it_round(ShipX[k]*SizeFactor),SYdata0int -it_round(ShipY[k]*SizeFactor)); }
        AreaEnd(actRastPort);

        SetAPen(actRastPort,127);
        /*** BodenPlatte ***/
        AreaMove(actRastPort,SXdata0int -it_round(ShipX[1]*SizeFactor),SYdata0int -it_round(ShipY[1]*SizeFactor));
        for (k = 2; k<13; ++k) { AreaDraw(actRastPort,SXdata0int -it_round(ShipX[k]*SizeFactor),SYdata0int -it_round(ShipY[k]*SizeFactor)); }
        AreaEnd(actRastPort);

        SetAPen(actRastPort,125);
        /*** Waffen ***/
        AreaMove(actRastPort,SXdata0int -it_round(ShipX[25]*SizeFactor),SYdata0int -it_round(ShipY[25]*SizeFactor));
        for (k = 26; k<29; ++k) { AreaDraw(actRastPort,SXdata0int -it_round(ShipX[k]*SizeFactor),SYdata0int -it_round(ShipY[k]*SizeFactor)); }
        AreaEnd(actRastPort);
        AreaMove(actRastPort,SXdata0int -it_round(ShipX[29]*SizeFactor),SYdata0int -it_round(ShipY[29]*SizeFactor));
        for (k = 30; k<33; ++k) { AreaDraw(actRastPort,SXdata0int -it_round(ShipX[k]*SizeFactor),SYdata0int -it_round(ShipY[k]*SizeFactor)); }
        AreaEnd(actRastPort);

        if (1 < ISize)
        {
            WRITE(320,220,123,WRITE_Center,actRastPort,FArr[l  ],SArr[l  ]);
            WRITE(320,245,123,WRITE_Center,actRastPort,FArr[l+1],SArr[l+1]);
        }
        ++ISize;
        if (55 < ISize)
        {
            l += 2;
            ISize = 0;
        }
        Factor     *= 1.018;        // 1 +  1/64 + 1/512 + 1/2048
        SizeFactor *= 1.0137;       // 1 + 1/128 + 1/256 + 1/512
        if (LMB_PRESSED)
            { ret_code = 1; goto leave_part; }
    }
    while ((Factor<3.2) && (SizeFactor<3.2));

    while ((sizeof(FArr)-1) > l)
    {
        WaitTOF();
        ScreenToFront(MyScreen[AScr]);
        AScr = 1-AScr;
        BltBitMapRastPort(&MyBitMap,0,0,MyRPort_PTR[AScr],0,75,640,360,192);
        if (0 < ISize)
        {
            WRITE(320,220,123,WRITE_Center,MyRPort_PTR[AScr],FArr[l  ],SArr[l  ]);
            WRITE(320,245,123,WRITE_Center,MyRPort_PTR[AScr],FArr[l+1],SArr[l+1]);
        }
        ++ISize;
        if (20 < ISize)
        {
            l += 2;
            ISize = 0;
        }
        if (LMB_PRESSED)
            { ret_code = 1; goto leave_part; }
    }

    ret_code = 0;
leave_part:

    /*****************************************************************************/
    // cleanup...

    if (NULL != IntroAreaMem)
    {
        FreeMem(IntroAreaMem, IntroAreaSize);
    }

    if (NULL != MyRastPtr)
    {
        FreeRaster(MyRastPtr, 640, 360);
    }

    return ret_code;
}
