#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

bool INITLANG()
{
    bool    WordSet;
    uint8*  c;
    uint16  i;
    uint32  Addr1;
    char    s[] = {"Language.txt\0"};
    BPTR    FHandle;

    FHandle = Open((CONST_STRPTR) s, MODE_OLDFILE);
    if (0 == FHandle)
    {
        puts("Kann die Sprach-Datei ");
        puts(s);
        puts(" nicht finden !\n");
        puts("Can`t find language file ");
        puts(s);
        puts(" !\n");
        return false;
    }
    (void)     Seek(FHandle, 0, OFFSET_END);
    TextMemL = Seek(FHandle, 0, OFFSET_BEGINNING);
    TextMemA = AllocMem(TextMemL, MEMF_CLEAR);
    if (NULL == TextMemA)
    {
        puts("Nicht genug Speicher vorhanden!\n");
        puts("Not enough Memory available!\n");
        Close(FHandle);
        return false;
    }
    (void) Read(FHandle, TextMemA, TextMemL);
    Close(FHandle);
    Addr1 = (uint32) TextMemA;
    i = 1;
    WordSet = false;
    do
    {
        c = (uint8*) Addr1;
        Addr1 = Addr1+1;
        if ((((*c>=33) && (*c<=58)) || ((*c>=60) && (*c<=255))) && (!WordSet))
        {
            PText[i] = (char*) (Addr1-1);
            i++;
            WordSet = true;
            
        } else if (*c == 59)
        {
            WordSet = true;
        } else if ((*c == 10) && (WordSet))
        {
            *c = 0;
            WordSet = false;
        }
    }
    while ((Addr1 <= (((uint32) TextMemA)+TextMemL)) && (i<=MAXSTR));
    return true;
}
