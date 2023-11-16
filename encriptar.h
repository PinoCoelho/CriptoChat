#ifndef ENCRIPTAR_H
#define ENCRIPTAR_H

#include <iostream>
#include <openssl/aes.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/hmac.h>

class Encriptador {
public:
    static void handleErrors();
    static void encrypt(const std::string& plaintext, const unsigned char* key, const unsigned char* iv, std::string& ciphertext);
    static void decrypt(const std::string& ciphertext, const unsigned char* key, const unsigned char* iv, std::string& decryptedText);
    static void generateHMAC(const std::string& data, const unsigned char* key, std::string& hmac);
    static void generarClaveYIV(unsigned char* key, unsigned char* iv);
};

#endif // ENCRIPTAR_H
