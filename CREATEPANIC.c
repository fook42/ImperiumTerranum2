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
    uint32          ModL;
    bool            PlanetLose, b;
    struct Window*  CPA_Window;
    struct Window*  CPA2_Window;
    struct RastPort* RPort_PTR;
    struct RastPort* RPort2_PTR;

    PlanetLose = false;
    MyPlanetHeader = PPtr;

    ActPProjects = MyPlanetHeader->ProjectPtr;
    TheProject = 0;
    if ((0 == (Year % 10)) && (((Year / 10)&1) == (PlanetNum&1)))
    {
        _s1 = PText[559];
        if (0 == ActPProjects->data[41])
        {
            if (0 == ActPProjects->data[30])
            {
                /* RECYCLINGANLAGE */
                if ((5500 < MyPlanetHeader->Population) && (Save.TechCosts[ActPlayer-1].data[ProjectNeedsTech[30]] <= 0)
                        && (30 != MyPlanetHeader->ProjectID))
                {
                    TheProject = 30;
                    _s1 = PText[560];
                } else {
                    return;
                }
            } else if (0 == ActPProjects->data[32])
            {
                /* HYDROKRAFTWERK */
                if ((7000 < MyPlanetHeader->Population) && (Save.TechCosts[ActPlayer-1].data[ProjectNeedsTech[32]] <= 0)
                        && (32 != MyPlanetHeader->ProjectID))
                {
                    TheProject = 32;
                } else {
                    return;
                }
            } else if (0 == ActPProjects->data[31])
            {
                /* FUSIONSKRAFTWERK */
                if ((9000 < MyPlanetHeader->Population) && (Save.TechCosts[ActPlayer-1].data[ProjectNeedsTech[31]] <= 0)
                        && (31 != MyPlanetHeader->ProjectID))
                {
                    TheProject = 31;
                } else {
                    return;
                }
            } else if (0 == ActPProjects->data[42])
            {
                if ((11500 < MyPlanetHeader->Population) && (Save.TechCosts[ActPlayer-1].data[ProjectNeedsTech[42]] <= 0)
                        && (42 != MyPlanetHeader->ProjectID))
                {
                    TheProject = 42;
                    _s1 = PText[561];
                } else {
                    return;
                }
            } else {
                return;
            }
        } else {
            return;
        }
        INFORMUSER();
        ModC = GETTHESOUND(3);
        ModL = ModMemL[3];
    } else {
        if ((0 != (rand()%80)) && (0 == Warnung[ActPlayer])) { return; }
        MyPlanetHeader->Population = it_round(MyPlanetHeader->Population*0.895);
        if (0 != Warnung[ActPlayer])
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
        case 0: if (0 == ActPProjects->data[28])
            {   /*kontinentale Union*/
                TheProject = 28;
                _s1 = PText[562];
            } else {
                return;
            }; break;

        case 1: if (0 == ActPProjects->data[29])
            {   /*globale Union*/
                TheProject = 29;
                _s1 = PText[563];
            } else {
                return;
            }; break;

        case 2: if (0 != Save.ProjectCosts[ActPlayer-1].data[4])
            {
                _s1 = PText[564];
            } else {
                return;
            }; break;

        case 3: _s1 = PText[565];
                break;
        default: if ( ((MyPlanetHeader->PFlags & FLAG_CIV_MASK) != MyPlanetHeader->Ethno)
                     && (NULL == MyPlanetHeader->FirstShip.NextShip)
                     && (0 == GetPlanetSys[GETCIVVAR(MyPlanetHeader->Ethno)-1]))
            {
                if ((0 != Warnung[ActPlayer]) && (0 != (rand()%10))) { return; }
                GetPlanet[   GETCIVVAR(MyPlanetHeader->Ethno)-1] = MyPlanetHeader;
                GetPlanetSys[GETCIVVAR(MyPlanetHeader->Ethno)-1] = ActSys;
                memcpy(&OldPlanet, MyPlanetHeader, sizeof(r_PlanetHeader));
                if (0 != Save.CivPlayer[ActPlayer-1]) { TheProject = -1; }
                _s1 = PText[566];
                if ((MyPlanetHeader->PFlags & FLAG_CIV_MASK) == ActPlayerFlag)
                {
                    _s2 = PText[567];
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
                    _s2 = PText[568];
                }
                PlanetLose = true;
            } else if (((1 == Warnung[ActPlayer])
                     || (2 == Warnung[ActPlayer]))
                     && (0 == (rand()%22)))
            {
                if (!SETNEWPLANETOWNER(MyPlanetHeader, &OldPlanet, &TheProject, ActSys, &_s1, &_s2, &PlanetLose)) { return; }
            } else if ((2 == Warnung[ActPlayer])
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
            (28 == TheProject) ||
            (29 == TheProject))
        {
            ModC = GETTHESOUND(1);
            ModL = ModMemL[1];
        }

        CPA_Window=MAKEWINDOW(85,110,341,91,MyScreen[0]);
        if (NULL == CPA_Window)
        {
            return;
        }
        RPort_PTR = CPA_Window->RPort;

        MAKEWINBORDER(RPort_PTR,0,0,340,90,12,6,1);
        WRITE(171,51,12,(1|WRITE_Center),RPort_PTR,2, _s1);
        strcpy(s, _PT_System);
        strcat(s, ": ");
        strcat(s, Save.SystemName.data[ActSys-1]);
        WRITE(171, 7,MyPlanetHeader->PFlags & FLAG_CIV_MASK,(1|WRITE_Center),RPort_PTR,3,s);
        strcpy(s, _PT_Planet);
        strcat(s, ": ");
        strcat(s, MyPlanetHeader->PName);
        WRITE(171,27,MyPlanetHeader->PFlags & FLAG_CIV_MASK,(1|WRITE_Center),RPort_PTR,3,s);
        if ((0 < TheProject) && (0 >= Save.TechCosts[ActPlayer-1].data[ProjectNeedsTech[TheProject]]))
        {
            strcpy(s, PText[570]);
            strcat(s, " ");
            strcat(s, Project.data[TheProject]);
            WRITE(171,68,12,(1|WRITE_Center),RPort_PTR,2, s);
        }
        if (-1 == TheProject)
        {
            WRITE(171,68,12,(1|WRITE_Center),RPort_PTR,2, _s2);
        }

        if ((0 != Save.CivPlayer[GETCIVVAR(MyPlanetHeader->PFlags)-1])
                && (ActPlayer == GETCIVVAR(MyPlanetHeader->PFlags)))
        {
            CPA2_Window=MAKEWINDOW(85,208,341,41,MyScreen[0]);
            if (NULL == CPA2_Window)
            {
                return;
            }
            RPort2_PTR = CPA2_Window->RPort;

            MAKEWINBORDER(RPort2_PTR,0,0,340,40,12,6,1);

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
                    delay(RDELAY);
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
            if (Save.PlayMySelf) { delay(PAUSE); }
            CloseWindow(CPA2_Window);
        } else {
            if (Save.PlayMySelf) { delay(PAUSE); }
            WAITLOOP(Save.PlayMySelf);
        }
        CloseWindow(CPA_Window);

        if (PlanetLose)
        {
            CHECKPROJECTS(&OldPlanet, MyPlanetHeader->PFlags);
        }
    }
    if (ModC != 0)
    {
        StopPlayer();
        FreeMem(ModC,ModL);
    }

}
