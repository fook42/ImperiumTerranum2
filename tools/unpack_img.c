/*************************************
 * Unpack_img.c
 *
 * tool to "decompress" .img files from ImperiumTerranum2
 * creates raw-image-bitplanes, no structured image format!
 *
 * just a short hack.. not much error handling included!
 *
 * F00K42 - 2025/01/25
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char   uint8;
typedef unsigned short  uint16;
typedef unsigned long   uint32;

void UNPACK(uint8* Anfang, uint8* PackedD, const uint32 Laenge)
{
    uint8  Anz;
    uint8* UnPackedD;

    UnPackedD = Anfang;
    do
    {
        Anz = *PackedD++;
        if (Anz&128)
        {
            Anz -= 127;
            memcpy(UnPackedD, PackedD, Anz);
            PackedD += Anz;
        } else {
            memset((void*)UnPackedD, *PackedD, Anz);
            PackedD++;
        }
        UnPackedD += Anz;
    }
    while ((UnPackedD<PackedD) && (UnPackedD<(Anfang+Laenge)));
}

int main(int argc, char **argv)
{
    FILE* in_file;
    FILE* out_file;
    uint8* file_memory_ptr;
    int Width, Height, Depth;

    if (6 > argc)
    {
        printf("ERROR: missing arguments!\n\n");
        printf("Usage %s <input_filename.img> <output_filename> <Width> <Height> <Depth>\n\n", argv[0]);
        return -1;
    }
    Width  = strtol(argv[3],(char **)NULL,10);
    Height = strtol(argv[4],(char **)NULL,10);
    Depth  = strtol(argv[5],(char **)NULL,10);

    in_file=fopen(argv[1],"rb");
    if (NULL != in_file)
    {
        fseek(in_file, 0, SEEK_END);
        int file_size = ftell(in_file);
        fseek(in_file, 0, SEEK_SET);

        file_memory_ptr=malloc(file_size*2);

        int file_read = fread(&file_memory_ptr[file_size], 1, file_size, in_file);

        fclose(in_file);
        if (file_read<file_size)
        {
            printf("ERROR: could not read complete file %s!\n", argv[1]);
            
        } else {
            printf(">> compressed file %s read\n",argv[1]);

            int ImgSize = (Width*Height*Depth) / 8;
            UNPACK(file_memory_ptr, &file_memory_ptr[file_size], ImgSize);

            out_file=fopen(argv[2], "wb+");

            if (NULL != out_file)
            {
                fwrite(file_memory_ptr,1,ImgSize,out_file);
                fclose(out_file);
                printf(">> decompressed file %s written\n", argv[2]);
            }
        }
        free(file_memory_ptr);
    }

    return 0;
}
