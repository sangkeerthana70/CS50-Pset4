// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"
void writeLine(int origWidth, int origPadding,int size,int padding,FILE *inptr, FILE *outptr);

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: resize factor infile outfile\n");
        return 1;
    }

    // remember filenames
    int size = atoi(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // Change bi.biWidth,bi.biHeight,bi.biSizeImage and bf.bfSize to accomodate resize
    int origWidth = bi.biWidth;
    int origHeight = bi.biHeight;
    int origPadding = (4 - (origWidth * sizeof(RGBTRIPLE)) % 4) % 4;


    bi.biWidth *= size;
    bi.biHeight *= size;

    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + padding) * abs(bi.biHeight);
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);


    // iterate over infile's scanlines
    int lineSize = (sizeof(RGBTRIPLE) * origWidth) + origPadding;
    for (int i = 0, biHeight = abs(origHeight); i < biHeight; i++)
    {
        for (int j = 0; j < size; j++)
        {
            writeLine(origWidth, origPadding,size,padding,inptr,outptr);
            // go back to beginning of scanline to reprint
            fseek(inptr, -lineSize, SEEK_CUR);
        }
        fseek(inptr, lineSize, SEEK_CUR);

    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}

void writeLine(int origWidth, int origPadding,int size,int padding,FILE *inptr, FILE *outptr) {
    // iterate over pixels in scanline
    for (int j = 0; j < origWidth; j++)
    {

        // temporary storage
        RGBTRIPLE triple;

        // read RGB triple from infile
        fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

        for (int k = 0; k < size; k++)
        {
            // write RGB triple to outfile
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }

    }

    // skip over padding, if any
    fseek(inptr, origPadding, SEEK_CUR);

    // then add it back (to demonstrate how)
    for (int l = 0; l < padding; l++)
    {
        fputc(0x00, outptr);
    }
}

