#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void CLEARINTUITION()
{
    struct IntuiMessage*  IMsg;
    uint8   i, RawCode;

    if ((NULL != MyScreen[0]->FirstWindow)
     && (NULL != MyScreen[1]->FirstWindow))
    {
        for (i = 0; i < 2; i++)
        {
            do
            {
                RawCode = 0;
                IMsg = (struct IntuiMessage*) GetMsg(MyWindow[i]->UserPort);
                if (NULL != IMsg)
                {
                    RawCode = 1;
                    ReplyMsg((struct Message *)IMsg);
                }
            }
            while (0 != RawCode);
        }
    }
}
