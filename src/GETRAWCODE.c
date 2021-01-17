#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

uint8 GETRAWCODE()
{
    struct IntuiMessage*  IMsg = NULL;
    uint8 _GETRAWCODE = 0;

    if ((NULL != MyScreen[0]->FirstWindow) &&
        (NULL != MyScreen[1]->FirstWindow))
    {
        IMsg = (struct IntuiMessage*) GetMsg(MyWindow[0]->UserPort);
        if (NULL == IMsg)
        {
            IMsg = (struct IntuiMessage*) GetMsg(MyWindow[1]->UserPort);
        }

        if (NULL != IMsg) {
            if (RAWKEY == IMsg->Class)
            {
                _GETRAWCODE = IMsg->Code;
            }
            ReplyMsg((struct Message *)IMsg);
        }
    }
    return _GETRAWCODE;
}
