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
    int     i, j, posy;

    const char* StimmungTexts[] = { PText[712],PText[713],PText[714],PText[715],PText[716],
                                    PText[717],PText[718],PText[719],PText[720],PText[711]};

    const char* EntwicklungTexts[] = {  PText[723],PText[724],PText[725],PText[726],PText[727],
                                        PText[728],PText[729],PText[730],PText[731],PText[732]};

    struct Window* STA_Window;
    struct RastPort* RPort_PTR;
    STA_Window = MAKEWINDOWBORDER(10,30,491,361,MyScreen[0]);
    if (NULL == STA_Window)
    {
        return;
    }
    RPort_PTR = STA_Window->RPort;


    --ActPlayer; // to shift the array

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
        if (SystemFlags[ActPlayer][i] & FLAG_KNOWN)
        {
            for (j = 0; j < SystemHeader[i].Planets; ++j)
            {
                MyPlanet = &(SystemHeader[i].PlanetMemA[j]);
                if ((MyPlanet->PFlags & FLAG_CIV_MASK) != ActPlayerFlag) { continue; }

                ActPProjects = MyPlanet->ProjectPtr;
                Planeten++;
                Kreativitaet   += ActPProjects->data[PROJECT_PART_ACCEL]+ActPProjects->data[PROJECT_INTERNET]
                                    +ActPProjects->data[PROJECT_VIRT_UNIVERSITY]+ActPProjects->data[PROJECT_INFO_HIGHWAY]
                                    +ActPProjects->data[PROJECT_WEATHERSTATION];
                Produktivitaet += ActPProjects->data[PROJECT_FUSIONPOWER]+ActPProjects->data[PROJECT_INT_PLANT]
                                    +ActPProjects->data[PROJECT_INFO_HIGHWAY]+ActPProjects->data[PROJECT_MICROIDS]
                                    +ActPProjects->data[PROJECT_WEATHERSTATION];
                Buildings      += ActPProjects->data[PROJECT_RECYCLINGPLANT]+ActPProjects->data[PROJECT_FUSIONPOWER]
                                    +ActPProjects->data[PROJECT_INTERNET]+ActPProjects->data[PROJECT_VIRT_UNIVERSITY]
                                    +ActPProjects->data[PROJECT_INT_PLANT]+ActPProjects->data[PROJECT_INFO_HIGHWAY]
                                    +ActPProjects->data[PROJECT_VON_NEUMANN]+ActPProjects->data[PROJECT_MICROIDS];
                if (0 < ActPProjects->data[PROJECT_SDI])          { ++Buildings; }
                if (0 < ActPProjects->data[PROJECT_SPACEPHALANX]) { ++Buildings; }
                Bio     += MyPlanet->Biosphaere;
                Infra   += MyPlanet->Infrastruktur;
                Ind     += MyPlanet->Industrie;
                Groesse += MyPlanet->Size;
                if ((0 != MyPlanet->Ethno) && ((MyPlanet->PFlags & FLAG_CIV_MASK) != MyPlanet->Ethno))
                {
                    ++Eth;
                }
            }
        }
    }

    posy = 20;
    for (i = 0; i < 9; ++i)
    {
        WRITE(20,posy,12,JAM1,RPort_PTR,3, PText[700 + i]);
        posy += 20;
    }
    WRITE(20,210,ActPlayerFlag,JAM1,RPort_PTR,3,_PT_Status);
    WRITE(50,230,ActPlayerFlag,JAM1,RPort_PTR,3,PText[710]);
    WRITE(50,250,ActPlayerFlag,JAM1,RPort_PTR,3,PText[711]);

    if (0 < Planeten)
    {
        _s = float2out( ((double)(20*Kreativitaet)/Planeten), 0, 2, s);
        *_s++ = '%';
        *_s  =0;
        WRITE(355,20,8,WRITE_Right|JAM1,RPort_PTR,1,s);

        _s = float2out( ((double)(20*Produktivitaet)/Planeten), 0, 2, s);
        *_s++ = '%';
        *_s  =0;
        WRITE(355,40,8,WRITE_Right|JAM1,RPort_PTR,1,s);

        _s = float2out( ((double)Bio/Planeten/2.0f), 0, 2, s);
        *_s++ = '%';
        *_s  =0;
        WRITE(355,60,8,WRITE_Right|JAM1,RPort_PTR,1,s);

        _s = float2out( ((double)Infra/Planeten/2.0f), 0, 2, s);
        *_s++ = '%';
        *_s  =0;
        WRITE(355,80,8,WRITE_Right|JAM1,RPort_PTR,1,s);

        _s = float2out( ((double)Ind/2.0f/Planeten), 0, 2, s);
        *_s++ = '%';
        *_s  =0;
        WRITE(355,100,8,WRITE_Right|JAM1,RPort_PTR,1,s);

        (void) dez2out(it_round(Save.Bevoelkerung[ActPlayer]/(double) Planeten), 0, s);
        WRITE(340,120,8,WRITE_Right|JAM1,RPort_PTR,1,s);

        (void) dez2out(it_round((double)Groesse/(double)Planeten/10.0f), 0, s);
        WRITE(340,140,8,WRITE_Right|JAM1,RPort_PTR,1,s);

        _s = float2out( ((double)Eth/Planeten*100.0), 0, 2, s);
        *_s++ = '%';
        *_s  =0;
        WRITE(355,160,8,WRITE_Right|JAM1,RPort_PTR,1,s);

        l = it_round((Buildings/Planeten*10.0)+(Bio/Planeten/2.0)+(Infra/Planeten/2.0));
        _s = float2out( ((double)l/3.1), 0, 2, s);
        *_s++ = '%';
        *_s  =0;
        WRITE(375,230,8,WRITE_Right|JAM1,RPort_PTR,1,s);

        _s = StimmungTexts[it_round(l/31.0)];
        WRITE(270,250,ActPlayerFlag,JAM1,RPort_PTR,3, _s);

        l = (uint32) (l / 30.0);
        if (9 < l) { l = 9; }
        _s = EntwicklungTexts[l];
        WRITE(250,180,12,JAM1,RPort_PTR,3, _s);
    }

    _s=my_strcpy(s, PText[735]);
    *_s++ = ':';
    *_s++ = ' ';
    (void) dez2out(Militaerausgaben[ActPlayer], 0, _s);
    WRITE(20,280,12,JAM1,RPort_PTR,3,s);

    _s=my_strcpy(s, PText[736]);
    *_s++ = ':';
    *_s++ = ' ';
    _s=dez2out((sint32) (Save.TechCosts[ActPlayer][Save.ActTech[ActPlayer]] / (AllCreative[ActPlayer]+1)), 0, _s);
    *_s++ = ' ';
    (void) my_strcpy(_s, _PT_Jahre);
    WRITE(20,300,12,JAM1,RPort_PTR,3,s);

    _s=my_strcpy(s, PText[674]);
    *_s++ = ':';
    *_s++ = ' ';
    _s=dez2out(Save.ImperatorState[ActPlayer], 0, _s);
    *_s++ = ' ';
    (void) my_strcpy(_s, PText[414]);
    WRITE(20,320,12,JAM1,RPort_PTR,3,s);

    // TODO ... debug-display???
/*    _s=dez2out(MaquesShips, 0, s);
    *_s++ = ' ';
    (void) dez2out(Save.WarPower[8], 0, _s);
    WRITE( 3, 2,12,0,RPort_PTR,0,s);
*/

    WAITLOOP(false);
    CloseWindow(STA_Window);

    ++ActPlayer; // to shift the array
}
