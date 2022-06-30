#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

int MAININTRO_PART1(uint16** SMemA, LONG* SMemL)
{
    int                 i;
    uint16              xpos;
    struct ITBitMap     IntroBitMap;
    char                s[40];
    char*               _s;
    int                 ret_code = -1;

    for (i = 0; i<2; i++)
    {
        MyScreen[i] = OPENCINEMA(5);
        if (NULL == MyScreen[i])
        {
            goto leave_part;
        }
        MyRPort_PTR[i] = &(MyScreen[i]->RastPort);
        MyVPort_PTR[i] = &(MyScreen[i]->ViewPort);
    }
    ScreenToFront(MyScreen[AScr]);

    if (!FillITBitMap(&IntroBitMap, 80, 183, 5))
    {
        ret_code = -2;
        goto leave_part;
    }
    IMemL[0] = IntroBitMap.MemL;            // needed for RAWLOADIMAGE, DISPLAYIMAGE...
    IMemA[0] = (uint8*) IntroBitMap.MemA;   // will be freed automatically when intro ends

    _s = my_strcpy(s,PathStr[7]);
    (void) my_strcpy(_s, "Frame0.pal");
    (void) SETCOLOR(MyScreen[0], s);
    (void) SETCOLOR(MyScreen[1], s);

    (void) my_strcpy(_s, "Frame0.img");   // Touchbyte ...
    if (!RAWLOADIMAGE(s,0,0,640,183,5,&IntroBitMap))
    {
        ret_code = -3;
        goto leave_part;
    }
    xpos = 5;
    for (i = 0; i<8; ++i)
    {
        AScr = 1-AScr;
        BltBitMapRastPort((struct BitMap*) &IntroBitMap,0,0,MyRPort_PTR[AScr],HighRes_Width-xpos,340,xpos,90,192);
        xpos += 5;
        WaitTOF();
        ScreenToFront(MyScreen[AScr]);
    }
    xpos = HighRes_Width-xpos-5;
    do
    {
        xpos -= 5;
        AScr = 1-AScr;
        BltBitMapRastPort((struct BitMap*) &IntroBitMap, 0,0,MyRPort_PTR[AScr],xpos   ,340,49,90,192);
        BltBitMapRastPort((struct BitMap*) &IntroBitMap,41,0,MyRPort_PTR[AScr],xpos+49,340, 5,90,192);
        WaitTOF();
        ScreenToFront(MyScreen[AScr]);
        if (LMB_PRESSED) { ret_code = 1; goto leave_part; }
    }
    while (xpos > 10);
    ClipBlit(MyRPort_PTR[AScr],10,340,MyRPort_PTR[1-AScr],10,340,50,90,192);
    xpos = 5;
    for (i = 0; i<9; ++i)
    {
        AScr = 1-AScr;
        BltBitMapRastPort((struct BitMap*) &IntroBitMap,50,0,MyRPort_PTR[AScr],HighRes_Width-xpos,340,xpos,90,192);
        xpos += 5;
        WaitTOF();
        ScreenToFront(MyScreen[AScr]);
    }
    SetAPen(MyRPort_PTR[  AScr],0);
    SetAPen(MyRPort_PTR[1-AScr],0);
    xpos = HighRes_Width-xpos+5;
    do
    {
        xpos -= 5;
        AScr = 1-AScr;
        BltBitMapRastPort((struct BitMap*) &IntroBitMap,50,0,MyRPort_PTR[AScr],xpos,340,41,90,192);
        RectFill(MyRPort_PTR[AScr],xpos+41,340,xpos+50,430);
        WaitTOF();
        ScreenToFront(MyScreen[AScr]);
        if (LMB_PRESSED) { ret_code = 1; goto leave_part; }
    }
    while (xpos > 60);

    ClipBlit(MyRPort_PTR[AScr],10,340,MyRPort_PTR[1-AScr],10,340,150,90,192);
    Delay(5);
    for (i = 0; i<2; i++)
    {
        AScr = 1-AScr;
        BltBitMapRastPort((struct BitMap*) &IntroBitMap,0,0,MyRPort_PTR[AScr],10,340,616,91,192);
        ScreenToFront(MyScreen[AScr]);
    }
    if (LMB_PRESSED) { ret_code = 1; goto leave_part; }
    Delay(15);
    AScr = 1-AScr;
    BltBitMapRastPort((struct BitMap*) &IntroBitMap,0,90,MyRPort_PTR[AScr],10,337,98,90,192);
    ScreenToFront(MyScreen[AScr]);
    custom.dmacon = BITCLR | DMAF_AUDIO; // 0x000F
    Delay(15);

    SPAddrA = SMemA[0];              SPVolA  = 0;  SPFreqA = 856; SPLengthA = (UWORD) (SMemL[0]/4);
    SPAddrB = SMemA[0]+(SMemL[0]/2); SPVolB  = 0;  SPFreqB = 856; SPLengthB = (UWORD) (SMemL[0]/4);
    SPAddrC = SMemA[1]+(SMemL[1]/2); SPVolC  = 64; SPFreqC = 380; SPLengthC = 1;
    SPAddrD = SMemA[1];              SPVolD  = 64; SPFreqD = 380; SPLengthD = 1;

    custom.dmacon = BITSET | DMAF_AUD0 | DMAF_AUD1; // 0x8003

    for (i = 0; i<65; ++i)
    {
        SPVolA = i;
        SPVolB = i;
        WaitTOF();
    }

    Delay(25);
    SetRast(MyRPort_PTR[1-AScr],0);
    WaitTOF();
    ScreenToFront(MyScreen[1-AScr]);
    SetRast(MyRPort_PTR[AScr],0);
    WaitTOF();
    ScreenToFront(MyScreen[AScr]);

    ret_code = 0;
leave_part:

    return ret_code;
}
