// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: recover rawfile\n");
        return 1;
    }

    char *infile = argv[1];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }
    printf("infile : %s\n",infile);
    //int outFileCnt = 0;
    char buffer[512];

    //while (!feof(inptr))
    //{
        fread(&buffer, 1, 512, inptr);
   /*     if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0 ) == 0xe0)
         {
             printf("new jpeg found");

         }
*/
        printf("bufer : %c\n", buffer[0]);
    //}

    fclose(inptr);
exit(0);
}

