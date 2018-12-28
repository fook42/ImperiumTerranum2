#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void CHECKPLANET(r_PlanetHeader* MyPlanetHeader)
{
    uint8   HomeWorld,SplitWorld,i,j;
    r_ShipHeader*    MyShipPtr;
    char    s[60];
    int     l;
    struct Window* CHP_Window;
    struct RastPort* RPort_PTR;


    HomeWorld = 0;
    for(i = 1; i < (MAXCIVS-1); i++)
    {
        if ((!strcmp(PNames[i].data[2],MyPlanetHeader->PName)) && (GETCIVVAR(MyPlanetHeader->PFlags) == i))
        {
            HomeWorld = i;
        }
    }
    if (0 < HomeWorld)
    {
        /**** Eroberte Civi abspalten ****/
        SplitWorld = 0;
        for(i = 0; i < Save.Systems; i++)
        {
            for(j = 0; j < SystemHeader[i].Planets; j++)
            {
                MyPlanetHeader = &(SystemHeader[i].PlanetMemA[j]);
                
                if (GETCIVVAR(MyPlanetHeader->PFlags) == HomeWorld)
                {
                    if ((0 == SplitWorld) && (MyPlanetHeader->Ethno != (MyPlanetHeader->PFlags & FLAG_CIV_MASK)))
                    {
                        if (MyPlanetHeader->Ethno != (MyPlanetHeader->PFlags & FLAG_CIV_MASK))
                        {
                            SplitWorld = MyPlanetHeader->Ethno;
                        }
                        if (Save.CivPlayer[ActPlayer-1] != 0)
                        {
                            INFORMUSER();
                            CHP_Window=MAKEWINDOW(80,120,351,131,MyScreen[0]);
                            if (NULL != CHP_Window)
                            {
                                RPort_PTR = CHP_Window->RPort;
                                MAKEWINBORDER(RPort_PTR,0,0,350,130,12,6,1);
                                WRITEWIN(176,16,GETCIVFLAG(HomeWorld),(1|WRITE_Center),RPort_PTR,4,PText[208]);
                                strcpy(s, GETCIVNAME(HomeWorld));
                                l=strlen(s);
                                s[l++]=' ';
                                strcpy(s+l, PText[209]);
                                WRITEWIN(176,36,GETCIVFLAG(HomeWorld),(1|WRITE_Center),RPort_PTR,4,s);
                                WRITEWIN(176,56,GETCIVFLAG(HomeWorld),(1|WRITE_Center),RPort_PTR,4,PText[210]);
                                WRITEWIN(176,76,SplitWorld,(1|WRITE_Center),RPort_PTR,4,PText[211]);
                                strcpy(s, GETCIVADJ(GETCIVVAR(SplitWorld)));
                                strcat(s, PText[212]);
                                strcat(s, "!");
                                WRITEWIN(176,96,SplitWorld,(1|WRITE_Center),RPort_PTR,4,s);
                                if (Save.PlayMySelf)
                                {
                                    delay(PAUSE);
                                    delay(PAUSE);
                                }
                                WAITLOOP(Save.PlayMySelf);
                                CloseWindow(CHP_Window);
                            }
                        }
                    }
                    if (SplitWorld == MyPlanetHeader->Ethno)
                    {
                        MyPlanetHeader->PFlags = MyPlanetHeader->Ethno;
                        MyShipPtr = MyPlanetHeader->FirstShip.NextShip;
                        while (NULL != MyShipPtr)
                        {
                            MyShipPtr->Owner = (MyPlanetHeader->PFlags & FLAG_CIV_MASK);
                            MyShipPtr = MyShipPtr->NextShip;
                        }
                    }
                }
            }
        }
    }
}