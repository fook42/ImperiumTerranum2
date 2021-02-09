#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void AUTOVERHANDLUNG(const int CivFlag1, const int CivFlag2, const int Mode)
{
    int     CivVar1, CivVar2;

    // no negotiations when Ceborgs are active or unknown civilization
    if ((WFLAG_CEBORC == Save.WorldFlag)
        && ((FLAG_OTHER == CivFlag1) || (FLAG_OTHER == CivFlag2)))
    {
        return;
    }
    CivVar1 = GETCIVVAR(CivFlag1);
    CivVar2 = GETCIVVAR(CivFlag2);
    // no negotiations when unknown civ or maques involved
    if ((CIVVAR_NONE == CivVar1) || (CIVVAR_MAQUES == CivVar1)
     || (CIVVAR_NONE == CivVar2) || (CIVVAR_MAQUES == CivVar2))
    {
        return;
    }
    if (((uint8) CivVar1 == ActPlayer) || ((uint8) CivVar2 == ActPlayer))
    {
        INFORMUSER();
    }

    --CivVar1;
    --CivVar2;
    if ((!Save.PlayMySelf)
        && ((0 != Save.CivPlayer[CivVar1]) || (0 != Save.CivPlayer[CivVar2])))
    {
        if ((MODE_TERRITORIUM == Mode) || (MODE_BELEIDIGUNG == Mode))
        {
            if ((CivFlag1 == ActPlayerFlag) || (CivFlag2 == ActPlayerFlag))
            {
                VERHANDLUNG(CivFlag1,Mode);
                REFRESHDISPLAY();
            }
        } else {
            if (CivFlag1 == ActPlayerFlag)
            {
                VERHANDLUNG(CivFlag2,MODE_OFFENSIV);
                REFRESHDISPLAY();
            } else if (CivFlag2 == ActPlayerFlag)
            {
                VERHANDLUNG(CivFlag1,MODE_DEFENSIV);
                REFRESHDISPLAY();
            }
        }
        return;
    }
    if (0 != (Year % 4)) { return; }
    if ((7 == CivVar1) && (WFLAG_JAHADR == Save.WorldFlag)
        && (Save.WarPower[7] > (Save.WarPower[CivVar2]*2)))
    {
        if ((Save.JSteuer[CivVar2] > 0) && (Save.JSteuer[CivVar2] < 100))
        {
            ++(Save.JSteuer[CivVar2]);
        } else {
            Save.JSteuer[CivVar2] = (uint8) (2+2*(Save.WarPower[7] / (double) (Save.WarPower[CivVar2]+1)));
            if (Save.JSteuer[CivVar2] > 53)
                { Save.JSteuer[CivVar2] = 53; }
        }
        GOTOPEACE(CivVar1,CivVar2);
    } else {
        Save.JSteuer[CivVar2] = 0;
    }
    return;
    /* TODO ... we "return" in either case .. so dead code below this line??? */
/*    if (Save.WarPower[CivVar1] > (Save.WarPower[CivVar2]*5))
    {
        return;
    }
    if (Save.WarPower[CivVar1] > (Save.WarPower[CivVar2]*3))
    {
        if (TAKETECH(CivFlag1,CivFlag2))
        {
            GOTOPEACE(CivVar1,CivVar2);
            return;
        }
    }
    if (Save.WarPower[CivVar1] > (Save.WarPower[CivVar2]*2))
    {
        Save.Staatstopf[CivVar1] += abs(Year*10);
        Save.Staatstopf[CivVar2] -= abs(Year*10);
        GOTOPEACE(CivVar1,CivVar2);
    }
*/
}
