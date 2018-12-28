#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void CEBORCATTACK(uint8 CA_ActPlayerFlag)
{
    int     i,j,random_wert;
    uint8   l;
    r_ShipHeader*   CA_MyShipPtr;

    if ((0            != Save.WorldFlag)
     && (WFLAG_CEBORC != Save.WorldFlag))
    {
        return;
    }
    Save.WorldFlag = WFLAG_CEBORC;

    if (0 != CA_ActPlayerFlag)
    {
        for(i = 0; i < Save.Systems; i++)
        {
            if ((SystemFlags[0][i] & FLAG_CIV_MASK) == CA_ActPlayerFlag)
            {
                break;
            }
        }
        if (i == Save.Systems) { return; }
    }

    j = rand() % Save.Systems;

    if (0 != CA_ActPlayerFlag)
    {
        while ((SystemFlags[0][j] & FLAG_CIV_MASK) != CA_ActPlayerFlag)
        {
            j = rand() % Save.Systems;
        }
    }
    for(i = 0; i < (MAXCIVS-1); i++)
    {
        Save.WarState[7][i] = LEVEL_COLDWAR;
    }
    random_wert = (rand()%11)+6;
    for(i = 0; i < random_wert; i++)
    {
        Save.Bevoelkerung[7]++;
        CA_MyShipPtr = (r_ShipHeader*) AllocMem(sizeof(r_ShipHeader), MEMF_CLEAR);
        if (NULL != CA_MyShipPtr)
        {
            if ((rand()%100)<40)
            {
                l = 20;
            } else {
                l = 23;
            }
            *CA_MyShipPtr = (r_ShipHeader) {0,l,FLAG_OTHER,0,100,1,0,(rand()%20)-40,(rand()%20)-40,
                                            ShipData(l).MaxShield,WEAPON_PTORPEDO,1,
                                            ShipData(l).MaxMove,0,0,0,NULL,NULL,NULL};
            LINKSHIP(CA_MyShipPtr, &(SystemHeader[j].FirstShip), 0);   // TODO: where does j come from?
        }
    }
}
