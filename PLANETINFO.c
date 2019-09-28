#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void PLANETINFO(uint8 ActSys)
{
typedef struct SArr6 {
    char*    data[6];
} SArr6;

    SArr6   SA6;
    char    s[60];
    char*   pos;
    sint32  l;
    uint8   i,j;
    uint8   x,y;
    uint8   stringlen;
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
        j = 12;
        if (0 < PlanetHeader->PFlags)
        {
            j = PlanetHeader->PFlags;
        }
        pos = float2out( ((double)PlanetHeader->Size/10.0), 0, 2, s);
        strcpy(pos, PText[171]);
        WRITE(89,33,j,0,RPort_PTR,3,s);

        stringlen=strlen(_PT_Klasse);
        memcpy(s, _PT_Klasse, stringlen);
        pos=s+stringlen;
        *pos++=' ';
        switch (PlanetHeader->Class) {
            case CLASS_DESERT    : *pos++='D'; break;
            case CLASS_HALFEARTH : *pos++='H'; break;
            case CLASS_EARTH     : *pos++='M'; break;
            case CLASS_SATURN    : *pos++='S'; break;
            case CLASS_GAS       : *pos++='G'; break;
            case CLASS_ICE       : *pos++='I'; break;
            case CLASS_PHANTOM   : *pos++='P'; break;
            case CLASS_STONES    : *pos++='T'; break;
            case CLASS_WATER     : *pos++='W'; break;
            default: *pos++='?';
        }
        *pos++='-';
        strcpy(pos, _PT_Planet);
        WRITE(89,53,j,0,RPort_PTR,3,s);
        i = it_round((double) (abs(PlanetHeader->PosX)+abs(PlanetHeader->PosY))/3.4f);
        l = ((sint32) (13-i)*(13-i)*(13-i) / 3) -270;
        WRITE(89,13,j,0,RPort_PTR,3,PlanetHeader->PName);
        if ((PlanetHeader->Class==CLASS_SATURN) || (PlanetHeader->Class==CLASS_GAS))
        {
            strcpy(s, PText[332]);
        } else {
            strcpy(s, PText[333]);
        }
        pos=s+strlen(s);
        *pos++=' ';
        switch (PlanetHeader->Class) {
            case CLASS_DESERT:    {
                                    pos = dez2out((PlanetHeader->Water / PlanetHeader->Size), 0, pos);
                                    *pos++='%'; *pos=0;
                                    SA6 = (SArr6) {{PText[334],PText[335],PText[336],PText[337],PText[338],s}};
                                    l = 5;
                                    } break;
            case CLASS_HALFEARTH: {
                                    pos = dez2out((PlanetHeader->Water / PlanetHeader->Size), 0, pos);
                                    *pos++='%'; *pos=0;
                                    SA6 = (SArr6) {{PText[340],PText[341],PText[342],PText[343],PText[344],s}};
                                    l = 4;
                                    } break;
            case CLASS_EARTH:     {
                                    pos = dez2out((PlanetHeader->Water / PlanetHeader->Size), 0, pos);
                                    *pos++='%'; *pos=0;
                                    SA6 = (SArr6) {{PText[346],PText[347],PText[348],PText[349],PText[350],s}};
                                    l = 6;
                                    } break;
            case CLASS_SATURN:    {
                                    pos = dez2out(l, 0, pos);
                                    *pos++=' '; *pos++='.'; *pos++='.'; *pos++=' ';
                                    pos = dez2out((l+(12-i)*10), 0, pos);
                                    *pos++=' '; *pos++='�'; *pos++='C'; *pos=0;
                                    SA6 = (SArr6) {{PText[352],PText[353],PText[354],PText[355],PText[356],s}};
                                    l = 3;
                                    } break;
            case CLASS_GAS:       {
                                    pos = dez2out(l, 0, pos);
                                    *pos++=' '; *pos++='.'; *pos++='.'; *pos++=' ';
                                    pos = dez2out((l+(12-i)*10), 0, pos);
                                    *pos++=' '; *pos++='�'; *pos++='C'; *pos=0;
                                    SA6 = (SArr6) {{PText[358],PText[359],PText[360],PText[361],PText[362],s}};
                                    l = 1;
                                    } break;
            case CLASS_ICE:       {
                                    pos = dez2out((PlanetHeader->Water / PlanetHeader->Size), 0, pos);
                                    *pos++='%'; *pos=0;
                                    SA6 = (SArr6) {{PText[364],PText[365],PText[366],PText[367],PText[368],s}};
                                    l = 8;
                                    } break;
            case CLASS_PHANTOM:   {
                                    SA6 = (SArr6) {{PText[370],PText[371],NULL,NULL,NULL,NULL}};
                                    l = 7;
                                    } break;
            case CLASS_STONES:    {
                                    pos = dez2out((PlanetHeader->Water / PlanetHeader->Size), 0, pos);
                                    *pos++='%'; *pos=0;
                                    SA6 = (SArr6) {{PText[373],PText[374],PText[375],PText[376],PText[377],s}};
                                    l = 0;
                                    } break;
            case CLASS_WATER:     {
                                    pos = dez2out((PlanetHeader->Water / PlanetHeader->Size), 0, pos);
                                    *pos++='%'; *pos=0;
                                    SA6 = (SArr6) {{PText[379],PText[380],PText[381],PText[382],PText[383],s}};
                                    l = 2;
                                    } break;
            default: { }
        }
        j = 0;
        for(i = 0; i < 6; ++i)
        {
            WRITE(9,j+87,12,0,RPort_PTR,2,SA6.data[i]);
            j += 17;
        }

        x = 0;
        y = 0;
        l<<=5; // l*32
        do
        {
            Delay(RDELAY);
            if ((x<32) && (y<32))
            {
                BltBitMapRastPort((struct BitMap*) &ImgBitMap7,l+x,y,RPort_PTR,(x<<1)+12,(y<<1)+12,1,1,192);
                BltBitMapRastPort((struct BitMap*) &ImgBitMap7,l+x,y,RPort_PTR,(x<<1)+13,(y<<1)+12,1,1,192);
                BltBitMapRastPort((struct BitMap*) &ImgBitMap7,l+x,y,RPort_PTR,(x<<1)+12,(y<<1)+13,1,1,192);
                BltBitMapRastPort((struct BitMap*) &ImgBitMap7,l+x,y,RPort_PTR,(x<<1)+13,(y<<1)+13,1,1,192);
                x++;
                if (x>31)
                {
                    x = 0;
                    y++;
                }
            }
        }
        while (LMB_NOTPRESSED && RMB_NOTPRESSED);
        PLAYSOUND(1,300);
        CloseWindow(PLI_Window);
    } else {
        HANDLEKNOWNPLANET(ActSys, 0, PlanetHeader);
    }
}
