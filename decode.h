#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1000
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    /* Source Image info */
    char *src_image_fname;
    FILE *fptr_src_image;
    char *Magic_strings;
    char *secret_file_extn;
    char *secret_data;

    char *magic_string;

    /* Secret File Info */
    char *secret_fname;
    char extn_secret_file[MAX_FILE_SUFFIX];
    long secret_extn_size;
    long decoded_Secret_size;
    long size_secret_file;

    /* Stego Image Info */
    char *decode_dest_fname;
    FILE *decode_dest;

} DecodeInfo;

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

Status open_file(DecodeInfo *decInfo);

void decode_string(char *buffer, int t_bits, DecodeInfo *decInfo,char *destination);

#endif
 