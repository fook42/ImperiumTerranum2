#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"

int it_round(double x)
{
    return (int) (x+0.5f);
}

char* dez2out(sint32 value, uint8 digits, char* dest)
{
    const sint32 _dez[]={1,10,100,1000,10000,100000,1000000,10000000};
    uint8        c;
    bool         show = true;

    if (0 == digits)
    {
        digits = 8;
        show = false;
    }
    if (0 > value)
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
    sint32 temp = (sint32) fvalue;

    _ftemp = dez2out(temp, dezdigits, dest);
    *_ftemp++='.';
    return dez2out(it_round((fvalue-temp)*pow(10.0f,flodigits)), flodigits, _ftemp);
}

char* hex2out(sint32 dez, uint8 digits, char* dest)
{
    uint8 c;
    bool show = true;
    if (0 == digits)
    {
        digits = 8;
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
    PLANEPTR PlaneMemA;
    uint32   BPRows= BytesPerRow*Rows;
    uint8    i;

    ITBMap->MemL = BPRows*Depth;
    ITBMap->MemA = AllocMem(ITBMap->MemL, MEMF_CHIP | MEMF_CLEAR);
    if (NULL != ITBMap->MemA)
    {
        ITBMap->BytesPerRow = BytesPerRow;
        ITBMap->Rows        = Rows;
        ITBMap->Flags       = 1;
        ITBMap->Depth       = Depth;
        ITBMap->pad         = 0;
        PlaneMemA = (PLANEPTR) ITBMap->MemA;
        for (i=0; i<Depth; ++i)
        {
            ITBMap->PPtr[i] = PlaneMemA;
            PlaneMemA += BPRows;
        }
        for (;i<8; ++i)
        {
            ITBMap->PPtr[i] = NULL;
        }
        return true;
    }
    return false;
}

ULONG string2hex(char* in_string)
{
    ULONG out_dez=0;
    char act_c=*in_string++;
    if ((act_c == '0') && (in_string[0] == 'x'))
    {
        in_string++;
        act_c=*in_string++;
    } else if (act_c == '$')
    {
        act_c=*in_string++;
    }
    int num=0;
    while((num<8) && (0 != act_c))
    {
        act_c -= '0';  // -> 0..9 ..
        act_c &= 0x1F; // a -> A
        if (act_c > 0x09 )
        {
            act_c -= 7;
        }
        out_dez = (out_dez<<4)+act_c;
        act_c=*in_string++;
        ++num;
    }
    return out_dez;
}

char* my_strcpy(char* destination, const char* source)
{
    // return if no memory is allocated to the destination
    if (destination == NULL)
        return NULL;
 
    // copy the C-string pointed by source into the array
    // pointed by destination
    while ((*destination++ = *source++) != '\0');
 
    // destination is returned by standard strcpy()
    return (destination-1);
}
