#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void PLANETINFO(const int ActSys)
{
    const char* ClassDescript[][5] = {
                                    {PText[373],PText[374],PText[375],PText[376],PText[377]},
                                    {PText[358],PText[359],PText[360],PText[361],PText[362]},
                                    {PText[379],PText[380],PText[381],PText[382],PText[383]},
                                    {PText[352],PText[353],PText[354],PText[355],PText[356]},
                                    {PText[340],PText[341],PText[342],PText[343],PText[344]},
                                    {PText[334],PText[335],PText[336],PText[337],PText[338]},
                                    {PText[346],PText[347],PText[348],PText[349],PText[350]},
                                    {PText[370],PText[371],      NULL,      NULL,      NULL},
                                    {PText[364],PText[365],PText[366],PText[367],PText[368]}
                                };

    const char ClassName[] = {'T','G','W','S','H','D','M','P','I'};
    //                         St, G , W , Sa, HE, De, E,  P , I

    char    s[60];
    char*   _s;
    sint32  l;
    int     i, color;
    int     x, y;
    int     PClass;
    r_PlanetHeader* PlanetHeader;
    struct Window* PLI_Window;
    struct RastPort* RPort_PTR;

    PlanetHeader = ObjPtr;
    if ((PlanetHeader->PFlags & FLAG_CIV_MASK) != ActPlayerFlag)
    {
        PLI_Window=MAKEWINDOW(81,81,348,198,MyScreen[0]);
        if (NULL == PLI_Window)
        {
            return;
        }
        RPort_PTR = PLI_Window->RPort;
        MAKEWINBORDER(RPort_PTR,0,0,347,197,12,6,1);
        MAKEWINBORDER(RPort_PTR,9,9, 80, 80,12,6,1);
        color = 12;
        if (0 < PlanetHeader->PFlags)
        {
            color = PlanetHeader->PFlags;
        }
        WRITE(89,13,color,0,RPort_PTR,3,PlanetHeader->PName);

        _s = float2out( ((double)PlanetHeader->Size/10.0), 0, 2, s);
        (void) my_strcpy(_s, PText[171]);
        WRITE(89,33,color,0,RPort_PTR,3,s);

        PClass = PlanetHeader->Class;
        _s=my_strcpy(s, _PT_Klasse);
        *_s++ = ' ';
        *_s++ = ClassName[PClass];
        *_s++ = '-';
        (void) my_strcpy(_s, _PT_Planet);
        WRITE(89,53,color,0,RPort_PTR,3,s);

        if (1 == ClassLifeFactor[PClass])
        {
            _s=my_strcpy(s, PText[333]);
            *_s++ = ' ';
            _s = dez2out((PlanetHeader->Water / PlanetHeader->Size), 0, _s);
            *_s++ = '%';
        } else if (CLASS_PHANTOM != PClass)
        {
            i = it_round((double) (abs(PlanetHeader->PosX)+abs(PlanetHeader->PosY))/3.4f);
            l = ((sint32) (13-i)*(13-i)*(13-i) / 3) -270;

            _s=my_strcpy(s, PText[332]);
            *_s++ = ' ';
            _s = dez2out(l, 0, _s);
            *_s++ = ' '; *_s++ = '.'; *_s++ = '.'; *_s++ = ' ';
            _s = dez2out((l+(12-i)*10), 0, _s);
            *_s++ = ' '; *_s++ = 0xB0; *_s++ = 'C';
        } else {
            _s = s;
        }
        *_s = 0;

        y = 87;
        for(i = 0; i < 5; ++i)
        {
            WRITE(9,y,12,0,RPort_PTR,2,ClassDescript[PClass][i]);
            y += 17;
        }
        WRITE(9,y,12,0,RPort_PTR,2, s);

        x = 0;
        y = 0;
        l = 32 * PClass;
        do
        {
            Delay(RDELAY);
            if (y<32)
            {
                BltBitMapRastPort((struct BitMap*) &ImgBitMap7,l+x,y,RPort_PTR,(x*2)+12,(y*2)+12,1,1,192);
                BltBitMapRastPort((struct BitMap*) &ImgBitMap7,l+x,y,RPort_PTR,(x*2)+13,(y*2)+12,1,1,192);
                BltBitMapRastPort((struct BitMap*) &ImgBitMap7,l+x,y,RPort_PTR,(x*2)+12,(y*2)+13,1,1,192);
                BltBitMapRastPort((struct BitMap*) &ImgBitMap7,l+x,y,RPort_PTR,(x*2)+13,(y*2)+13,1,1,192);
                x++;
                if (x>31)
                {
                    x = 0;
                    y++;
                }
            }
        }
        while (LMB_NOTPRESSED && RMB_NOTPRESSED);
        PLAYSOUND(0,300);
        CloseWindow(PLI_Window);
    } else {
        HANDLEKNOWNPLANET(ActSys, 0, PlanetHeader);
    }
}
