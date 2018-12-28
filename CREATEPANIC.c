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
    uint8           i,k;
    APTR            ModC = NULL;
    uint32          ModL;
    //    uint32          l;  // ... todo ... not used?
    bool            PlanetLose, b;
    struct Window* CPA_Window;
    struct RastPort* RPort_PTR;

    PlanetLose = false;
    MyPlanetHeader = PPtr;

    ActPProjects = MyPlanetHeader->ProjectPtr;
    TheProject = 0;
    if (((Year % 10) == 0) && (((Year / 10)&1) == (PlanetNum&1)))
    {
        _s1 = PText[559];
        if (ActPProjects->data[41] == 0)
        {
            if (ActPProjects->data[30] == 0)
            {
                /* RECYCLINGANLAGE */
                if ((MyPlanetHeader->Population>5500) && (Save.TechCosts[ActPlayer-1].data[ProjectNeedsTech[30]] <= 0)
                        && (MyPlanetHeader->ProjectID != 30))
                {
                    TheProject = 30;
                    _s1 = PText[560];
                } else {
                    return;
                }
            } else if (ActPProjects->data[32] == 0)
            {
                /* HYDROKRAFTWERK */
                if ((MyPlanetHeader->Population>7000) && (Save.TechCosts[ActPlayer-1].data[ProjectNeedsTech[32]] <= 0)
                        && (MyPlanetHeader->ProjectID != 32))
                {
                    TheProject = 32;
                } else {
                    return;
                }
            } else if (ActPProjects->data[31] == 0)
            {
                /* FUSIONSKRAFTWERK */
                if ((MyPlanetHeader->Population>9000) && (Save.TechCosts[ActPlayer-1].data[ProjectNeedsTech[31]] <= 0)
                        && (MyPlanetHeader->ProjectID != 31))
                {
                    TheProject = 31;
                } else {
                    return;
                }
            } else if (ActPProjects->data[42] == 0)
            {
                if ((MyPlanetHeader->Population>11500) && (Save.TechCosts[ActPlayer-1].data[ProjectNeedsTech[42]] <= 0)
                        && (MyPlanetHeader->ProjectID != 42))
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
        ModC = GETTHESOUND(4);
        ModL = ModMemL[4];
    } else {
        if (((rand()%80) != 0) && (Warnung[ActPlayer] == 0)) { return; }
        MyPlanetHeader->Population = it_round(MyPlanetHeader->Population*0.895);
        if (Warnung[ActPlayer] != 0)
        {
            i = rand()%20;
            if ((i==2) || (i==3))
            {
                i = 0;
            }
        } else {
            i = rand()%10;
        }
        switch (i) {
        case 0: if (ActPProjects->data[28] == 0)
            {   /*kontinentale Union*/
                TheProject = 28;
                _s1 = PText[562];
            } else {
                return;
            }; break;

        case 1: if (ActPProjects->data[29] == 0)
            {   /*globale Union*/
                TheProject = 29;
                _s1 = PText[563];
            } else {
                return;
            }; break;

        case 2: if (Save.ProjectCosts[ActPlayer-1].data[4] != 0)
            {
                _s1 = PText[564];
            } else {
                return;
            }; break;

        case 3: _s1 = PText[565];
                break;
        default: if (((MyPlanetHeader->PFlags & FLAG_CIV_MASK) != MyPlanetHeader->Ethno) && (MyPlanetHeader->FirstShip.NextShip == NULL)
                    && (GetPlanetSys[GETCIVVAR(MyPlanetHeader->Ethno)-1] == 0))
            {
                if ((Warnung[ActPlayer] != 0) && ((rand()%10) != 0)) { return; }
                GetPlanet[GETCIVVAR(MyPlanetHeader->Ethno)-1] = MyPlanetHeader;
                GetPlanetSys[GETCIVVAR(MyPlanetHeader->Ethno)-1] = ActSys;
                CopyMem(MyPlanetHeader, &OldPlanet, sizeof(r_PlanetHeader));
                if (Save.CivPlayer[ActPlayer-1] != 0) { TheProject = -1; }
                _s1 = PText[566];
                if ((MyPlanetHeader->PFlags & FLAG_CIV_MASK) == ActPlayerFlag)
                {
                    _s2 = PText[567];
                }
                for(i = 0; i < (MAXCIVS-1); i++)
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
            } else if (((Warnung[ActPlayer] == 1) || (Warnung[ActPlayer] == 2)) && ((rand()%22) == 0))
            {
                if (!SETNEWPLANETOWNER(MyPlanetHeader, &OldPlanet, &TheProject, ActSys, &_s1, &_s2, &PlanetLose)) { return; }
            } else if ((Warnung[ActPlayer] == 2) && ((rand()%10) == 0))
            {
                if (!SETNEWPLANETOWNER(MyPlanetHeader, &OldPlanet, &TheProject, ActSys, &_s1, &_s2, &PlanetLose)) { return; }
            } else {
                return;
            }
        }
    }

//    l = GETCIVVAR(MyPlanetHeader->PFlags); // ... todo .. nonsense call ???

    if ((Save.CivPlayer[GETCIVVAR(MyPlanetHeader->PFlags)-1] != 0) || (TheProject == -1))
    {
        INFORMUSER();
        if ((-1 == TheProject) ||
            (28 == TheProject) ||
            (29 == TheProject))
        {
            ModC = GETTHESOUND(2);
            ModL = ModMemL[2];
        }

        CPA_Window=MAKEWINDOW(85,110,341,91,MyScreen[0]);
        if (NULL == CPA_Window)
        {
            return;
        }
        RPort_PTR = CPA_Window->RPort;

        MAKEWINBORDER(RPort_PTR,0,0,340,90,12,6,1);
        WRITEWIN(171,51,12,(1|WRITE_Center),RPort_PTR,3, _s1);
        strcpy(s, _PT_System);
        strcat(s, ": ");
        strcat(s, Save.SystemName.data[ActSys-1]);
        WRITEWIN(171, 7,MyPlanetHeader->PFlags & FLAG_CIV_MASK,(1|WRITE_Center),RPort_PTR,4,s);
        strcpy(s, _PT_Planet);
        strcat(s, ": ");
        strcat(s, MyPlanetHeader->PName);
        WRITEWIN(171,27,MyPlanetHeader->PFlags & FLAG_CIV_MASK,(1|WRITE_Center),RPort_PTR,4,s);
        if ((0 < TheProject) && (0 >= Save.TechCosts[ActPlayer-1].data[ProjectNeedsTech[TheProject]]))
        {
            strcpy(s, PText[570]);
            strcat(s, " ");
            strcat(s, Project.data[TheProject]);
            WRITEWIN(171,68,12,(1|WRITE_Center),RPort_PTR,3, s);
        }
        if (-1 == TheProject)
        {
            WRITEWIN(171,68,12,(1|WRITE_Center),RPort_PTR,3, _s2);
        }

        if ((Save.CivPlayer[GETCIVVAR(MyPlanetHeader->PFlags)-1] != 0)
                && (ActPlayer == GETCIVVAR(MyPlanetHeader->PFlags)))
        {
            MAKEBORDER(MyScreen[0],85,208,425,248,12,6,0);
            BltBitMapRastPort((struct BitMap*) &ImgBitMap7, MyPlanetHeader->Class*32,0,&(MyScreen[0]->RastPort),95,212,32,32,192);
            for(k = 1; k <= 2; k++) { DrawImage(&(MyScreen[0]->RastPort),&GadImg1,k*140,218); }
            WRITE(198,220,0,WRITE_Center,MyScreen[0],4,_PT_Planet);
            WRITE(338,220,0,WRITE_Center,MyScreen[0],4,_PT_weiter);
            b = false;
            if (!Save.PlayMySelf)
            {
                do
                {
                    delay(RDELAY);
                    if (LMB_PRESSED && (MouseY(0)>=218) && (MouseY(0)<=238))
                    {
                        if ((MouseX(0)>=140) && (MouseX(0)<=256))
                        {
                            KLICKGAD(140,218);
                            b = true;
                            HANDLEKNOWNPLANET(ActSys,0,MyPlanetHeader);
                        } else if ((MouseX(0)>=280) && (MouseX(0)<=396))
                        {
                            KLICKGAD(280,218);
                            b = true;
                        }
                    }
                }
                while(!b);
            }
            if (Save.PlayMySelf) { delay(PAUSE); }
            RECT(MyScreen[0],0,85,208,425,248);
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
