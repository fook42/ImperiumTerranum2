#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

bool INITDESK(uint8 Mode)
{
    char    s[50];
    char*   _s;
    BPTR    FHandle;

    _s=my_strcpy(s, PathStr[0]);
    _s=my_strcpy(_s, "Desk.pal");
    (void) SETCOLOR( MyScreen[0], s);

    (void) my_strcpy(_s-3, "img");
    if (!DISPLAYIMAGE( s, 512, 0, 128, 512, 7, MyScreen[0], 0)) {  return false; }
    SETWORLDCOLORS();
    WRITE_RP0(576,156,45,WRITE_Center, 3, _PT_Jahr);
    WRITE_RP0(576,207,45,WRITE_Center, 3, PText[146]);
    WRITE_RP0(576,258,45,WRITE_Center, 3, PText[147]);

    _s=my_strcpy(s, PathStr[5]);
    (void) my_strcpy(_s, "DeskImages.img");
    if (!RAWLOADIMAGE(s,0,32,608,32,4,&ImgBitMap4)) { return false; }

    if (1 == Mode)
    {
        _s=my_strcpy(s, PathStr[0]);
        (void) my_strcpy(_s, "DeskImages.img");
        if (!RAWLOADIMAGE(s,0,0,416, 32,7,&ImgBitMap7)) { return false; }

        (void) my_strcpy(_s, "ProjectIcons.img");
        if (!RAWLOADIMAGE(s,0,0,640,192,8,&ImgBitMap8)) { return false; }

        IMemL[1] = 4480;
        IMemA[1] = AllocMem( IMemL[1], MEMF_CHIP | MEMF_CLEAR );
        if (NULL == IMemA[1]) { return false; }

        (void) my_strcpy(_s, "DeskGads.img");
        FHandle = OPENSMOOTH( s, MODE_OLDFILE);       /*Planets/Deskgads.img*/
        if (0 == FHandle)
        {
            FreeMem( IMemA[1], IMemL[1] );
            IMemA[1] = NULL;
            return false;
        }
        (void) Read( FHandle, IMemA[1], IMemL[1]);
        Close( FHandle );
        GadImg1.ImageData = (UWORD*)  IMemA[1];
        GadImg2.ImageData = (UWORD*) (IMemA[1] + 2240);

        _s=my_strcpy(s, PathStr[4]);
        (void) my_strcpy(_s, "MOD.Invention");
        LOADMOD( s, 0 );
        (void) my_strcpy(_s, "MOD.War");
        LOADMOD( s, 1 );
        (void) my_strcpy(_s, "MOD.Tech");
        LOADMOD( s, 2 );
        (void) my_strcpy(_s, "MOD.Bad");
        LOADMOD( s, 3 );

        _s=my_strcpy(s, PathStr[7]);
        (void) my_strcpy(_s, "Worm.img");
        if (!RAWLOADIMAGE(s,0,32,512,32,7,&ImgBitMap7)) { return false; }

        (void) my_strcpy(_s, "XPlode.img");
        if (!RAWLOADIMAGE(s,0, 0,512,32,4,&ImgBitMap4)) { return false; }
    }
    return true;
}
