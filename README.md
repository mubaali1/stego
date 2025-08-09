# stego
stenography we are hiding our text in image and send then we will extracts that information. (encoding and decodin) 


--------------------------------- for encodeing output -------------------------------------------------------------------------------

$ ./a.exe -e beautiful.bmp secret.txt stego.bmp
9INFO: File opened Successfully
INFO: Header copied Successfully
INFO: Encoded Magic String Successfully
INFO: Encoded secret file size Successfully
INFO: Encoded secret file extn Successfully
INFO: Encoded secret file size Successfully
INFO: Encoded secret message Successfully
***Encoding Done Sucessfully***
--------------------------------- for decoding output -------------------------------------------------------------------------------

$ ./a.exe -d stego.bmp 
INFO: File Opened Successfully
Enter the Magic string: #*
INFO: Magic string verified successfully.
INFO: Decoded extension size Successfully
INFO: Decoded extention Successfully
INFO: Decoded secret file size Successfully
INFO: Decoding secret file data successfully
***Decoding  Done Sucessfully***



