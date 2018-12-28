#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "IT2_Defines.h"
#include "IT2_Types.h"

void delay(uint16 time)
{
    (void) usleep(time*10000);
}

int it_round(double x)
{
    return (int) (x+0.5f);
}

char* dez2out(long int value, uint8 digits, char* dest)
{
    const long int _dez[]={1,10,100,1000,10000,100000,1000000,10000000};
    unsigned char c;
    bool show = true;

    if (0 == digits)
    {
        digits=8;
        show = false;
    }
    if (value<0)
    {
        *dest++='-';
        value=-value;
    }
    while (digits--)
    {
        c = value/_dez[digits];
        if ((true == show) || (0 != c) || (0 == digits))
        {
            show = true;
            *dest++=c+'0';
        }
        value=value%_dez[digits];
    }
    *dest=0;
    return dest;
}

char* float2out(double fvalue, uint8 dezdigits, uint8 flodigits, char* dest)
{
    char* _ftemp;
    long int temp = (long int) fvalue;
    _ftemp = dez2out(temp, dezdigits, dest);
    *_ftemp++='.';
    return dez2out(it_round((fvalue-temp)*pow(10.0f,flodigits)), flodigits, _ftemp);    
}

char* hex2out(long int dez, uint8 digits, char* dest)
{
    unsigned char c;
    bool show = true;
    if (0 == digits)
    {
        digits=8;
        show = false;
    }
    while (digits--)
    {
        c=(dez>>(digits<<2))&0x0F;
        if ((true == show) || (0 != c) || (0 == digits))
        {
            show = true;
            if (c>9) { c+=7; }
            *dest++=c+'0';
        }
    }
    *dest=0;
    return dest;
}

bool FillITBitMap(struct ITBitMap* ITBMap, uint16 BytesPerRow, uint16 Rows, uint8 Depth)
{
    int i;
    ITBMap->MemL = BytesPerRow*Rows*Depth;
    ITBMap->MemA = AllocMem(ITBMap->MemL, MEMF_CHIP+MEMF_CLEAR);
    if (NULL != ITBMap->MemA)
    {
        ITBMap->BytesPerRow  = BytesPerRow;
        ITBMap->Rows         = Rows;
        ITBMap->Flags        = 1;
        ITBMap->Depth        = Depth;
        ITBMap->pad = 0;

        for (i=0; i<Depth; i++)
        {
            ITBMap->PPtr[i] = (PLANEPTR) (ITBMap->MemA + (i*BytesPerRow*Rows));
        }
        for (;i<8; i++)
        {
            ITBMap->PPtr[i] = NULL;
        }
        return true;
    }
    return false;
}
