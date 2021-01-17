#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void DOINFLATION(uint8 DI_ActPlayer)
{
    int j;

    for(j = 1; j<43; j++)
    {
        // costs += (costs>>12) * 0x7B;    // = costs+(costs>>6)+(costs>>7)+(costs>>8)+(costs>>9)+(costs>>11)++(costs>>12);
        // =~  costs* 1,03 (INFLATION)
        Save.ProjectCosts[DI_ActPlayer].data[j] += (Save.ProjectCosts[DI_ActPlayer].data[j]>>12)*0x7B;
        Save.TechCosts[   DI_ActPlayer].data[j] += (   Save.TechCosts[DI_ActPlayer].data[j]>>12)*0x7B;
    }
}
