#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void DOINFLATION(uint8 DI_ActPlayer)
{
    int j;
    for(j = 1; j<43; j++)
    {
        Save.ProjectCosts[DI_ActPlayer].data[j] = it_round(Save.ProjectCosts[DI_ActPlayer].data[j]*INFLATION);
        Save.TechCosts[   DI_ActPlayer].data[j] = it_round(Save.TechCosts[   DI_ActPlayer].data[j]*INFLATION);
    }
}
