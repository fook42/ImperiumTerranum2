#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

bool INITLANG()
{
    bool    WordSet;
    uint8   c;
    uint16  i;
    uint8*  TextMemEnd;
    uint8*  Char_PTR;
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
    TextMemEnd = (uint8*) (TextMemA + TextMemL);
    Char_PTR = (uint8*) TextMemA;
    i = 1;
    WordSet = false;
    do
    {
        c = *Char_PTR;
        if ( (32 < c) && (59 != c) && (!WordSet) )
        {
            PText[i] = Char_PTR;
            ++i;
            WordSet = true;
        } else if (59 == c)
        {
            WordSet = true;
        } else if ((10 == c) && (WordSet))
        {
            *Char_PTR = 0;
            WordSet = false;
        }
        ++Char_PTR;
    }
    while ((TextMemEnd >= Char_PTR) && (MAXSTR >= i));
    return true;
}
