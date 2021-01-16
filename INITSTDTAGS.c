#define PUBSCREENNAME "IT2c PubScreen 0";

#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void INITSTDTAGS()
{
    struct TagItem newTags[] = {{SA_DisplayID,   ScreenModeID_HighRes},
                                {SA_Interleaved, true},
                                {SA_Draggable,   false},
                                {SA_AutoScroll,  true},
                                {SA_Pens,        (ULONG) &Pens},
                                {SA_Colors,      (ULONG) &ColSpec},
#ifdef _DEBUG_
                                {SA_PubName,     (ULONG) PUBSCREENNAME},
#else
                                {TAG_DONE,0},
#endif
                                {TAG_DONE,0} };
    memset(&Pens, 0, sizeof(PenArr));
    ColSpec = (ColSpecArr) {{ 0,0,0,0,-1 }};

    memcpy( Tags, newTags, 7*sizeof(struct TagItem));
}
