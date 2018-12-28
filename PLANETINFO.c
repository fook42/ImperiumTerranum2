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
        if (PlanetHeader->PFlags>0) { j = PlanetHeader->PFlags; }
        pos = float2out( ((double)PlanetHeader->Size/10.0), 0, 2, s);
        strcpy(pos, PText[171]);
        WRITEWIN(89,33,j,0,RPort_PTR,4,s);

        stringlen=strlen(_PT_Klasse);
        memcpy(s, _PT_Klasse, stringlen);
        s[stringlen++]=' ';
        switch (PlanetHeader->Class) {
            case CLASS_DESERT    : s[stringlen]='D'; break;
            case CLASS_HALFEARTH : s[stringlen]='H'; break;
            case CLASS_EARTH     : s[stringlen]='M'; break;
            case CLASS_SATURN    : s[stringlen]='S'; break;
            case CLASS_GAS       : s[stringlen]='G'; break;
            case CLASS_ICE       : s[stringlen]='I'; break;
            case CLASS_PHANTOM   : s[stringlen]='P'; break;
            case CLASS_STONES    : s[stringlen]='T'; break;
            case CLASS_WATER     : s[stringlen]='W'; break;
            default: s[stringlen]='?';
        }
        stringlen++;
        s[stringlen++]='-';
        strcpy(s+stringlen, _PT_Planet);
        WRITEWIN(89,53,j,0,RPort_PTR,4,s);
        i = it_round((double) (abs(PlanetHeader->PosX)+abs(PlanetHeader->PosY))/3.4f);
        l = ((sint32) (13-i)*(13-i)*(13-i) / 3) -270;
        WRITEWIN(89,13,j,0,RPort_PTR,4,PlanetHeader->PName);
        if ((PlanetHeader->Class==CLASS_SATURN) || (PlanetHeader->Class==CLASS_GAS))
        {
            strcpy(s, PText[332]);
        } else {
            strcpy(s, PText[333]);
        }
        stringlen=strlen(s);
        s[stringlen++]=' ';
        switch (PlanetHeader->Class) {
            case CLASS_DESERT:    {
                                    pos = dez2out((PlanetHeader->Water / PlanetHeader->Size), 0, s+stringlen);
                                    *pos++='%'; *pos=0;
                                    SA6 = (SArr6) {{PText[334],PText[335],PText[336],PText[337],PText[338],s}};
                                    l = 5;
                                    } break;
            case CLASS_HALFEARTH: {
                                    pos = dez2out((PlanetHeader->Water / PlanetHeader->Size), 0, s+stringlen);
                                    *pos++='%'; *pos=0;
                                    SA6 = (SArr6) {{PText[340],PText[341],PText[342],PText[343],PText[344],s}};
                                    l = 4;
                                    } break;
            case CLASS_EARTH:     {
                                    pos = dez2out((PlanetHeader->Water / PlanetHeader->Size), 0, s+stringlen);
                                    *pos++='%'; *pos=0;
                                    SA6 = (SArr6) {{PText[346],PText[347],PText[348],PText[349],PText[350],s}};
                                    l = 6;
                                    } break;
            case CLASS_SATURN:    {
                                    pos = dez2out(l, 0, s+stringlen);
                                    *pos++=' '; *pos++='.'; *pos++='.'; *pos++=' ';
                                    pos = dez2out((l+(12-i)*10), 0, pos);
                                    *pos++=' '; *pos++='°'; *pos++='C'; *pos=0;
                                    SA6 = (SArr6) {{PText[352],PText[353],PText[354],PText[355],PText[356],s}};
                                    l = 3;
                                    } break;
            case CLASS_GAS:       {
                                    pos = dez2out(l, 0, s+stringlen);
                                    *pos++=' '; *pos++='.'; *pos++='.'; *pos++=' ';
                                    pos = dez2out((l+(12-i)*10), 0, pos);
                                    *pos++=' '; *pos++='°'; *pos++='C'; *pos=0;
                                    SA6 = (SArr6) {{PText[358],PText[359],PText[360],PText[361],PText[362],s}};
                                    l = 1;
                                    } break;
            case CLASS_ICE:       {
                                    pos = dez2out((PlanetHeader->Water / PlanetHeader->Size), 0, s+stringlen);
                                    *pos++='%'; *pos=0;
                                    SA6 = (SArr6) {{PText[364],PText[365],PText[366],PText[367],PText[368],s}};
                                    l = 8;
                                    } break;
            case CLASS_PHANTOM:   {
                                    SA6 = (SArr6) {{PText[370],PText[371],NULL,NULL,NULL,NULL}};
                                    l = 7;
                                    } break;
            case CLASS_STONES:    {
                                    pos = dez2out((PlanetHeader->Water / PlanetHeader->Size), 0, s+stringlen);
                                    *pos++='%'; *pos=0;
                                    SA6 = (SArr6) {{PText[373],PText[374],PText[375],PText[376],PText[377],s}};
                                    l = 0;
                                    } break;
            case CLASS_WATER:     {
                                    pos = dez2out((PlanetHeader->Water / PlanetHeader->Size), 0, s+stringlen);
                                    *pos++='%'; *pos=0;
                                    SA6 = (SArr6) {{PText[379],PText[380],PText[381],PText[382],PText[383],s}};
                                    l = 2;
                                    } break;
            default: { }
        }
        for(i = 0; i < 6; i++)
        {
            WRITEWIN(9,i*17+87,12,0,RPort_PTR,3,SA6.data[i]);
        }

        x = 0;
        y = 0;
        do
        {
            delay(RDELAY);
            if ((x<32) && (y<32))
            {
                BltBitMapRastPort((struct BitMap*) &ImgBitMap7,l*32+x,y,RPort_PTR,x*2+12,y*2+12,1,1,192);
                BltBitMapRastPort((struct BitMap*) &ImgBitMap7,l*32+x,y,RPort_PTR,x*2+13,y*2+12,1,1,192);
                BltBitMapRastPort((struct BitMap*) &ImgBitMap7,l*32+x,y,RPort_PTR,x*2+12,y*2+13,1,1,192);
                BltBitMapRastPort((struct BitMap*) &ImgBitMap7,l*32+x,y,RPort_PTR,x*2+13,y*2+13,1,1,192);
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
