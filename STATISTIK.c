#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void STATISTIK()
{
    ByteArr42*  ActPProjects;
    r_PlanetHeader* MyPlanet;
    uint32  Produktivitaet,Kreativitaet,Planeten;
    uint32  Bio,Infra,Ind,Groesse,Eth,Buildings,l;
    char    s[50];
    char*   _s;
    uint8   i, j;
    uint16  posy;

    struct Window* STA_Window;
    struct RastPort* RPort_PTR;
    STA_Window = MAKEWINDOW(10,30,491,361,MyScreen[0]);
    if (NULL == STA_Window)
    {
        return;
    }
    RPort_PTR = STA_Window->RPort;
    MAKEWINBORDER(RPort_PTR,0,0,490,360,12,6,1);

    Produktivitaet = 0;
    Kreativitaet = 0;
    Planeten = 0;
    Bio = 0;
    Infra = 0;
    Ind = 0;
    Groesse = 0;
    Eth = 0;
    Buildings = 0;

    for (i = 0; i < Save.Systems; ++i)
    {
        if (SystemFlags[ActPlayer-1][i] & FLAG_KNOWN)
        {
            for (j = 0; j < SystemHeader[i].Planets; ++j)
            {
                MyPlanet = &(SystemHeader[i].PlanetMemA[j]);
                if ((MyPlanet->PFlags & FLAG_CIV_MASK) == ActPlayerFlag)
                {
                    ActPProjects = MyPlanet->ProjectPtr;
                    Planeten++;
                    Kreativitaet   += ActPProjects->data[33]+ActPProjects->data[35]+ActPProjects->data[36]
                                     +ActPProjects->data[38]+ActPProjects->data[42];
                    Produktivitaet += ActPProjects->data[31]+ActPProjects->data[37]+ActPProjects->data[38]
                                     +ActPProjects->data[41]+ActPProjects->data[42];
                    Buildings      += ActPProjects->data[30]+ActPProjects->data[31]+ActPProjects->data[35]
                                     +ActPProjects->data[36]+ActPProjects->data[37]+ActPProjects->data[38]
                                     +ActPProjects->data[39]+ActPProjects->data[41];
                    if (ActPProjects->data[34] > 0) { Buildings++; }
                    if (ActPProjects->data[40] > 0) { Buildings++; }
                    Bio     += MyPlanet->Biosphaere;
                    Infra   += MyPlanet->Infrastruktur;
                    Ind     += MyPlanet->Industrie;
                    Groesse += MyPlanet->Size;
                    if ((0 != MyPlanet->Ethno) && ((MyPlanet->PFlags & FLAG_CIV_MASK) != MyPlanet->Ethno))
                    {
                        Eth++;
                    }
                }
            }
        }
    }

    posy = 20;
    for (i = 0; i < 9; ++i)
    {
        WRITE(20,posy,12,0,RPort_PTR,4, PText[700 + i]);
        posy += 20;
    }
    WRITE(20,210,ActPlayerFlag,0,RPort_PTR,4,_PT_Status);
    WRITE(50,230,ActPlayerFlag,0,RPort_PTR,4,PText[710]);
    WRITE(50,250,ActPlayerFlag,0,RPort_PTR,4,PText[711]);

    if (0 < Planeten)
    {
        _s = float2out( ((double)(20*Kreativitaet)/Planeten), 0, 2, s);
        *_s++='%';
        *_s  =0;
        WRITE(355,20,8,WRITE_Right,RPort_PTR,2,s);

        _s = float2out( ((double)(20*Produktivitaet)/Planeten), 0, 2, s);
        *_s++='%';
        *_s  =0;
        WRITE(355,40,8,WRITE_Right,RPort_PTR,2,s);

        _s = float2out( ((double)Bio/Planeten/2.0f), 0, 2, s);
        *_s++='%';
        *_s  =0;
        WRITE(355,60,8,WRITE_Right,RPort_PTR,2,s);

        _s = float2out( ((double)Infra/Planeten/2.0f), 0, 2, s);
        *_s++='%';
        *_s  =0;
        WRITE(355,80,8,WRITE_Right,RPort_PTR,2,s);

        _s = float2out( ((double)Ind/2.0f/Planeten), 0, 2, s);
        *_s++='%';
        *_s  =0;
        WRITE(355,100,8,WRITE_Right,RPort_PTR,2,s);

        (void)dez2out(it_round(Save.Bevoelkerung[ActPlayer-1]/(double) Planeten), 0, s);
        WRITE(340,120,8,WRITE_Right,RPort_PTR,2,s);

        (void)dez2out(it_round((double)Groesse/(double)Planeten/10.0f), 0, s);
        WRITE(340,140,8,WRITE_Right,RPort_PTR,2,s);

        _s = float2out( ((double)Eth/Planeten*100.0), 0, 2, s);
        *_s++='%';
        *_s  =0;
        WRITE(355,160,8,WRITE_Right,RPort_PTR,2,s);

        l = it_round((Buildings/Planeten*10.0)+(Bio/Planeten/2.0)+(Infra/Planeten/2.0));
        _s = float2out( ((double)l/3.1), 0, 2, s);
        *_s++='%';
        *_s  =0;
        WRITE(375,230,8,WRITE_Right,RPort_PTR,2,s);

        switch (it_round(l/31.0))
        {
            case 0:  _s = PText[712]; break;
            case 1:  _s = PText[713]; break;
            case 2:  _s = PText[714]; break;
            case 3:  _s = PText[715]; break;
            case 4:  _s = PText[716]; break;
            case 5:  _s = PText[717]; break;
            case 6:  _s = PText[718]; break;
            case 7:  _s = PText[719]; break;
            case 8:  _s = PText[720]; break;
            default: _s = PText[711];
        }
        WRITE(270,250,ActPlayerFlag,0,RPort_PTR,4, _s);

        l = (uint32) (l / 3.0);
        if       (l<10)             { _s = PText[723]; }
        else if ((l>=10) && (l<20)) { _s = PText[724]; }
        else if ((l>=20) && (l<30)) { _s = PText[725]; }
        else if ((l>=30) && (l<40)) { _s = PText[726]; }
        else if ((l>=40) && (l<50)) { _s = PText[727]; }
        else if ((l>=50) && (l<60)) { _s = PText[728]; }
        else if ((l>=60) && (l<70)) { _s = PText[729]; }
        else if ((l>=70) && (l<80)) { _s = PText[730]; }
        else if ((l>=80) && (l<90)) { _s = PText[731]; }
        else if  (l>=90)            { _s = PText[732]; }
        WRITE(250,180,12,0,RPort_PTR,4, _s);
    }

    l=strlen(PText[735]);
    memcpy(s, PText[735], l);
    s[l++]=':';
    s[l++]=' ';
    (void)dez2out(Militaerausgaben[ActPlayer-1], 0, s+l);
    WRITE(20,280,12,0,RPort_PTR,4,s);

    l=strlen(PText[736]);
    memcpy(s, PText[736], l);
    s[l++]=':';
    s[l++]=' ';
    _s=dez2out((sint32) (Save.TechCosts[ActPlayer-1].data[Save.ActTech[ActPlayer-1]] / (AllCreative[ActPlayer-1]+1)), 0, s+l);
    *_s++=' ';
    strcpy(_s, _PT_Jahre);
    WRITE(20,300,12,0,RPort_PTR,4,s);

    l=strlen(PText[674]);
    memcpy(s, PText[674], l);
    s[l++]=':';
    s[l++]=' ';
    _s=dez2out(Save.ImperatorState[ActPlayer-1], 0, s+l);
    *_s++=' ';
    strcpy(_s, PText[414]);
    WRITE(20,320,12,0,RPort_PTR,4,s);

    // TODO ... debug-display???
    _s=dez2out(MaquesShips, 0, s);
    *_s++=' ';
    (void)dez2out(Save.WarPower[8], 0, _s);
    WRITE( 3, 2,12,0,RPort_PTR,1,s);
    WAITLOOP(false);

    CloseWindow(STA_Window);
}
