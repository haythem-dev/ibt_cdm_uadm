#include "pch.h"
#include <string.h>
#include <cstring>
#include <stdlib.h>
#include <iostream>

#include <openssl/evp.h>
#include <openssl/err.h>
#include "aes_util.h"

/**
 * The two functions bellow are taken from the openssl official website :
 * https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption
 * You can read more there
 */

int aes_encrypt(unsigned char* plaintext, int plaintext_len, unsigned char* key, unsigned char* iv, unsigned char* ciphertext)
{
    EVP_CIPHER_CTX* ctx;

    int len;

    int ciphertext_len;

    /* Create and initialise the context */
    if (!(ctx = EVP_CIPHER_CTX_new()))
    {
        handleErrors();
    }
    /*
     * Initialise the encryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
    {
        handleErrors();
    }
    /*
     * Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
    {
        handleErrors();
    }
    ciphertext_len = len;

    /*
     * Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
    {
        handleErrors();
    }
    ciphertext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int aes_decrypt(unsigned char* ciphertext, int ciphertext_len, unsigned char* key, unsigned char* iv, unsigned char* plaintext)
{
    EVP_CIPHER_CTX* ctx;

    int len;

    int plaintext_len;

    /* Create and initialise the context */
    if (!(ctx = EVP_CIPHER_CTX_new()))
    {
        handleErrors();
    }
    /*
     * Initialise the decryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
    {
        handleErrors();
    }
    /*
     * Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary.
     */
    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
    {
        handleErrors();
    }
    plaintext_len = len;

    /*
     * Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
    {
        handleErrors();
    }
    plaintext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

void handleErrors()
{
    ERR_print_errors_fp(stderr);
    //abort();
}

void hashPassword(const char* string, char outputBuffer[65])
{

#if OPENSSL_VERSION_NUMBER < 0x10100000L
#  define EVP_MD_CTX_new   EVP_MD_CTX_create
#  define EVP_MD_CTX_free  EVP_MD_CTX_destroy
#endif

    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    unsigned char* hash = (unsigned char*)malloc(EVP_MAX_MD_SIZE * sizeof(char));
    unsigned int hashSize = 65;
    unsigned char* tempOutBuff = (unsigned char*)malloc(hashSize * sizeof(char));

    EVP_DigestInit(ctx, EVP_sha256());
    EVP_DigestUpdate(ctx, string, strlen(string));
    EVP_DigestFinal(ctx, tempOutBuff, &hashSize);

    for (int i = 0; i < (int)hashSize; i++)
    {
        sprintf(outputBuffer + (i * 2), "%02x", tempOutBuff[i]);
    }
	EVP_MD_CTX_free(ctx);
	free(tempOutBuff);
	free(hash);
	// Ensure the output buffer is null-terminated
    outputBuffer[64] = 0;
}

std::string AESEncrypt(std::string plainText, std::string pass)
{
    // Converting string to unsigned character (bytes) array
    unsigned char* plainTextBytes = (unsigned char*)plainText.c_str();

    // Allocate memory for hash bytes (chars) array with 65 elements
    char* hash = (char*)malloc(sizeof(char) * 65);

    // Allocate memory for cipher bytes (chars) array with 1024 elements
    unsigned char* cipher = (unsigned char*)malloc(sizeof(char) * 1024);

    // Here we hash the plain text password
    hashPassword((const char*)pass.c_str(), hash);

    // We convert the char array (hash variable) into an std::string
    std::string hashStr(hash);
    //cout << "SHA256: " << hashStr << endl;

    // The hash contains the encrytion key and the initialization vector -> (iv)
    // so we divide it between the key and the iv using std::string method substr()
    std::string keyStr = hashStr.substr(0, 16);
    //cout << "Key: " << keyStr << endl;

    std::string iv = hashStr.substr(16, 16);
    //cout << "iv: " << iv << endl;

    // here we initiate the encryption process
    int cipherSize = aes_encrypt(plainTextBytes, (int)strlen((char*)plainTextBytes), (unsigned char*)keyStr.c_str(), (unsigned char*)iv.c_str(), cipher);

    // We encode the cipher into a base64 encoded std::string  
    // string base64Cipher = base64_encode(
    //     cipher,
    //     cipherSize);

    unsigned char base64Cipher[1024];
    EVP_EncodeBlock(base64Cipher, cipher, cipherSize);

    // We encode the iv to the base64
    //std::string base64Iv = base64_encode(iv, iv.length());

    // Append the base64 encode iv to the end of the base64 encoded cipher with a `:` in between
    // string cipherStr((char *) base64Cipher.c_str());
    std::string cipherStr((char*)base64Cipher);

	// delete all malloced memory
	free(cipher);
	free(hash);

    return cipherStr;
}

std::string AESDecrypt(std::string cipher, std::string pass)
{
    // string cipherDecoded = base64_decode(cipher);
    // unsigned char * cipherBytes = (unsigned char *) cipherDecoded.c_str();

    void* cipherBytes = (void*)malloc(sizeof(char) * 1024);
	unsigned char* ci = (unsigned char*)cipher.c_str();
    int cilen = (int)strlen((char*)ci);
    int cblen = EVP_DecodeBlock((unsigned char*)cipherBytes, ci, cilen);
    cblen = 16 * max(cblen / 16, 1); // size down to the next lower 16bytes, but min 16 bytes

    // Converting string to unsigned character (bytes) array
    unsigned char* plainTextBytes = (unsigned char*)malloc(sizeof(char) * 1024);
	memset(plainTextBytes, 0, 1024);

    // Allocate memory for hash bytes (chars) array with 65 elements
    char* hash = (char*)malloc(sizeof(char) * 65);

    // Here we hash the plain text password
    hashPassword((const char*)pass.c_str(), hash);

    // We convert the char array (hash variable) into an std::string
    std::string hashStr(hash);

    // The hash contains the encrytion key and the initialization vector -> (iv)
    // so we divide it between the key and the iv using std::string method substr()
    std::string keyStr = hashStr.substr(0, 16);
    // cout << "Key: " << keyStr << endl;

    std::string iv = hashStr.substr(16, 16);
    // cout << "iv: " << iv << endl;

    int len = aes_decrypt((unsigned char*)cipherBytes, cblen, (unsigned char*)keyStr.c_str(), (unsigned char*)iv.c_str(), plainTextBytes);

	// release the allocated memory
	free(cipherBytes);
	free(hash);

	if ((len >= 0) && (len < 1024))
    {
        // Ensure the plainText is null-terminated
        plainTextBytes[len] = '\0'; // Ensure null termination for safety
    }

    std::string plainText((char*)plainTextBytes);
	free(plainTextBytes); // Free the allocated memory for plainTextBytes

    return plainText;
}

/*
std::string base64_encode(std::string data, size_t in_len)
{
    size_t len_encoded = (in_len + 2) / 3 * 4;
    const char* base64_chars_ = base64_chars;
    std::string ret;
    ret.reserve(len_encoded);
    unsigned int pos = 0;
    unsigned char trailing_char = '.';
    unsigned char* bytes_to_encode = (unsigned char*)data.c_str();
    while (pos < in_len)
    {
        ret.push_back(base64_chars_[(bytes_to_encode[pos + 0] & 0xfc) >> 2]);

        if (pos + 1 < in_len)
        {
            ret.push_back(base64_chars_[((bytes_to_encode[pos + 0] & 0x03) << 4) + ((bytes_to_encode[pos + 1] & 0xf0) >> 4)]);

            if (pos + 2 < in_len)
            {
                ret.push_back(base64_chars_[((bytes_to_encode[pos + 1] & 0x0f) << 2) + ((bytes_to_encode[pos + 2] & 0xc0) >> 6)]);
                ret.push_back(base64_chars_[bytes_to_encode[pos + 2] & 0x3f]);
            }
            else
            {
                ret.push_back(base64_chars_[(bytes_to_encode[pos + 1] & 0x0f) << 2]);
                ret.push_back(trailing_char);
            }
        }
        else
        {
            ret.push_back(base64_chars_[(bytes_to_encode[pos + 0] & 0x03) << 4]);
            ret.push_back(trailing_char);
            ret.push_back(trailing_char);
        }
        pos += 3;
    }
    return ret;
}

static unsigned int pos_of_char(const unsigned char chr)
{
    //
    // Return the position of chr within base64_encode()
    //

    if (chr >= 'A' && chr <= 'Z') return chr - 'A';
    else if (chr >= 'a' && chr <= 'z') return chr - 'a' + ('Z' - 'A') + 1;
    else if (chr >= '0' && chr <= '9') return chr - '0' + ('Z' - 'A') + ('z' - 'a') + 2;
    else if (chr == '+' || chr == '-') return 62; // Be liberal with input and accept both url ('-') and non-url ('+') base 64 characters (
    else if (chr == '/' || chr == '_') return 63; // Ditto for '/' and '_'

    return 0;
}

static std::string base64_decode(std::string encoded_string)
{
    if (encoded_string.empty())
    {
        return std::string();
    }

    size_t length_of_string = encoded_string.length();
    size_t pos = 0;

    //
    // The approximate length (bytes) of the decoded string might be one or
    // two bytes smaller, depending on the amount of trailing equal signs
    // in the encoded string. This approximation is needed to reserve
    // enough space in the string to be returned.
    //
    size_t approx_length_of_decoded_string = length_of_string / 4 * 3;
    std::string ret;
    ret.reserve(approx_length_of_decoded_string);

    while (pos < length_of_string)
    {
        //
        // Iterate over encoded input string in chunks. The size of all
        // chunks except the last one is 4 bytes.
        //
        // The last chunk might be padded with equal signs or dots
        // in order to make it 4 bytes in size as well, but this
        // is not required as per RFC 2045.
        //
        // All chunks except the last one produce three output bytes.
        //
        // The last chunk produces at least one and up to three bytes.
        //

        size_t pos_of_char_1 = pos_of_char(encoded_string[pos + 1]);

        //
        // Emit the first output byte that is produced in each chunk:
        //
        ret.push_back(static_cast<std::string::value_type>(((pos_of_char(encoded_string[pos + 0])) << 2) + ((pos_of_char_1 & 0x30) >> 4)));

        if ((pos + 2 < length_of_string) &&  // Check for data that is not padded with equal signs (which is allowed by RFC 2045)
            encoded_string[pos + 2] != '=' &&
            encoded_string[pos + 2] != '.'            // accept URL-safe base 64 strings, too, so check for '.' also.
            )
        {
            //
            // Emit a chunk's second byte (which might not be produced in the last chunk).
            //
            unsigned int pos_of_char_2 = pos_of_char(encoded_string[pos + 2]);
            ret.push_back(static_cast<std::string::value_type>(((pos_of_char_1 & 0x0f) << 4) + ((pos_of_char_2 & 0x3c) >> 2)));

            if ((pos + 3 < length_of_string) &&
                encoded_string[pos + 3] != '=' &&
                encoded_string[pos + 3] != '.'
                )
            {
                //
                // Emit a chunk's third byte (which might not be produced in the last chunk).
                //
                ret.push_back(static_cast<std::string::value_type>(((pos_of_char_2 & 0x03) << 6) + pos_of_char(encoded_string[pos + 3])));
            }
        }
        pos += 4;
    }
    return ret;
}
*/