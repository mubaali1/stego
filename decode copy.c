// #include <stdio.h>
// #include "decode.h"
// #include "types.h"
// #include <string.h>
// #include <stdlib.h>
// #include "common.h"
// #include <unistd.h>

// void decode_string(char *buffer, int t_bits, DecodeInfo *decInfo, char *destination)
// {
//     fread(buffer, 1, t_bits, decInfo->fptr_src_image);
//     printf("total bytes:%d\n",t_bits);
//     int size = t_bits / 8;

//     for (int i = 0; i < size; i++)
//     {
//         destination[i] = 0;
//         for (int j = 0; j < 8; j++)
//         {
//             int buffer_index = i * 8 + j;
//             int last_bit = buffer[buffer_index] & 1;
//             destination[i] = (destination[i] << 1) | last_bit;
//         }
//     }
//     destination[size] = '\0'; 
//     printf("decoded string: %s\n",destination);
// }

// Status do_decoding(DecodeInfo *decInfo)
// {
//     if (open_file(decInfo) == e_failure)
//     {
//         return e_failure;
//     }
//     else
//     {
//         printf("INFO: File Opened Successfully\n");
//     }
//     char header[100];
//     fread(header,1,54,decInfo->fptr_src_image);
//     if(!(header[0]=='B' && header[1]=='M'))
//     {
//         printf("ERROR: It is not a .BMP file\n");
//         return e_failure;
//     }

//     // 2. Decoding magic string
//     char expected_magic[100];
//     printf("Enter the Magic string: ");
//     scanf("%[^\n]",expected_magic);
  
//     int Mstr_len = strlen(expected_magic);
//     decInfo->magic_string = malloc(Mstr_len + 1);
//     decInfo->Magic_strings = malloc(Mstr_len + 1);
//     strcpy(decInfo->magic_string, expected_magic);
//     int t_bits = Mstr_len * 8;
//     unsigned char buffer[1000];
//     decode_string(buffer, t_bits, decInfo, decInfo->Magic_strings);
//     printf("Expected: %s\n", decInfo->magic_string);
//     printf("Decoded : %s\n", decInfo->Magic_strings);
//     if (strcmp((char *)decInfo->magic_string, (char *)decInfo->Magic_strings) != 0)
//     {
//         printf("ERROR: Magic string mismatch! Aborting decoding.\n");
//         return e_failure;
//     }
//     else
//     {
//         printf("INFO: Magic string verified successfully.\n");
//     }

//     // 3.Decoding size of extention

//     unsigned char extn_size[32];
//     fread(extn_size, 1, 32, decInfo->fptr_src_image);
//     int decoded_value = 0;
//     for (int i = 0; i < 32; i++)
//     {
//         int lsb = extn_size[i] & 1;
//         decoded_value |= (lsb << (31 - i));
//     }
//     printf("Decoded extension size : %d\n", decoded_value);

//     // 4. decoding extension
//     unsigned char extension[10];
//     decInfo->secret_file_extn = malloc(10);
//     decode_string(extension, decoded_value * 8, decInfo, decInfo->secret_file_extn);

//     // 5.decode size of secret code
//     unsigned char secret_size[32];
//     fread(secret_size, 1, 32, decInfo->fptr_src_image);
//     int decoded_Secret_size = 0;
//     for (int i = 0; i < 32; i++)
//     {
//         int lsb = secret_size[i] & 1;
//         decoded_Secret_size |= (lsb << (31 - i));
//     }
//     printf("Decoded secret code size : %d\n", decoded_Secret_size);

//     // 6. decode  secret code

//     unsigned char secret_code[1000];
//     decInfo->secret_data = malloc(1000);
//     decode_string(secret_code, decoded_Secret_size * 8, decInfo, decInfo->secret_data);
//     printf("%s\n",decInfo->secret_data);
//     char full_filename[100] = {0};
//     strcpy(full_filename, decInfo->decode_dest_fname);
//     strcat(full_filename, decInfo->secret_file_extn);
//     decInfo->decode_dest = fopen(full_filename, "wb");
//     fwrite(decInfo->secret_data, 1, decoded_Secret_size, decInfo->decode_dest);

//     // Cleanup
//     fclose(decInfo->decode_dest);
//     free(decInfo->Magic_strings);
//     free(decInfo->secret_file_extn);
//     free(decInfo->secret_data);

//     return e_success;
// }

// Status open_file(DecodeInfo *decInfo)
// {
//     // Src Image file
//     decInfo->fptr_src_image = fopen(decInfo->src_image_fname, "rb");
//     // Do Error handling
//     if (decInfo->fptr_src_image == NULL)
//     {
//         perror("fopen");
//         fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->src_image_fname);

//         return e_failure;
//     }
//     return e_success;
// }