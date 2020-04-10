const char __ver[] = "$VER: ImperiumTerranum 2.8c (10.04.2020)\0";

#include <stdio.h>
#include <stdlib.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <proto/diskfont.h>
#include <proto/medplayer.h>
#include <dos/dostags.h>
#include <workbench/startup.h>

#define AUTOCON "CON:20/20/500/120/mycon/auto/close/wait"
#define _EXTERN_

#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

int main(void)
{
    struct Process   *proc;
    struct WBStartup *WBMsg = NULL;
    bool             prCLI;
    BPTR             CLIout;
    int              rc = 0;

    proc    = (struct Process *)FindTask(NULL);

    prCLI = proc->pr_CLI ? true : false;
    if (!prCLI)
    {
        WaitPort(&proc->pr_MsgPort);
        WBMsg = (struct WBStartup *)GetMsg(&proc->pr_MsgPort);
    }

    CLIout = prCLI ? Output() : Open(AUTOCON,MODE_OLDFILE);
    if (CLIout)
    {
        SelectOutput(CLIout);
        //--------------------------------------------------
        MEDPlayerBase = OpenLibrary((CONST_STRPTR) "medplayer.library", (unsigned long int) 0);
        if(!MEDPlayerBase)
        {
            puts("Can't open medplayer.library!\n");
        }

        MAIN_FNC();
        rc = 0;

        if(MEDPlayerBase)
        {
            StopPlayer();
            FreePlayer();
            CloseLibrary(MEDPlayerBase);
        }
        //--------------------------------------------------
        if (!prCLI)
        {
            Close(CLIout);
        }

    } else {
        rc = RETURN_FAIL;
    }
    if (WBMsg)
    {
        Forbid();
        ReplyMsg(&WBMsg->sm_Message);
    }

    return rc;
}
