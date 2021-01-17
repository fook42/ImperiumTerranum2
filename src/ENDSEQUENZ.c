#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void ENDSEQUENZ(sint16 Mode)
{
typedef struct SArr13 { char* data[11]; } SArr13;

    SArr13  SA13;
    uint8   i, t;
    uint16  slen, ypos;
    BPTR    FHandle;
    struct MMD0 *SndModulePtr = NULL;
    char    s[70];
    char    s2[70];
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
        return;
    }

    MyScreen[0] = OPENCINEMA(8);
    if (NULL == MyScreen[0])
    {
        puts("can not open screen - exit!\n");
        return;
    }

    RPort_PTR = &(MyScreen[0]->RastPort);

    slen = strlen(PathStr[4]);
    memcpy(s, PathStr[4], slen);
    if ((1 == Mode) || (-3 == Mode))
    {
        strcpy( s+slen, "MOD.HappyEnd" );
    } else {
        strcpy( s+slen, "MOD.DeadEnd" );
    }
    FHandle = OPENSMOOTH(s, MODE_OLDFILE);
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
            strcpy( s+slen, "FieldEnd.pal" );
        } else {
            strcpy( s+slen, "DeadEnd.pal" );
        }
    } else if (-2 == Mode)
    {
        strcpy( s+slen, "BigBang.pal" );
    } else {
        strcpy( s+slen, "HappyEnd.pal" );
    }
    (void) SETCOLOR(MyScreen[0], s);
    strcpy( s+strlen(s)-3, "img" );

    if (!DISPLAYIMAGE(s,0,75,640,360,8,MyScreen[0],0))
    {
        puts("can not display image - exit!\n");
        return;
    }

    SetRGB4(&(MyScreen[0]->ViewPort),255,7,7,7);
    if (-1 == Mode)
    {
        slen = strlen(_PT_ImJahre);
        memcpy(s, _PT_ImJahre, slen);
        s[slen++]=' ';
        _s = dez2out(Year, 0, s+slen);
        *_s++=' ';
        strcpy(_s, PText[759]);
        if (Save.WorldFlag == WFLAG_FIELD)
        {
            SA13 = (SArr13) {{s,PText[760],PText[761],PText[762],PText[763],PText[764],
                                PText[765],PText[766],PText[767],PText[768],PText[769]}};
            t = 11;
        } else {
            slen = strlen(PText[771]);
            strcpy(s2, PText[771]);
            s2[slen++]=' ';
            strcpy(s2+slen, GETCIVADJ(ActPlayer));
            strcpy(s2+strlen(s2), PText[772]);
            SA13 = (SArr13) {{s,s2,        PText[773],PText[774],PText[775],PText[776],
                                PText[777],PText[778],PText[779],PText[780],NULL}};
            t = 10;
        }
    } else if (-2 == Mode)
    {
        _s = dez2out(Year, 0, s);
        *_s++=' ';
        strcpy(_s, PText[782]);
        slen = strlen(s);
        s[slen++]=' ';
        strcpy(s+slen, GETCIVNAME(ActPlayer));
        slen = strlen(s);
        s[slen++]=' ';
        strcpy(s+slen, PText[783]);
        SA13 = (SArr13) {{s,PText[784],PText[785],PText[786],PText[787],PText[788],
                            PText[789],PText[790],PText[791],NULL,NULL}};
        t = 9;
    } else if (-3 == Mode)
    {
        Save.ImperatorState[ActPlayer-1] -= 1500;
        slen = strlen(_PT_ImJahre);
        memcpy(s, _PT_ImJahre, slen);
        s[slen++]=' ';
        _s = dez2out(Year, 0, s+slen);
        *_s++=' ';
        strcpy(_s, PText[794]);
        slen = strlen(s);
        s[slen++]=' ';
        strcpy(s+slen, GETCIVNAME(ActPlayer));
        slen = strlen(s);
        s[slen++]=' ';
        strcpy(s+slen, PText[795]);
        SA13 = (SArr13) {{s,PText[796],PText[797],PText[798],PText[799],PText[800],
                            PText[801],PText[802],PText[803],PText[804],NULL}};
        t = 10;
    } else if (1 == Mode)
    {
        slen = strlen(_PT_ImJahre);
        memcpy(s, _PT_ImJahre, slen);
        s[slen++]=' ';
        _s = dez2out(Year, 0, s+slen);
        *_s++=' ';
        strcpy(_s, PText[794]);
        slen = strlen(s);
        s[slen++]=' ';
        strcpy(s+slen, GETCIVNAME(ActPlayer));
        slen = strlen(s);
        if (3 != ActPlayer)
        {
            s[slen++]='n';
        }
        s[slen++]=',';
        s[slen++]=' ';
        strcpy(s+slen, PText[806]);
        SA13 = (SArr13) {{s,PText[807],PText[808],PText[809],PText[810],PText[811],
                            PText[812],PText[813],PText[814],NULL,NULL}};
        t = 9;
    } else {
        slen = strlen(_PT_ImJahre);
        memcpy(s, _PT_ImJahre, slen);
        s[slen++]=' ';
        _s = dez2out(Year, 0, s+slen);
        *_s++=' ';
        strcpy(_s, PText[794]);
        slen = strlen(s);
        s[slen++]=' ';
        strcpy(s+slen, GETCIVNAME(Mode));
        slen = strlen(s);
        if (3 != Mode)
        {
            s[slen++]='n';
        }
        s[slen++]=',';
        s[slen++]=' ';
        strcpy(s+slen, PText[816]);
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
    }

    SWITCHDISPLAY();
    RECTWIN(RPort_PTR,0,0,75,639,434);
}
