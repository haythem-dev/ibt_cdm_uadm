#include <iostream>

//static const char* base64_chars = {
//             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
//             "abcdefghijklmnopqrstuvwxyz"
//             "0123456789"
//             "-_" };

void handleErrors();
void hashPassword(const char* string, char outputBuffer[65]);

int aes_encrypt(unsigned char* plaintext, int plaintext_len, unsigned char* key, unsigned char* iv, unsigned char* ciphertext);
std::string AESEncrypt(std::string plainText, std::string pass);

int aes_decrypt(unsigned char* ciphertext, int ciphertext_len, unsigned char* key, unsigned char* iv, unsigned char* plaintext);
std::string AESDecrypt(std::string cipher, std::string pass);

//std::string base64_decode(std::string data);
//std::string base64_encode(std::string data, size_t len);
