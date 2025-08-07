#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>
#include <stdlib.h>
#include "common.h"
#include <unistd.h>

Status do_encoding(EncodeInfo *encInfo)
{
    // Test open_files
    if (open_files(encInfo) == e_failure)
    {
        printf("ERROR: Failed to open the file\n");
        return e_failure;
    }
    else
    {
        printf("INFO: File opened Successfully\n");
    }
    // checking secret Data will fit in image file or not
    if (check_capacity(encInfo) == e_failure)
    {
        return e_failure;
    }
    // 1. coping Header
    for (int i = 0; i <= 100; i++)
    {
        printf("INFO:Copying header...(%d%%)\r", i);
        usleep(5000); // microsecond
        fflush(stdout);
    }
    if (copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
    {
       printf("INFO: Header copied Successfully\n");
    }
    else
    {
        printf("Faild to copy .BMP header\n");
        return e_failure;
    }
  // 2. Encoding magic string
    for (int i = 0; i <= 100; i++)
    {
        printf("INFO: Encoding magic strings...(%d%%)\r", i);
        usleep(10000); // microsecond
        fflush(stdout);
    }
    if (encode_magic_string(MAGIC_STRING, encInfo) == e_success)
    {
       printf("INFO: Encoded Magic String Successfully\n");
    }
     else
    {
        printf("Failed to encode magic string...\n");
        return e_failure;
    }
   // 3.encoding secrect file size
    for (int i = 0; i <= 100; i++)
    {
        printf("INFO: Encodeing secret file size...(%d%%)\r", i);
        usleep(10000); // microsecond
        fflush(stdout);
    }
    if (encode_secret_file_extn_size(4, encInfo) == e_success)
    {
      printf("INFO: Encoded secret file size Successfully\n");
    }
    else
    {
        printf("Failed to encode secret file size...\n");
        return e_failure;
    }
  // 4.encoding secret file extension
    for (int i = 0; i <= 100; i++)
    {
        printf("INFO: Encodeing secret file extn...(%d%%)\r", i);
        usleep(10000); // microsecond
        fflush(stdout);//forces the buffered output to be written immediately ensure pending output displayed.
    }
    // printf("Encodeing secret file extn...\n");
    strcpy(encInfo->extn_secret_file, ".txt");
    if (encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_success)
    {
        printf("INFO: Encoded secret file extn Successfully\n");
    }
    else
    {
        printf("Failed to encode secret file size...\n");
        return e_failure;
    }
  // 5.Encoding secret file size
    encInfo->size_secret_file = 0;
    int i = 0;
    char ch;
    while (fread(&ch, 1, 1, encInfo->fptr_secret) == 1)
    {
        if (i >= MAX_SECRET_BUF_SIZE)
        {
            fprintf(stderr, "Secret file too large. Max supported: %d bytes\n", MAX_SECRET_BUF_SIZE);
            return e_failure;
        }
        encInfo->secret_data[i++] = ch;
        encInfo->size_secret_file++;
    }
    for (int i = 0; i <= 100; i++)
    {
        printf("INFO: Encoding secret file size...(%d%%)\r", i);
        usleep(10000); // microsecond
        fflush(stdout);
    }
    if (encode_secret_file_size(encInfo->size_secret_file, encInfo) == e_success)
    {
       printf("INFO: Encoded secret file size Successfully\n");
    }
     else
    {
        printf("Failed to encode secret file size...\n");
        return e_failure;
    }
   // 6.encoding secret data
    for (int i = 0; i <= 100; i++)
    {
        printf("INFO: Encoding secret message...(%d%%)\r", i);
        usleep(20000); // microsecond
        fflush(stdout);
    }
    // printf("\nEncoding secret message...\n");
    if (encode_secret_file_data(encInfo) == e_success)
    {
     printf("INFO: Encoded secret message Successfully\n");
    }
    else
    {
        printf("Failed to encode secret file...\n");
        return e_failure;
    }
   // 7. copying remaining data
    char remain;
    while (fread(&remain, 1, 1, encInfo->fptr_src_image) > 0)
        fwrite(&remain, 1, 1, encInfo->fptr_stego_image);
    // close the files
    fclose(encInfo->fptr_src_image);
    fclose(encInfo->fptr_stego_image);
     return e_success;
}
uint get_file_size(FILE *fptr, EncodeInfo *encInfo)
{
    char ch;
    int len = 0;
    int i = 0;
    while (fread(&ch, 1, 1, fptr) == 1)
    {
        len++;
    }
    fseek(fptr, 0, SEEK_SET);
    return len;
}
Status check_capacity(EncodeInfo *encInfo)
{
    encInfo->image_size = get_image_size_for_bmp(encInfo->fptr_src_image);
    // printf("INFO: Image size = %u\n", encInfo->image_size);
    encInfo->secret_data_size = get_file_size(encInfo->fptr_secret, encInfo);
    // printf("secret data size: %u\n", encInfo->secret_data_size);
   if (encInfo->secret_data_size == 0)
    {
        printf("No data available in secret file\n");
        return e_failure;
    }
    if ((encInfo->secret_data_size * 8 + 166) > encInfo->image_size)
    {
        printf(" Secret file size is greater than image file\n");
        return e_failure;
    }
}
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    long int t_bits = encInfo->size_secret_file * 8;
    unsigned char *buffer = (unsigned char *)malloc(t_bits);
    if (buffer == NULL)
    {
        printf("Memory allocation failed\n");
        return e_failure;
    }
    fread(buffer, 1, t_bits, encInfo->fptr_src_image);
    int mask = 1;
    int bit_val;
    encode_char_byte(encInfo->secret_data, t_bits, buffer, encInfo);
    return e_success;
}
 OperationType check_operation_type(char *argv[])
{
    if (strcmp(argv[1], "-e") == 0)
    {
       return e_encode; 
    }
    else if (strcmp(argv[1], "-d") == 0)
    {
         return e_decode;
    }
    else
    {
        return e_unsupported;
    }
     
}
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char buffer[54];
    if (fread(buffer, 1, 54, fptr_src_image) != 54)
    {
        printf("Error in reading Header");
        return e_failure;
    }
    if (!(buffer[0] == 'B' && buffer[1] == 'M'))
    {
        printf("ERROR It is not a .BMP file\n");
        return e_failure;
    }
    if (fwrite(buffer, 1, 54, fptr_dest_image) != 54)
    {
        printf("Error in writing Header");
        return e_failure;
    }
   return e_success;
}
void encode_char_byte(const char *magic_string, int t_bits, unsigned char buffer[], EncodeInfo *encInfo)
{
    for (int i = 0; i < t_bits; i++)
    {
        int byte_index = i / 8;      // index in buffer
        int bit_index = 7 - (i % 8); // Start from MSB
       int bit_val = (magic_string[byte_index] >> bit_index) & 1; // get the bit from moving towards lsb
        buffer[i] = buffer[i] & (~1);    
        // Clear LSB
        buffer[i] = buffer[i] | bit_val; 
        // Set LSB with data bit
    }
    fwrite(buffer, 1, t_bits, encInfo->fptr_stego_image);
}
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    unsigned char buffer[1000];
    int t_bits = strlen(magic_string) * 8;
    fread(buffer, 1, t_bits, encInfo->fptr_src_image);
    int mask = 1;
    int bit_val;
    encode_char_byte(magic_string, t_bits, buffer, encInfo);
    // test_decode(buffer, t_bits, encInfo);
    return e_success;
}
void encode_integer_to_lsb(int value, unsigned char *buffer, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    fread(buffer, 1, 32, fptr_src_image);
    // Embed each bit of the integer into the LSBs of the buffer
    for (int i = 31; i >= 0; i--)
    {
        int bit = (value >> i) & 1;
        buffer[31 - i] = (buffer[31 - i] & ~1) | bit;
    }
    // Write modified buffer to stego image
    fwrite(buffer, 1, 32, fptr_stego_image) ;
}
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    unsigned char buffer[100];
    int t_bits = strlen(file_extn) * 8;
    fread(buffer, 1, t_bits, encInfo->fptr_src_image);
    int mask = 1;
    int bit_val;
    encode_char_byte(file_extn, t_bits, buffer, encInfo);
    // fseek(encInfo->fptr_src_image, -t_bits, SEEK_CUR); // move file pointer back to original position
    // test_decode(buffer, t_bits, encInfo);
    return e_success;
}
Status encode_secret_file_extn_size(int file_ext_size, EncodeInfo *encInfo)
{
    unsigned char buffer[32];
    encode_integer_to_lsb(file_ext_size, buffer, encInfo->fptr_src_image, encInfo->fptr_stego_image);
    return e_success;
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    unsigned char buffer[32];
    encode_integer_to_lsb(file_size, buffer, encInfo->fptr_src_image, encInfo->fptr_stego_image);
    return e_success;
}
/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    // printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    // printf("height = %u\n", height);
    fseek(fptr_image, 0, SEEK_SET);
    // Return image capacity
    return width * height * 3;
}

/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
        return e_failure;
    }
   // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

        return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

        return e_failure;
    }

    // No failure return e_success
    return e_success;
}
