#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

uint8 FINDMONEYPLANET(uint8 CivFlag, uint8 CivVar, char* s)
{
    r_PlanetHeader* MyPlanetHeader;
    ByteArr42*      ActPProject;
    uint8           i,j,k,l,SysID,PID,Objects,XProjectID;

    SysID = 0;
    PID = 0;
    Objects = 0;
    XProjectID = 0;
    for(i = 0; i < Save.Systems; i++)
    {
        if (SystemHeader[i].Planets>0)
        {
            for(j = 0; j < SystemHeader[i].Planets; j++)
            {
                MyPlanetHeader = &(SystemHeader[i].PlanetMemA[j]);
                l = 0;
                if ((MyPlanetHeader->PFlags & FLAG_CIV_MASK) == CivFlag)
                {
                    ActPProject = MyPlanetHeader->ProjectPtr;
                    l = ActPProject->data[25];
                    for(k = 28; k <= 42; k++)
                    {
                        if (ActPProject->data[k] > 0 )
                        {
                            l++;
                        }
                    }
                }
                if (l > Objects)
                {
                    SysID = i+1;
                    PID = j;
                    Objects = l;
                }
            }
        }
    }
    if (0 == Objects) { return 0; }
    if (SysID>0)
    {
        MyPlanetHeader = &(SystemHeader[SysID-1].PlanetMemA[PID]);
        ActPProject = MyPlanetHeader->ProjectPtr;
        strcpy(s, _PT_System);
        strcat(s, ": ");
        strcat(s, Save.SystemName.data[SysID-1]);
        strcat(s, "   ");
        strcat(s, _PT_Planet);
        strcat(s, ": ");
        strcat(s, MyPlanetHeader->PName);
        do
        {
            XProjectID = (rand()%42)+1;
        }
        while (((XProjectID != 25) && ((XProjectID < 28) || (XProjectID >42))) || (ActPProject->data[XProjectID]<=0));
        ActPProject->data[XProjectID] = 0;
        Save.Staatstopf[CivVar] += (Save.ProjectCosts[CivVar].data[XProjectID] / 29);
    }
    return XProjectID;
}
