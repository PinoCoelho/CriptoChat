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

    connect(descifrarButton, &QPushButton::clicked, this, &Server::descifrarTexto);
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
        std::ifstream file(filePath.toStdString());
        std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        // Aquí se podría enviar el archivo al cliente para su desencriptación y muestra del texto en el servidor
        // Sin embargo, debes ajustar la comunicación entre el cliente y el servidor para esto.
    }
}
