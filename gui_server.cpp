#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

void handleErrors() {
    ERR_print_errors_fp(stderr);
    abort();
}

void encrypt(const std::string& plaintext, const unsigned char* key, const unsigned char* iv, std::string& ciphertext) {
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

void decrypt(const std::string& ciphertext, const unsigned char* key, const unsigned char* iv, std::string& decryptedText) {
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
