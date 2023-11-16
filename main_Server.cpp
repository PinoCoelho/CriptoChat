#include "mainwindow_Server.h"
#include <QApplication>
#include <iostream>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
    // Texto original
    std::string originalText = "Pino trafica gomitas en el edificio F2 y esta armado";

    // Generar una clave aleatoria de 16 bytes (128 bits)
    unsigned char key[16];
    if (RAND_bytes(key, sizeof(key)) != 1)
        handleErrors();

    // Generar un IV (vector de inicialización) aleatorio de 16 bytes
    unsigned char iv[AES_BLOCK_SIZE];
    if (RAND_bytes(iv, sizeof(iv)) != 1)
        handleErrors();

    // Cifrar el texto
    std::string ciphertext;
    encrypt(originalText, key, iv, ciphertext);

    // Guardar el texto cifrado en un archivo
    FILE *fp = fopen("encrypted.txt", "wb");
    fwrite(iv, 1, sizeof(iv), fp);
    fwrite(ciphertext.c_str(), 1, ciphertext.size(), fp);
    fclose(fp);

    // Descifrar el texto
    std::string decryptedText;
    decrypt(ciphertext, key, iv, decryptedText);

    // Mostrar el resultado
    std::cout << "Texto original: " << originalText << std::endl;
    std::cout << "Texto cifrado: " << ciphertext << std::endl;
    std::cout << "Texto descifrado: " << decryptedText << std::endl;

    return 0;
}
