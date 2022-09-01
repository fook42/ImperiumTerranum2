#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void ENDSEQUENZ(sint16 Mode)
{
typedef struct SArr13 { char* data[11]; } SArr13;

    SArr13  SA13;
    uint8   i, t;
    uint16  ypos;
    BPTR    FHandle;
    struct MMD0 *SndModulePtr = NULL;
    char    s[70];
    char    s2[50];
    char*   _s;
    struct RastPort* RPort_PTR;

    ScreenToFront(XScreen);
    SMALLGAMEEXIT();
    while (NULL != MyScreen[0]->FirstWindow)
    {
        CloseWindow(MyScreen[0]->FirstWindow);
    }
    CloseScreen(MyScreen[0]);

    IMemL[0] = 230656;
    IMemA[0] = AllocMem(IMemL[0], MEMF_CHIP);
    if (NULL == IMemA[0])
    {
        puts("not enough chip-memory (225KB) - exit!\n");
        return; // @TODO .. cleanup and free all memory!
    }

    MyScreen[0] = OPENCINEMA(8);
    if (NULL == MyScreen[0])
    {
        puts("can not open screen - exit!\n");
        return; // @TODO .. cleanup and free all memory!
    }

    RPort_PTR = &(MyScreen[0]->RastPort);

    _s = my_strcpy(s, PathStr[4]);
    if ((1 == Mode) || (-3 == Mode))
    {
        (void) my_strcpy( _s, "MOD.HappyEnd" );
    } else {
        (void) my_strcpy( _s, "MOD.DeadEnd" );
    }
    FHandle = OPENSMOOTH(s, MODE_OLDFILE, NULL);
    if (0 != FHandle)
    {
        Close(FHandle);
        StopPlayer();
        FreePlayer();
        SndModulePtr = LoadModule(s);
        if (GETMIDIPLAYER(SndModulePtr))
        {
            PlayModule(SndModulePtr);
        }
    }
    Delay(5);

    if (-1 == Mode)
    {
        if (Save.WorldFlag == WFLAG_FIELD)
        {
            _s=my_strcpy( _s, "FieldEnd.pal" );
        } else {
            _s=my_strcpy( _s, "DeadEnd.pal" );
        }
    } else if (-2 == Mode)
    {
        _s=my_strcpy( _s, "BigBang.pal" );
    } else {
        _s=my_strcpy( _s, "HappyEnd.pal" );
    }
    (void) SETCOLOR(MyScreen[0], s);
    (void) my_strcpy( _s-3, "img" );

    if (!DISPLAYIMAGE(s,0,75,640,360,8,MyScreen[0],MyRPort_PTR[0],0))
    {
        puts("can not display image - exit!\n");
        return; // @TODO .. cleanup and free all memory!
    }

    SetRGB32(&(MyScreen[0]->ViewPort),255,0x77777777,0x77777777,0x77777777);
    if (-1 == Mode)
    {
        _s=my_strcpy(s, _PT_ImJahre);
        *_s++ = ' ';
        _s = dez2out(Year, 0, _s);
        *_s++ = ' ';
        (void) my_strcpy(_s, PText[759]);
        if (Save.WorldFlag == WFLAG_FIELD)
        {
            SA13 = (SArr13) {{s,PText[760],PText[761],PText[762],PText[763],PText[764],
                                PText[765],PText[766],PText[767],PText[768],PText[769]}};
            t = 11;
        } else {
            _s=my_strcpy(s2, PText[771]);
            *_s++ = ' ';
            _s=my_strcpy(_s, GETCIVADJ(ActPlayer));
            (void) my_strcpy(_s, PText[772]);
            SA13 = (SArr13) {{s,s2,        PText[773],PText[774],PText[775],PText[776],
                                PText[777],PText[778],PText[779],PText[780],NULL}};
            t = 10;
        }
    } else if (-2 == Mode)
    {
        _s = dez2out(Year, 0, s);
        *_s++ = ' ';
        _s = my_strcpy(_s, PText[782]);
        *_s++ = ' ';
        _s = my_strcpy(_s, GETCIVNAME(ActPlayer));
        *_s++ = ' ';
        (void) my_strcpy(_s, PText[783]);
        SA13 = (SArr13) {{s,PText[784],PText[785],PText[786],PText[787],PText[788],
                            PText[789],PText[790],PText[791],NULL,NULL}};
        t = 9;
    } else if (-3 == Mode)
    {
        Save.ImperatorState[ActPlayer-1] -= 1500;
        _s = my_strcpy(s, _PT_ImJahre);
        *_s++ = ' ';
        _s = dez2out(Year, 0, _s);
        *_s++ = ' ';
        _s = my_strcpy(_s, PText[794]);
        *_s++ = ' ';
        _s = my_strcpy(_s, GETCIVNAME(ActPlayer));
        *_s++ = ' ';
        (void) my_strcpy(_s, PText[795]);
        SA13 = (SArr13) {{s,PText[796],PText[797],PText[798],PText[799],PText[800],
                            PText[801],PText[802],PText[803],PText[804],NULL}};
        t = 10;
    } else if (1 == Mode)
    {
        _s = my_strcpy(s, _PT_ImJahre);
        *_s++ = ' ';
        _s = dez2out(Year, 0, _s);
        *_s++ = ' ';
        _s = my_strcpy(_s, PText[794]);
        *_s++ = ' ';
        _s = my_strcpy(_s, GETCIVNAME(ActPlayer));
        if (3 != ActPlayer)
        {
            *_s++ = 'n';
        }
        *_s++ = ',';
        *_s++ = ' ';
        (void) my_strcpy(_s, PText[806]);
        SA13 = (SArr13) {{s,PText[807],PText[808],PText[809],PText[810],PText[811],
                            PText[812],PText[813],PText[814],NULL,NULL}};
        t = 9;
    } else {
        _s = my_strcpy(s, _PT_ImJahre);
        *_s++ = ' ';
        _s = dez2out(Year, 0, _s);
        *_s++ = ' ';
        _s = my_strcpy(_s, PText[794]);
        *_s++ = ' ';
        _s = my_strcpy(_s, GETCIVNAME(Mode));
        if (3 != Mode)
        {
            *_s++ = 'n';
        }
        *_s++ = ',';
        *_s++ = ' ';
        (void) my_strcpy(_s, PText[816]);
        SA13 = (SArr13) {{s,PText[817],PText[818],PText[819],PText[820],PText[821],
                            PText[822],PText[823],NULL,NULL,NULL}};
        t = 8;
    }
    ScreenToFront(MyScreen[0]);
    Delay(200);
    ypos = 105;
    for (i = 0; i < t; ++i)
    {
        WRITE(320,ypos,255,(WRITE_Center|WRITE_Shadow),RPort_PTR,4,SA13.data[i]);
        ypos += 25;
        Delay(50);
    }
    if (-2 == Mode)
    {
        WRITE(320,340,255,WRITE_Center,RPort_PTR,0,"(Womit bewiesen w�re, da� unser Universum geschlossen ist!)");
    }
    WAITLOOP(false);
    if (NULL != SndModulePtr)
    {
       StopPlayer();
       UnLoadModule(SndModulePtr);
       FreePlayer();
    }

    SWITCHDISPLAY();
    SetRast(RPort_PTR, 0); // clear Rastport
}
