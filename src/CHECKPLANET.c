#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void CHECKPLANET(const r_PlanetHeader* MyPlanetHeader)
{
    uint8   HomeWorld,SplitWorld,i,j;
    r_ShipHeader*   CHP_MyShipPtr;
    r_PlanetHeader* CHP_MyPlanetHeader;
    char    s[60];
    char*   _s;
    struct Window*  CHP_Window;
    struct RastPort* RPort_PTR;

    HomeWorld = 0;
    for(i = 1; i < (MAXCIVS-1); ++i)
    {
        if ((!strcmp(PNames[i].data[2],MyPlanetHeader->PName)) && (GETCIVVAR(MyPlanetHeader->PFlags) == i))
        {
            HomeWorld = i;
            break;
        }
    }
    if (0 != HomeWorld)
    {
        /**** Eroberte Civi abspalten ****/
        SplitWorld = 0;
        for(i = 0; i < Save.Systems; ++i)
        {
            for(j = 0; j < SystemHeader[i].Planets; ++j)
            {
                CHP_MyPlanetHeader = &(SystemHeader[i].PlanetMemA[j]);

                if (GETCIVVAR(CHP_MyPlanetHeader->PFlags) == HomeWorld)
                {
                    if ((0 == SplitWorld)
                        && (CHP_MyPlanetHeader->Ethno != (CHP_MyPlanetHeader->PFlags & FLAG_CIV_MASK)))
                    {
                        SplitWorld = CHP_MyPlanetHeader->Ethno;
                        if (0 != Save.CivPlayer[ActPlayer-1])
                        {
                            INFORMUSER();
                            CHP_Window=MAKEWINDOWBORDER(80,120,351,131,MyScreen[0]);
                            if (NULL != CHP_Window)
                            {
                                RPort_PTR = CHP_Window->RPort;

                                WRITE(176,16,GETCIVFLAG(HomeWorld),(1|WRITE_Center),RPort_PTR,3,PText[208]);
                                _s=my_strcpy(s, GETCIVNAME(HomeWorld));
                                *_s++ = ' ';
                                (void) my_strcpy(_s, PText[209]);
                                WRITE(176,36,GETCIVFLAG(HomeWorld),(1|WRITE_Center),RPort_PTR,3,s);
                                WRITE(176,56,GETCIVFLAG(HomeWorld),(1|WRITE_Center),RPort_PTR,3,PText[210]);
                                WRITE(176,76,           SplitWorld,(1|WRITE_Center),RPort_PTR,3,PText[211]);
                                _s=my_strcpy( s, GETCIVADJ(GETCIVVAR(SplitWorld)));
                                _s=my_strcpy(_s, PText[212]);
                                *_s++ = '!';
                                *_s = 0;
                                WRITE(176,96,           SplitWorld,(1|WRITE_Center),RPort_PTR,3,s);
                                if (Save.PlayMySelf)
                                {
                                    Delay(PAUSE);
                                    Delay(PAUSE);
                                }
                                WAITLOOP(Save.PlayMySelf);
                                CloseWindow(CHP_Window);
                            }
                        }
                    }
                    if (CHP_MyPlanetHeader->Ethno == SplitWorld)
                    {
                        CHP_MyPlanetHeader->PFlags = CHP_MyPlanetHeader->Ethno;
                        CHP_MyShipPtr = CHP_MyPlanetHeader->FirstShip.NextShip;
                        while (NULL != CHP_MyShipPtr)
                        {
                            CHP_MyShipPtr->Owner = (CHP_MyPlanetHeader->PFlags & FLAG_CIV_MASK);
                            CHP_MyShipPtr = CHP_MyShipPtr->NextShip;
                        }
                    }
                }
            }
        }
    }
}
