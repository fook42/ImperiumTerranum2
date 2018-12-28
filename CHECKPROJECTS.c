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
    int             l;
    struct Window*   CPJ_Window;
    struct RastPort* RPort_PTR;

    MyPlanetHeader = PlanetPtr;
    ActPProject = MyPlanetHeader->ProjectPtr;
    for(i = 1; i < 8; i++)
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

                    strcpy(s, GETCIVNAME(GETCIVVAR(MyPlanetHeader->PFlags)));
                    l=strlen(s);
                    s[l++]=' ';
                    strcpy(s+l, PText[205]);
                    WRITEWIN(171,12,MyPlanetHeader->PFlags & FLAG_CIV_MASK,(1|WRITE_Center),RPort_PTR,4,s);

                    l=strlen(Project.data[i]);
                    memcpy(s, Project.data[i], l);
                    s[l++]='-';
                    s[l]=0;
                    WRITEWIN(171,32,12,(1|WRITE_Center),RPort_PTR,4,s);

                    l=strlen(PText[206]);
                    memcpy(s, PText[206], l);
                    s[l++]=' ';
                    strcpy(s+l, GETCIVNAME(GETCIVVAR(NewOwner)));
                    WRITEWIN(171,53,NewOwner & FLAG_CIV_MASK,(1|WRITE_Center),RPort_PTR,4,s);
                    if (Save.PlayMySelf)
                    {
                        delay(PAUSE);
                    }
                    WAITLOOP(Save.PlayMySelf);
                    CloseWindow(CPJ_Window);
                }
            }
        }
    }
}