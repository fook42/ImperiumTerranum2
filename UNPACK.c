#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void UNPACK(uint8* Anfang, uint8* PackedD, uint32 Laenge, uint8 Mode)
{
    uint8  Anz;
    uint8* UnPackedD;

    if (0 == Mode) { SWITCHDISPLAY(); }
    UnPackedD = Anfang;
    do
    {
        Anz = *PackedD++;
        if (Anz&128)
        {
            Anz -= 127;
            CopyMem((APTR) PackedD, (APTR) UnPackedD, (ULONG) Anz);
            PackedD += Anz;
        } else {
            memset((void*)UnPackedD, *PackedD, Anz);
            PackedD++;
        }
        UnPackedD += Anz;
    }
    while ((UnPackedD<PackedD) && (UnPackedD<(Anfang+Laenge)));
}
