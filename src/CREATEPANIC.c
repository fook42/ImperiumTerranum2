#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void CREATEPANIC(r_PlanetHeader* PPtr, uint8 ActSys, uint8 PlanetNum)
{
    ByteArr42*      ActPProjects;
    r_PlanetHeader* MyPlanetHeader;
    r_PlanetHeader  OldPlanet;
    sint16          TheProject;
    char            s[40];
    char*           _s1 = NULL;
    char*           _s2 = NULL;
    uint8           i;
    APTR            ModC = NULL;
    bool            PlanetLose, b;
    struct Window*  CPA_Window;
    struct Window*  CPA2_Window;
    struct RastPort* RPort_PTR;
    struct RastPort* RPort2_PTR;

    PlanetLose = false;
    MyPlanetHeader = PPtr;

    ActPProjects = MyPlanetHeader->ProjectPtr;
    TheProject = PROJECT_NONE;
    if ((0 == (Year % 10)) && (((Year / 10)&1) == (PlanetNum&1)))
    {
        // year == xyz0 and if z is even/odd same as the planetnumber
        // ... example: 1950 (plannum=1,3,5,7,9,11) or 1960 (plannum=2,4,6,8,10) ...etc
        _s1 = PText[559];
        if (0 != ActPProjects->data[PROJECT_MICROIDS])
        {
            // if Microids exists then we dont panic
            return;
        }
        if (0 == ActPProjects->data[PROJECT_RECYCLINGPLANT])
        {
            /* RECYCLINGANLAGE */
            if ((5500 >= MyPlanetHeader->Population) || (Save.TechCosts[ActPlayer-1][ProjectNeedsTech[PROJECT_RECYCLINGPLANT]] > 0)
                    || (PROJECT_RECYCLINGPLANT == MyPlanetHeader->ProjectID))
            {
                return;
            }
            TheProject = PROJECT_RECYCLINGPLANT;
            _s1 = PText[560];
        } else if (0 == ActPProjects->data[PROJECT_HYDROPOWER])
        {
            /* HYDROKRAFTWERK */
            if ((7000 >= MyPlanetHeader->Population) || (Save.TechCosts[ActPlayer-1][ProjectNeedsTech[PROJECT_HYDROPOWER]] > 0)
                    || (PROJECT_HYDROPOWER == MyPlanetHeader->ProjectID))
            {
                return;
            }
            TheProject = PROJECT_HYDROPOWER;
        } else if (0 == ActPProjects->data[PROJECT_FUSIONPOWER])
        {
            /* FUSIONSKRAFTWERK */
            if ((9000 >= MyPlanetHeader->Population) || (Save.TechCosts[ActPlayer-1][ProjectNeedsTech[PROJECT_FUSIONPOWER]] > 0)
                    || (PROJECT_FUSIONPOWER == MyPlanetHeader->ProjectID))
            {
                return;
            }
            TheProject = PROJECT_FUSIONPOWER;
        } else if (0 == ActPProjects->data[PROJECT_WEATHERSTATION])
        {
            if ((11500 >= MyPlanetHeader->Population) || (Save.TechCosts[ActPlayer-1][ProjectNeedsTech[PROJECT_WEATHERSTATION]] > 0)
                    || (PROJECT_WEATHERSTATION == MyPlanetHeader->ProjectID))
            {
                return;
            }
            TheProject = PROJECT_WEATHERSTATION;
            _s1 = PText[561];
        } else {
            return;
        }
        INFORMUSER();
        ModC = GETTHESOUND(3);
    } else {
        if ((0 != (rand()%80)) && (0 == Warnung[ActPlayer-1])) { return; }
        MyPlanetHeader->Population = it_round(MyPlanetHeader->Population*0.895);
        if (0 != Warnung[ActPlayer-1])
        {
            i = rand()%20;
            if ((2 == i) || (3 == i))
            {
                i = 0;
            }
        } else {
            i = rand()%10;
        }
        switch (i) {
        case 0:
            if (0 != ActPProjects->data[PROJECT_CONT_UNION])
            {   /*kontinentale Union*/
                return;
            }
            TheProject = PROJECT_CONT_UNION;
            _s1 = PText[562];   // Bürgerkrieg ausgebrochen
            break;

        case 1:
            if (0 != ActPProjects->data[PROJECT_GLOBAL_UNION])
            {   /*globale Union*/
                return;
            }
            TheProject = PROJECT_GLOBAL_UNION;
            _s1 = PText[563];   // Kriege zwischen den Nationen
            break;

        case 2:
            if (0 == Save.ProjectCosts[ActPlayer-1][4])
            {
                return;
            }
            _s1 = PText[564];   // Massenepidemie durch unbekannten Virus
            break;

        case 3:
            _s1 = PText[565];   // Klimakatastrophe durch Kometeneinschläge
            break;

        default:
            if ( ((MyPlanetHeader->PFlags & FLAG_CIV_MASK) != MyPlanetHeader->Ethno)
                && (NULL == MyPlanetHeader->FirstShip.NextShip)
                && (0 == GetPlanetSys[GETCIVVAR(MyPlanetHeader->Ethno)-1]))
            {
                if ((0 != Warnung[ActPlayer-1]) && (0 != (rand()%10))) { return; }
                GetPlanet[   GETCIVVAR(MyPlanetHeader->Ethno)-1] = MyPlanetHeader;
                GetPlanetSys[GETCIVVAR(MyPlanetHeader->Ethno)-1] = ActSys;
                memcpy(&OldPlanet, MyPlanetHeader, sizeof(r_PlanetHeader));
                if (0 != Save.CivPlayer[ActPlayer-1])
                {
                    TheProject = -1;
                }
                _s1 = PText[566];   // Schwere ethnische Konflikte ausgebrochen
                if ((MyPlanetHeader->PFlags & FLAG_CIV_MASK) == ActPlayerFlag)
                {
                    _s2 = PText[567];   // Unabhängigkeit wurde erklärt
                }
                for(i = 0; i < (MAXCIVS-1); ++i)
                {
                    if (Save.WarState[i][GETCIVVAR(MyPlanetHeader->Ethno)-1] == LEVEL_DIED)
                    {
                        Save.WarState[i][GETCIVVAR(MyPlanetHeader->Ethno)-1] = LEVEL_PEACE;
                    }
                }
                if (Save.WarState[GETCIVVAR(MyPlanetHeader->PFlags)-1][GETCIVVAR(MyPlanetHeader->Ethno)-1] == LEVEL_DIED)
                {
                    Save.WarState[GETCIVVAR(MyPlanetHeader->PFlags)-1][GETCIVVAR(MyPlanetHeader->Ethno)-1] = LEVEL_COLDWAR;
                }
                MyPlanetHeader->PFlags = MyPlanetHeader->Ethno;
                if ((MyPlanetHeader->PFlags & FLAG_CIV_MASK) == ActPlayerFlag)
                {
                    _s2 = PText[568];   // Übertritt zur Föderation wurde erklärt
                }
                PlanetLose = true;
            } else if (((1 == Warnung[ActPlayer-1])
                     || (2 == Warnung[ActPlayer-1]))
                     && (0 == (rand()%22)))
            {
                if (!SETNEWPLANETOWNER(MyPlanetHeader, &OldPlanet, &TheProject, ActSys, &_s1, &_s2, &PlanetLose)) { return; }
            } else if ((2 == Warnung[ActPlayer-1])
                     && (0 == (rand()%10)))
            {
                if (!SETNEWPLANETOWNER(MyPlanetHeader, &OldPlanet, &TheProject, ActSys, &_s1, &_s2, &PlanetLose)) { return; }
            } else {
                return;
            }
        }
    }

//    l = GETCIVVAR(MyPlanetHeader->PFlags); // ... todo .. nonsense call ???

    if ((0 != Save.CivPlayer[GETCIVVAR(MyPlanetHeader->PFlags)-1]) || (-1 == TheProject))
    {
        INFORMUSER();
        if ((-1 == TheProject) ||
            (PROJECT_CONT_UNION   == TheProject) ||
            (PROJECT_GLOBAL_UNION == TheProject))
        {
            if (NULL != ModC)
            {
                StopPlayer();
                UnLoadModule(ModC);
            }
            ModC = GETTHESOUND(1);
        }

        CPA_Window=MAKEWINDOWBORDER(85,110,341,91,MyScreen[0]);
        if (NULL == CPA_Window)
        {
            return;
        }
        RPort_PTR = CPA_Window->RPort;


        WRITE(171,51,12,(1|WRITE_Center),RPort_PTR,2, _s1);
        _s1=my_strcpy(s, _PT_System);
        *_s1++ = ':';
        *_s1++ = ' ';
        (void) my_strcpy(_s1, Save.SystemName[ActSys-1]);
        WRITE(171, 7,MyPlanetHeader->PFlags & FLAG_CIV_MASK,(1|WRITE_Center),RPort_PTR,3,s);
        _s1=my_strcpy(s, _PT_Planet);
        *_s1++ = ':';
        *_s1++ = ' ';
        (void) my_strcpy(_s1, MyPlanetHeader->PName);
        WRITE(171,27,MyPlanetHeader->PFlags & FLAG_CIV_MASK,(1|WRITE_Center),RPort_PTR,3,s);
        if ((PROJECT_NONE < TheProject) && (0 >= Save.TechCosts[ActPlayer-1][ProjectNeedsTech[TheProject]]))
        {
            _s1=my_strcpy(s, PText[570]);   // Bürger fordern
            *_s1++ = ' ';
            (void) my_strcpy(_s1, ProjectName[TheProject]);
            WRITE(171,68,12,(1|WRITE_Center),RPort_PTR,2, s);
        }
        if (-1 == TheProject)
        {
            WRITE(171,68,12,(1|WRITE_Center),RPort_PTR,2, _s2);
        }

        if ((0 != Save.CivPlayer[GETCIVVAR(MyPlanetHeader->PFlags)-1])
                && (ActPlayer == GETCIVVAR(MyPlanetHeader->PFlags)))
        {
            CPA2_Window=MAKEWINDOWBORDER(85,208,341,41,MyScreen[0]);
            if (NULL == CPA2_Window)
            {
                return;
            }
            RPort2_PTR = CPA2_Window->RPort;



            BltBitMapRastPort((struct BitMap*) &ImgBitMap7, MyPlanetHeader->Class*32,0,RPort2_PTR,10,4,32,32,192);
            DrawImage(RPort2_PTR,&GadImg1, 55,10);
            DrawImage(RPort2_PTR,&GadImg1,195,10);
            WRITE(113,12,0,WRITE_Center,RPort2_PTR,3,_PT_Planet);
            WRITE(253,12,0,WRITE_Center,RPort2_PTR,3,_PT_weiter);
            b = false;
            if (!Save.PlayMySelf)
            {
                do
                {
                    Delay(RDELAY);
                    if (LMB_PRESSED && (9 < CPA2_Window->MouseY) && (31 > CPA2_Window->MouseY))
                    {
                        if ((54 < CPA2_Window->MouseX) && (172 > CPA2_Window->MouseX))
                        {
                            KLICKWINGAD(RPort2_PTR,  55, 10);
                            b = true;
                            HANDLEKNOWNPLANET(ActSys,0,MyPlanetHeader);
                        } else if ((194 < CPA2_Window->MouseX) && (312 > CPA2_Window->MouseX))
                        {
                            KLICKWINGAD(RPort2_PTR, 195, 10);
                            b = true;
                        }
                    }
                }
                while(!b);
            }
            if (Save.PlayMySelf) { Delay(PAUSE); }
            CloseWindow(CPA2_Window);
        } else {
            if (Save.PlayMySelf) { Delay(PAUSE); }
            WAITLOOP(Save.PlayMySelf);
        }
        CloseWindow(CPA_Window);

        if (PlanetLose)
        {
            CHECKPROJECTS(&OldPlanet, MyPlanetHeader->PFlags);
        }
    }
    if (NULL != ModC)
    {
        StopPlayer();
        UnLoadModule(ModC);
        FreePlayer();
    }

}
