# bmp-steg
bmp-steg is a steganography tool that allows you to hide secret messages within BMP image files. It uses various techniques to hide encrypted data in the gaps of the BMP file structure.

## How it works
bmp-steg works by taking advantage of padding within the pixel array, as well between various structure blocks (gaps). The message is first encrypted using a BBS encryption scheme, and then the encrypted message is hidden within the image data. The resulting image appears identical to the original image, but contains the hidden message. The algorithm tries its hardest to make use of existing gaps, and often will not change the size of the existing file.

## Security features
bmp-steg has several security features that make it a secure way to hide secret messages:

* **Password-based encryption**: The message is encrypted using a password-based encryption scheme, which means that only someone with the correct password can decrypt and read the message.
* **Blum Blum Shub**: Uses either an optional or internal initialValue to generate a BBS byte stream, combined with xor, to encrypt the password.

# Usage
bmp-steg can be used from the command line to hide and extract secret messages from BMP image files. Here are the basic usage instructions:

## Options
* `-s`: Save/Hide a secret message within an image.
* `-g`: Get/Extract a hidden message from an image.
* `-h`: Specify an encryption key to use for encryption and decryption.

## Hiding a message
To hide a message within a BMP image file, use the following command:


```
bmp-steg -s <image_file> <message> [-h <hash>]
```
`<image_file>` is the name of the BMP image file that you want to hide the message in.\
`<message>` is the secret message that you want to hide.\
`<hash>` is the 256bit passcode that will be used to encrypt the message. If you don't specify a password, a default passcode will be used.

Example:

```
bmp-steg -s image.bmp "Hello, World!" -p mypassword -h DEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEF
```
This will hide the message "Hello, World!" within the image file "image.bmp", using the password "mypassword" and the above hash string.

## Extracting a message
To extract a message from a BMP image file, use the following command:

```
bmp-steg -g <image_file> [-h <hash>]
```
`<image_file>` is the name of the BMP image file that contains the hidden message.
`<hash>` is the 256bit passcode that will be used to decrypt the message, if originally provided.
Example:

```
bmp-steg -g image.bmp -p qwert -h DEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEF
```
This will extract the message from the image file "image.bmp", using the above hash string.

Note: If you don't specify a password or initial value, the default values will be used. However, this is not recommended, as it reduces the security of the encryption.