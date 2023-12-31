#include "encriptar.h"
#include <QApplication>
#include <iostream>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/hmac.h>
#include <fstream>


void Encriptador::handleErrors() {
    ERR_print_errors_fp(stderr);
    abort();
}

void Encriptador::encrypt(const std::string& plaintext, const unsigned char* key, const unsigned char* iv, std::string& ciphertext) {
    EVP_CIPHER_CTX *ctx;

    if (!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, key, iv))
        handleErrors();

    int len;
    int ciphertext_len;

    ciphertext.resize(plaintext.size() + AES_BLOCK_SIZE); // Allocate enough space for padding

    if (1 != EVP_EncryptUpdate(ctx, reinterpret_cast<unsigned char*>(&ciphertext[0]), &len,
                               reinterpret_cast<const unsigned char*>(plaintext.c_str()), plaintext.size()))
        handleErrors();

    ciphertext_len = len;

    if (1 != EVP_EncryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(&ciphertext[len]), &len))
        handleErrors();

    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    ciphertext.resize(ciphertext_len); // Adjust size to actual ciphertext length
}

void Encriptador::decrypt(const std::string& ciphertext, const unsigned char* key, const unsigned char* iv, std::string& decryptedText) {
    EVP_CIPHER_CTX *ctx;

    if (!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, key, iv))
        handleErrors();

    int len;
    int plaintext_len;

    decryptedText.resize(ciphertext.size() + AES_BLOCK_SIZE); // Allocate enough space for padding

    if (1 != EVP_DecryptUpdate(ctx, reinterpret_cast<unsigned char*>(&decryptedText[0]), &len,
                               reinterpret_cast<const unsigned char*>(ciphertext.c_str()), ciphertext.size()))
        handleErrors();

    plaintext_len = len;

    if (1 != EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(&decryptedText[len]), &len))
        handleErrors();

    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    decryptedText.resize(plaintext_len); // Adjust size to actual plaintext length
}

void Encriptador::generarClaveYIV(unsigned char* key, unsigned char* iv) {
    // Generar una clave aleatoria
    if (RAND_bytes(key, 16) != 1) {
        handleErrors();
    }

    // Generar un IV aleatorio
    if (RAND_bytes(iv, AES_BLOCK_SIZE) != 1) {
        handleErrors();
    }
}

// Función para duplicar bits y corregir errores
void correctErrors(const std::string& originalText) {
    // Generar una clave aleatoria de 16 bytes (128 bits)
    unsigned char key[16];
    if (RAND_bytes(key, sizeof(key)) != 1)
        Encriptador::handleErrors();

    // Generar un IV (vector de inicialización) aleatorio de 16 bytes
    unsigned char iv[AES_BLOCK_SIZE];
    if (RAND_bytes(iv, sizeof(iv)) != 1)
        Encriptador::handleErrors();

    // Cifrar el texto
    std::string ciphertext;
    Encriptador::encrypt(originalText, key, iv, ciphertext);

    // Imprimir el texto cifrado
    std::cout << "Texto original: " << originalText << std::endl;
    std::cout << "Texto cifrado: " << ciphertext << std::endl;

    // Guardar el texto cifrado en un archivo (opcional)
    std::ofstream outputFile("encrypted.txt", std::ios::binary);
    outputFile.write(reinterpret_cast<char*>(iv), AES_BLOCK_SIZE);
    outputFile.write(ciphertext.c_str(), ciphertext.size());
    outputFile.close();

    // Descifrar el texto
    std::string decryptedText;
    Encriptador::decrypt(ciphertext, key, iv, decryptedText);

    // Imprimir el texto descifrado
    std::cout << "Texto descifrado: " << decryptedText << std::endl;

    // Generar el HMAC
    std::string hmac;
    Encriptador::generateHMAC(decryptedText, key, hmac);

    // ... (Código para verificar el HMAC, imprimir o manejar el resultado)
}

void Encriptador::generateHMAC(const std::string& data, const unsigned char* key, std::string& hmac) {
    unsigned int len;
    unsigned char result[EVP_MAX_MD_SIZE];

    HMAC(EVP_sha256(), key, strlen(reinterpret_cast<const char*>(key)),
         reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), result, &len);

    hmac.assign(reinterpret_cast<char*>(result), len);
}

