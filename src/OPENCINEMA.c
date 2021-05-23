#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

struct Screen* OPENCINEMA(uint8 Depth)
{
    struct NewScreen OC_Screen = {0,0,640,435,Depth,0,0,HIRES+LACE,CUSTOMSCREEN|SCREENQUIET,NULL,NULL,NULL,NULL};

    INITSTDTAGS();
    return OpenScreenTagList(&OC_Screen, Tags);
}
