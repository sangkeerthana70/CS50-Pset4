// recovers all Jpeg files from memory card

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: recover image\n");
        return 1;
    }

    char *infile = argv[1];
    int outFileCnt = 0;
    char outfile[8];//name of jpeg file
    typedef uint8_t  BYTE;
    BYTE buffer[512];//data type BYTE array
    FILE *outptr = NULL;
    short imgFound = 0;//short is a small integer set to false

    // open memory card
    FILE *inptr = fopen(infile, "r");

    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    //while true to read all blocks in raw file
    while (fread(&buffer, 512, 1, inptr) == 1)
    {
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            if (imgFound)
            {
                fclose(outptr);
            }
            imgFound = 1;
            sprintf(outfile, "%03i.jpg", outFileCnt);//create a Jpeg file.
            outptr = fopen(outfile, "w");//open the jpg file
            outFileCnt++;//increment count
        }
        if (imgFound)
        {
            fwrite(&buffer, 512, 1, outptr);//write to the byte array.
        }
    }

    fclose(inptr);
}

