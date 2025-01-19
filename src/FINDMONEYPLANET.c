#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

uint8 FINDMONEYPLANET(uint8 CivFlag, uint8 CivVar, char* s)
{
    r_PlanetHeader* MyPlanetHeader;
    ByteArr42*      ActPProject;
    uint8           i,j,k,l,SysID,PlanetID,Objects,XProjectID;
    char*           _s;

    SysID = MAXSYSTEMS;
    PlanetID = 0;
    Objects = 0;
    XProjectID = 0;
    for(i = 0; i < Save.Systems; ++i)
    {
        if (0 == SystemHeader[i].Planets) { continue; }

        for(j = 0; j < SystemHeader[i].Planets; ++j)
        {
            MyPlanetHeader = &(SystemHeader[i].PlanetMemA[j]);
            l = 0;
            if ((MyPlanetHeader->PFlags & FLAG_CIV_MASK) == CivFlag)
            {
                ActPProject = MyPlanetHeader->ProjectPtr;
                l = ActPProject->data[PROJECT_SPACEDOCK];
                for(k = PROJECT_CONT_UNION; k < PROJECT_NOMORE; ++k)
                {
                    if (0 < ActPProject->data[k])
                    {
                        ++l;
                    }
                }
            }
            if (l > Objects)
            {
                SysID = i;
                PlanetID = j;
                Objects = l;
            }
        }
    }
    if (0 == Objects) { return 0; }

    if (MAXSYSTEMS != SysID)
    {
        MyPlanetHeader = &(SystemHeader[SysID].PlanetMemA[PlanetID]);
        ActPProject = MyPlanetHeader->ProjectPtr;
        _s=my_strcpy(s, _PT_System);
        *_s++ = ':';
        *_s++ = ' ';
        _s=my_strcpy(_s, Save.SystemName[SysID]);
        *_s++ = ' ';
        *_s++ = ' ';
        *_s++ = ' ';
        _s=my_strcpy(_s, _PT_Planet);
        *_s++ = ':';
        *_s++ = ' ';
        (void) my_strcpy(_s, MyPlanetHeader->PName);

        do
        {
            XProjectID = (rand()%42)+1;
        }
        while (((PROJECT_SPACEDOCK != XProjectID) && ((PROJECT_CONT_UNION > XProjectID) || (PROJECT_WEATHERSTATION < XProjectID)))
             || (1 > ActPProject->data[XProjectID]));
        ActPProject->data[XProjectID] = 0;
        Save.Staatstopf[CivVar] += (Save.ProjectCosts[CivVar][XProjectID] / 29);
    }
    return XProjectID;
}
