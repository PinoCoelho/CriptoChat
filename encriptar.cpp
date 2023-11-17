#include "encriptar.h"
#include <openssl/rand.h>
#include <fstream>

void Encriptador::handleErrors() {
    ERR_print_errors_fp(stderr);
    abort();
}

void Encriptador::encrypt(const std::string& plaintext, const unsigned char* key, const unsigned char* iv, std::string& ciphertext) {
    // Implementación de la función de encriptación
}

void Encriptador::decrypt(const std::string& ciphertext, const unsigned char* key, const unsigned char* iv, std::string& decryptedText) {
    // Implementación de la función de desencriptación
}

void Encriptador::generateHMAC(const std::string& data, const unsigned char* key, std::string& hmac) {
    // Implementación de la función de generación de HMAC
}

void Encriptador::generarClaveYIV(unsigned char* key, unsigned char* iv) {
    // Implementación de la generación de clave y IV
}
