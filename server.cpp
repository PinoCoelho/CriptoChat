#include "server.h"
#include "encriptar.h"
#include <QMessageBox>
#include <iostream>
#include <QFileDialog>
#include <fstream>

Server::Server(QWidget *parent) : QWidget(parent) {
    setupUI();
    server = new QTcpServer(this);

    if (!server->listen(QHostAddress::Any, 12345)) {
        QMessageBox::critical(this, "Error", "No se pudo iniciar el servidor.");
    } else {
        connect(server, &QTcpServer::newConnection, this, &Server::nuevaConexion);
    }
}

void Server::setupUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);

    bandejaEntrada = new QTextEdit(this);
    layout->addWidget(bandejaEntrada);

    descifrarButton = new QPushButton("Descifrar", this);
    layout->addWidget(descifrarButton);

    cargarArchivoButton = new QPushButton("Cargar Archivo Encriptado", this);
    layout->addWidget(cargarArchivoButton);

    connect(descifrarButton, &QPushButton::clicked, this, &Server::descifrarTexto);
    connect(cargarArchivoButton, &QPushButton::clicked, this, &Server::cargarArchivoEncriptado);
}

void Server::nuevaConexion() {
    socket = server->nextPendingConnection();
    connect(socket, &QTcpSocket::readyRead, this, &Server::recibirMensaje);
}

void Server::recibirMensaje() {
    QByteArray data = socket->readAll();

    if (data.size() >= 16 + AES_BLOCK_SIZE) {
        // Recibimos la clave y el IV del cliente
        unsigned char key[16];
        unsigned char iv[AES_BLOCK_SIZE];
        memcpy(key, data.constData(), 16);
        memcpy(iv, data.constData() + 16, AES_BLOCK_SIZE);

        // Realizamos la desencriptación si el tamaño de datos recibidos es mayor que el tamaño de clave y IV
        if (data.size() > 16 + AES_BLOCK_SIZE) {
            std::string encryptedText = data.mid(16 + AES_BLOCK_SIZE).toStdString();
            std::string decryptedText;

            // Realizar la desencriptación con el Encriptador y mostrar el texto desencriptado
            Encriptador encriptador;
            encriptador.decrypt(encryptedText, key, iv, decryptedText);

            // Muestra el texto desencriptado en la bandeja de entrada o donde desees
            bandejaEntrada->append(QString::fromStdString(decryptedText));
        }
    }
}

void Server::descifrarTexto() {
    QString filePath = QFileDialog::getOpenFileName(this, tr("Seleccionar archivo encriptado"), "", tr("Archivos de texto encriptados (*.txt)"));
    if (!filePath.isEmpty()) {
        std::ifstream encryptedFile(filePath.toStdString());
        std::string encryptedContent((std::istreambuf_iterator<char>(encryptedFile)), std::istreambuf_iterator<char>());
        encryptedFile.close();

        // Realizar la desencriptación y mostrar el texto descifrado
        unsigned char key[16];
        unsigned char iv[AES_BLOCK_SIZE];
        Encriptador::generarClaveYIV(key, iv);

        std::string decryptedText;
        Encriptador::decrypt(encryptedContent, key, iv, decryptedText);

        // Muestra el texto desencriptado en la bandeja de entrada o donde desees
        bandejaEntrada->setText(QString::fromStdString(decryptedText));
    }
}

void Server::cargarArchivoEncriptado() {
    QString filePath = QFileDialog::getOpenFileName(this, tr("Seleccionar archivo encriptado"), "", tr("Archivos de texto encriptados (*.txt)"));
    if (!filePath.isEmpty()) {
        std::ifstream encryptedFile(filePath.toStdString());
        std::string encryptedContent((std::istreambuf_iterator<char>(encryptedFile)), std::istreambuf_iterator<char>());
        encryptedFile.close();
        qDebug()<<"EncryptedContent "<<encryptedContent;
        // Realizar la desencriptación y mostrar el texto descifrado
        unsigned char key[16];
        unsigned char iv[AES_BLOCK_SIZE];
        Encriptador::generarClaveYIV(key, iv);

        std::string decryptedText;
        qDebug("cualquier cosa");
        Encriptador::decrypt(encryptedContent, key, iv, decryptedText);
        qDebug()<<"DecryptedText " <<decryptedText;
        // Muestra el texto desencriptado en la bandeja de entrada o donde desees
        bandejaEntrada->setText(QString::fromStdString(decryptedText));
    }
}

