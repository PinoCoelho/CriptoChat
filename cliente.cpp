// cliente.cpp
#include "cliente.h"
#include <QMessageBox>
#include <openssl/aes.h>

Cliente::Cliente(QWidget *parent) : QWidget(parent) {
    setupUI();

    // Configurar el socket del cliente
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &Cliente::slotDatosRecibidos);

    // Conectar el cliente al servidor
    tcpSocket->connectToHost("127.0.0.1", 12345); // Dirección IP y puerto del servidor
    if (!tcpSocket->waitForConnected()) {
        QMessageBox::critical(this, "Error", "No se pudo conectar al servidor.");
        close();
    }

    connect(enviarButton, &QPushButton::clicked, this, &Cliente::enviarMensaje);
}

void Cliente::setupUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);

    mensajeInput = new QTextEdit(this);
    layout->addWidget(mensajeInput);

    convertirButton = new QPushButton("Convertir", this);
    layout->addWidget(convertirButton);

    enviarButton = new QPushButton("Enviar", this);
    layout->addWidget(enviarButton);

    connect(enviarButton, &QPushButton::clicked, this, &Cliente::enviarMensaje);
    connect(convertirButton, &QPushButton::clicked, this, &Cliente::convertirTexto);
}

void Cliente::enviarMensaje() {
    // Obtener el texto ingresado en mensajeInput
    QString mensaje = mensajeInput->toPlainText();

    // Emitir la señal para enviar el mensaje al servidor
    emit enviarMensajeSignal(mensaje);
}

void Cliente::slotDatosRecibidos() {
    // Manejar los datos recibidos del servidor
    QByteArray datos = tcpSocket->readAll();
    // Puedes procesar los datos según tus necesidades
}

void Cliente::handleErrors() {
    ERR_print_errors_fp(stderr);
    abort();
}

void Cliente::encrypt(const std::string& plaintext, const unsigned char* key, const unsigned char* iv, std::string& ciphertext) {
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

void Cliente::convertirTexto() {
    // Obtener el texto ingresado en mensajeInput
    QString textoOriginal = mensajeInput->toPlainText();

    // Clave y vector de inicialización para AES (deberías cambiar estos valores según tus necesidades)
    const unsigned char key[] = "0123456789abcdef";
    const unsigned char iv[] = "0123456789abcdef";

    // Convertir QString a std::string
    std::string textoOriginalStd = textoOriginal.toStdString();

    // Encriptar el texto
    std::string textoEncriptado;
    encrypt(textoOriginalStd, key, iv, textoEncriptado);

    // Mostrar el texto encriptado en mensajeInput
    mensajeInput->setText(QString::fromStdString(textoEncriptado));
}
