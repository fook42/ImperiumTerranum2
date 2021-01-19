#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void CHECKPROJECTS(r_PlanetHeader* PlanetPtr, uint8 NewOwner)
{
    r_PlanetHeader* MyPlanetHeader;
    ByteArr42*      ActPProject;
    uint8           i;
    char            s[60];
    char*           _s;
    struct Window*   CPJ_Window;
    struct RastPort* RPort_PTR;

    MyPlanetHeader = PlanetPtr;
    ActPProject = MyPlanetHeader->ProjectPtr;
    for(i = 1; i < 8; ++i)
    {
        if (0 < ActPProject->data[i])
        {
            Save.ProjectCosts[GETCIVVAR(NewOwner)-1              ].data[i] = 0;
            Save.ProjectCosts[GETCIVVAR(MyPlanetHeader->PFlags)-1].data[i] = abs(Year)*i*11;
            if ((Save.CivPlayer[GETCIVVAR(NewOwner)-1              ] != 0)
             || (Save.CivPlayer[GETCIVVAR(MyPlanetHeader->PFlags)-1] != 0))
            {
                CPJ_Window=MAKEWINDOW(85,118,341,83,MyScreen[0]);
                if (NULL != CPJ_Window)
                {
                    RPort_PTR = CPJ_Window->RPort;
                    MAKEWINBORDER(RPort_PTR,0,0,340,82,12,6,1);

                    _s=my_strcpy(s, GETCIVNAME(GETCIVVAR(MyPlanetHeader->PFlags)));
                    *_s++=' ';
                    (void)my_strcpy(_s, PText[205]);
                    WRITE(171,12,MyPlanetHeader->PFlags & FLAG_CIV_MASK,(1|WRITE_Center),RPort_PTR,3,s);

                    _s=my_strcpy(s, Project.data[i]);
                    *_s++='-';
                    *_s=0;
                    WRITE(171,32,12,(1|WRITE_Center),RPort_PTR,3,s);

                    _s=my_strcpy(s, PText[206]);
                    *_s++=' ';
                    (void)my_strcpy(_s, GETCIVNAME(GETCIVVAR(NewOwner)));
                    WRITE(171,53,NewOwner & FLAG_CIV_MASK,(1|WRITE_Center),RPort_PTR,3,s);
                    if (Save.PlayMySelf)
                    {
                        Delay(PAUSE);
                    }
                    WAITLOOP(Save.PlayMySelf);
                    CloseWindow(CPJ_Window);
                }
            }
        }
    }
}
