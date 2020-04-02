#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void SYSINFO(uint8 SysID, uint8 ThePlayerFlag)
{
    r_PlanetHeader* MyPlanetHeader;
    ByteArr42*      ActPProject;
    uint32  SysPop,Buildings;
    uint8   MyPlanets;
    uint8   i,j;
    char    s[80];
    char*   _s;

    if (0 == SysID) { return; }
    --SysID; // to shift the arrays
    RECT_RP0(0,30,250,480,360);
    SysPop = 0;
    MyPlanets = 0;
    Buildings = 0;
    for(i = 0; i < SystemHeader[SysID].Planets; ++i)
    {
        MyPlanetHeader = &(SystemHeader[SysID].PlanetMemA[i]);
        if ((MyPlanetHeader->PFlags & FLAG_CIV_MASK) == ThePlayerFlag)
        {
            SysPop += MyPlanetHeader->Population;
            ++MyPlanets;
            ActPProject = MyPlanetHeader->ProjectPtr;
            for(j = 1; j < 43; ++j)
            {
                if (0 < ActPProject->data[j]) { ++Buildings; }
            }
        }
    }
    if (0 < MyPlanets)
    {
        MAKEBORDER(MyScreen[0],30,250,480,360,12,6,1);

        strcpy(s, _PT_System);
        strcat(s, ": ");
        strcat(s, Save.SystemName.data[SysID]);
        WRITE(256,260,SystemFlags[0][SysID] & FLAG_CIV_MASK,WRITE_Center,MyRPort_PTR[0],3,s);

        strcpy(s, _PT_Bevoelkerung);
        strcat(s, ": ");
        _s = dez2out(SysPop, 0, s+strlen(s));
        *_s++=' ';
        strcpy(_s, PText[154]);
        WRITE(256,290,ThePlayerFlag,WRITE_Center,MyRPort_PTR[0],3,s);

        strcpy(s, _PT_Planeten);
        strcat(s, ": ");
        _s = dez2out(SystemHeader[SysID].Planets, 0, s+strlen(s));
        *_s++=','; *_s++=' ';
        strcpy(_s, _PT_davon);
        strcat(s, " ");
        _s = dez2out(MyPlanets, 0, s+strlen(s));
        *_s++=' ';
        strcpy(_s, GETCIVADJ(GETCIVVAR(ThePlayerFlag)));
        WRITE(256,310,ThePlayerFlag,WRITE_Center,MyRPort_PTR[0],3,s);

        Buildings = (Buildings / MyPlanets) +1;
        if      (3  > Buildings) { _s = _PT_praktisch_nicht_entwickelt; }
        else if (5  > Buildings) { _s = _PT_geringe_Entwicklungsstufe; }
        else if (10 > Buildings) { _s = _PT_maessig_hoch_entwickelt; }
        else if (18 > Buildings) { _s = _PT_hoch_entwickelt; }
        else if (33 < Buildings) { _s = _PT_hoechste_Entwicklungsstufe; }
        WRITE(256,330,ThePlayerFlag,WRITE_Center,MyRPort_PTR[0],3, _s);
    }
}
