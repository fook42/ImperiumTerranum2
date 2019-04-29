#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

bool INITDESK(uint8 Mode)
{
    uint16  slen;
    char    s[50];
    BPTR    FHandle;

    slen = strlen(PathStr[0]);
    memcpy(s, PathStr[0], slen+1);
    strcpy(s+slen, "Desk.pal");
    (void) SETCOLOR( MyScreen[0], s);

    strcpy(s+slen, "Desk.img");
    if (!DISPLAYIMAGE( s, 512, 0, 128, 512, 7, MyScreen[0], 0)) {  return false; }
    SETWORLDCOLORS();
    WRITE(576,156,45,WRITE_Center, MyRPort_PTR[0], 4, _PT_Jahr);
    WRITE(576,207,45,WRITE_Center, MyRPort_PTR[0], 4, PText[146]);
    WRITE(576,258,45,WRITE_Center, MyRPort_PTR[0], 4, PText[147]);

    slen = strlen(PathStr[5]);
    memcpy(s, PathStr[5], slen+1);

    strcpy(s+slen, "DeskImages.img");
    if (!RAWLOADIMAGE(s,0,32,608,32,4,&ImgBitMap4)) { return false; }

    if (1 == Mode)
    {
        slen = strlen(PathStr[0]);
        memcpy(s, PathStr[0], slen+1);

        strcpy(s+slen, "DeskImages.img");
        if (!RAWLOADIMAGE(s,0,0,416, 32,7,&ImgBitMap7)) { return false; }

        strcpy(s+slen, "ProjectIcons.img");
        if (!RAWLOADIMAGE(s,0,0,640,192,8,&ImgBitMap8)) { return false; }

        IMemL[1] = 4480;
        IMemA[1] = AllocMem( IMemL[1], MEMF_CHIP+MEMF_CLEAR );
        if (NULL == IMemA[1]) { return false; }

        strcpy(s+slen, "DeskGads.img");
        FHandle = OPENSMOOTH( s, MODE_OLDFILE);       /*Planets/Deskgads.img*/
        if (0 == FHandle)
        {
            FreeMem( IMemA[1], IMemL[1] );
            IMemA[1] = NULL;
            return false;
        }
        (void) Read( FHandle, IMemA[1], IMemL[1]);
        Close( FHandle );
        GadImg1.ImageData = IMemA[1];
        GadImg2.ImageData = IMemA[1]+2240;

        slen = strlen(PathStr[4]);
        memcpy(s, PathStr[4], slen+1);

        strcpy(s+slen, "MOD.Invention");
        LOADMOD( s, 1 );
        strcpy(s+slen, "MOD.War");
        LOADMOD( s, 2 );
        strcpy(s+slen, "MOD.Tech");
        LOADMOD( s, 3 );
        strcpy(s+slen, "MOD.Bad");
        LOADMOD( s, 4 );

        slen = strlen(PathStr[7]);
        memcpy(s, PathStr[7], slen+1);

        strcpy(s+slen, "Worm.img");
        if (!RAWLOADIMAGE(s,0,32,512,32,7,&ImgBitMap7)) { return false; }

        strcpy(s+slen, "XPlode.img");
        if (!RAWLOADIMAGE(s,0, 0,512,32,4,&ImgBitMap4)) { return false; }
    }
    return true;
}
