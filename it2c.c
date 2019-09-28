const char __ver[] = "$VER: ImperiumTerranum 2.8c (10.04.2020)\0";

#include <stdio.h>
#include <stdlib.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <proto/diskfont.h>
#include <proto/medplayer.h>
#include <proto/asl.h>  // for screenmode-requester
#include <dos/dostags.h>
#include <workbench/startup.h>

#define AUTOCON "CON:20/20/500/120/mycon/auto/close/wait"
#define _EXTERN_

#define SCREENREQTITLE_High "Please select HIGHRES (min. 640x512) screenmode"
#define SCREENREQTITLE_Low  "Please select LOWRES (min. 320x256) screenmode"

#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

ULONG getScreenmode(char* RequestTitle)
{
    struct ScreenModeRequester* gSm_SMRequester;
    struct TagItem RequesterTags[]={{ASLSM_DoWidth, true},
                                    {ASLSM_DoHeight, true},
                                    {ASLSM_DoDepth, true},
                                    {ASLSM_TitleText, (ULONG) RequestTitle},
                                    {TAG_DONE,0},
                                    {TAG_END,0} };
    bool    rc;
    ULONG   _returnvalue = 0;

    gSm_SMRequester = AllocAslRequest(ASL_ScreenModeRequest, RequesterTags);

    if (NULL != gSm_SMRequester)
    {
        rc = AslRequest( (APTR) gSm_SMRequester, NULL );
        if (rc)
        {
/*            puts("screenmode selected");
            Printf("ID: %ld\n",     gSm_SMRequester->sm_DisplayID );
            Printf("Width: %ld\n",  gSm_SMRequester->sm_DisplayWidth );
            Printf("Height: %ld\n", gSm_SMRequester->sm_DisplayHeight );
            Printf("Depth: %ld\n",  gSm_SMRequester->sm_DisplayDepth );
*/
            _returnvalue = gSm_SMRequester->sm_DisplayID;
        }
/*        else
        {
            puts("no screenmode selected");
        }
*/
        FreeAslRequest( gSm_SMRequester );
    }
    return _returnvalue;
}

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
        MEDPlayerBase = OpenLibrary((CONST_STRPTR) "medplayer.library", (ULONG) 0);
        if(!MEDPlayerBase)
        {
            puts("Can't open medplayer.library!\n");
        }

        ScreenModeID_HighRes = getScreenmode(SCREENREQTITLE_High);
        if (0 == ScreenModeID_HighRes)
        {
            ScreenModeID_HighRes = 0xA9004;
        }

        ScreenModeID_LowRes = getScreenmode(SCREENREQTITLE_Low);
        if (0 == ScreenModeID_LowRes)
        {
            ScreenModeID_LowRes = 0xA1000;
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
