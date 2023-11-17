// server.cpp
#include "server.h"
#include "cliente.h"  // Asegúrate de incluir el archivo de encabezado de Cliente
#include <QMessageBox>
#include <QFileDialog>
#include <openssl/rand.h>
#include <openssl/aes.h>
#include <iostream>

Server::Server(QWidget *parent) : QWidget(parent) {
    setupUI();
}

void Server::setupUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);

    bandejaEntrada = new QTextEdit(this);
    bandejaEntrada->setReadOnly(true);
    layout->addWidget(bandejaEntrada);

    descifrarButton = new QPushButton("Descifrar", this);
    layout->addWidget(descifrarButton);

    cargarArchivoButton = new QPushButton("Cargar Archivo Encriptado", this);
    layout->addWidget(cargarArchivoButton);

    connect(descifrarButton, &QPushButton::clicked, this, &Server::descifrarTexto);
    connect(cargarArchivoButton, &QPushButton::clicked, this, &Server::cargarArchivoEncriptado);

    // Configurar el servidor y conectar la señal de nueva conexión
    tcpServer = new QTcpServer(this);
    connect(tcpServer, &QTcpServer::newConnection, this, &Server::nuevaConexion);
    if (!tcpServer->listen(QHostAddress::Any, 12345)) {
        QMessageBox::critical(this, "Error", "El servidor no pudo iniciar.");
        close();
    }
}

void Server::recibirMensaje() {
    // Implementación para recibir mensajes del cliente y mostrarlos en bandejaEntrada
    QByteArray datos = clienteSocket->readAll();

}

void Server::descifrarTexto(bool checked) {
    // Obtener el texto encriptado de la bandeja de entrada
    QString textoEncriptado = bandejaEntrada->toPlainText();
    std::string textoEncriptadoStd = textoEncriptado.toStdString();

    // Clave y vector de inicialización para AES (deberías cambiar estos valores según tus necesidades)
    const unsigned char key[] = "0123456789abcdef";
    const unsigned char iv[] = "0123456789abcdef";

    // Texto descifrado (simulación simple)
    std::string textoDescifrado;
    decrypt(textoEncriptadoStd, key, iv, textoDescifrado);

    // Mostrar el texto descifrado en la bandeja de entrada
    bandejaEntrada->append("Texto Descifrado:\n" + QString::fromStdString(textoDescifrado));
}

void Server::decrypt(const std::string& ciphertext, const unsigned char* key, const unsigned char* iv, std::string& decryptedText) {
    // Simulación simple de desencriptación
    decryptedText = "Mensaje desencriptado: " + ciphertext + "\nClave: " + reinterpret_cast<const char*>(key) + "\nIV: " + reinterpret_cast<const char*>(iv);
}

void Server::cargarArchivoEncriptado() {
    // Implementación para cargar un archivo encriptado del cliente y mostrarlo en bandejaEntrada
}

void Server::nuevaConexion() {
    // Cuando hay una nueva conexión de un cliente
    clienteSocket = tcpServer->nextPendingConnection();
    connect(clienteSocket, &QTcpSocket::readyRead, this, &Server::recibirMensaje);
    // Puedes conectar más señales y ranuras según tus necesidades
}

void Server::manejarMensaje(const QString &mensaje) {
    // Manejar el mensaje recibido, por ejemplo, mostrarlo en bandejaEntrada
    bandejaEntrada->append(mensaje);
}
