/*
Name   : Taiyyaba Naikodi
Date   : 12/05/2025
project name : Steganography
Description : This project demonstrates the implementation of image-based steganography using c. It hides secret text inside a bitmap
image(.bmp) file using the least significant Bit (LSB) technigue and later extracts it to verify the process.

Objective :
Embed a secret message inside a BMP image using LSB technique.
Retrive the hidden message from stego image.
Maintain image quality and ensure imperceptibility.

We have help option to know the prototype as ./a.out -h stego.bmp

In a 24-bit BMP file,each pixel has 3 bytes(R, G, B). The LSB of each byte is modified to store message bits.

Example :
Original byte(Red channel) :10110110
Modified with message bit 1 :10110111

=====ENcoding process======
Read the input BMP image header (54 bytes).
Read and convert the secret txt into binary.
Modify the LSB of image pixels to embed each bit of the message.
Save the modified image as the encoded file.

=======Decoding process=======
Read the header and pixel data of the stego image.
Extract LSBs from each byte used during encoding.
Reconstruct the original message from binary data.
Input Image: input.bmp(24 bit, uncompressed)
Secret txt: "Hello, c steganography!"
Encoded Image: encode.bmp
Decoded Image: "Hello, c steganography!"
*/
#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>
#include "decode.h"

void display_Error(char *argv[])
{
    printf("ERROR: Incorrect format of command line arguments.\n");
    printf("Use "
           "%s -h"
           " for help\n",
           argv[0]);
}
void display_help(char *argv[])
{
    printf("Help menu for LSB Image Steganography\n\n");
    printf("Encoding : %s -e <.bmp_file> <.text_file> <Output file[optional]>\n", argv[0]);
    printf("Decoding : %s -d <.bmp_file> <Output file[optional]>\n\n", argv[0]);
}
int main(int argc, char *argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;
    uint img_size;
   if (argc < 2)
    {
        display_Error(argv);
        return 1;
    }
    if (strcmp(argv[1], "-h") == 0)
    {
        display_help(argv);
        return 0;
    }
    else if (check_operation_type(argv) == e_encode)
    {
        if (argc == 4)
        {
            encInfo.src_image_fname = argv[2];
            encInfo.secret_fname = argv[3];
            encInfo.stego_image_fname = "stego.bmp";
        }
        else if (argc == 5)
        {
            encInfo.src_image_fname = argv[2];
            encInfo.secret_fname = argv[3];
            int len = strlen(argv[4]);
            printf("%d",len);
            if (len > 4 && strstr(argv[4], ".bmp"))
            {
              encInfo.stego_image_fname = argv[4];  
            }
            else
            {
                encInfo.stego_image_fname = strcat(argv[4], ".bmp");
            }
        }
         else
        {
            display_Error(argv);
            return 0;
        }
        if (do_encoding(&encInfo) == e_success)
        {
            printf("***Encoding Done Sucessfully***\n");
        }
        else
        {
            printf("Encoding  Failed!!!\n");
        }
    }
    else if (check_operation_type(argv) == e_decode)
    {
        if (argc == 3)
        {
            decInfo.src_image_fname = argv[2];
            decInfo.decode_dest_fname = "decode";
        }
        else if (argc == 4)
        {
            decInfo.src_image_fname = argv[2];
            decInfo.decode_dest_fname = argv[3];
        }
        else
        {
            display_Error(argv);
            return 0;
        }
        if (do_decoding(&decInfo)==e_success)
        {
            printf("***Decoding  Done Sucessfully***\n");
        }
        else
        {
            printf("Decoding  Failed!!!\n");
        }
    }
    else
    {
        display_Error(argv);
        return 1;
    }
   return 0;
}
