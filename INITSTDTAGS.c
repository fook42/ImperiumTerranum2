char PubScreenName[] = "IT2c PubScreen 0";

#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void INITSTDTAGS()
{
    struct TagItem newTags[] = {{SA_DisplayID,   0x9004+HelpID},
                                {SA_Interleaved, true},
                                {SA_Draggable,   false},
                                {SA_Pens,        (uint32) &Pens},
                                {SA_Colors,      (uint32) &ColSpec},
#ifdef _DEBUG_
                                {SA_PubName,     (uint32) PubScreenName},
#else
                                {TAG_DONE,0},
#endif
                                {TAG_DONE,0} };
    memset(&Pens, 0, sizeof(PenArr));
    ColSpec = (ColSpecArr) {{ 0,0,0,0,-1 }};

    memcpy( Tags, newTags, 7*sizeof(struct TagItem));
}
