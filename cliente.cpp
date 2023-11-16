#include "cliente.h"
#include "encriptar.h"
#include <QMessageBox>
#include <QFileDialog>
#include <iostream>
#include <fstream>

Cliente::Cliente(QWidget *parent) : QWidget(parent) {
    setupUI();
    socket = new QTcpSocket(this);

    connect(enviarButton, &QPushButton::clicked, this, &Cliente::enviarMensaje);
    connect(convertirButton, &QPushButton::clicked, this, &Cliente::convertirTextoAArchivo);
}

void Cliente::setupUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);

    mensajeInput = new QTextEdit(this);
    layout->addWidget(mensajeInput);

    convertirButton = new QPushButton("Convertir", this);
    layout->addWidget(convertirButton);

    enviarButton = new QPushButton("Enviar", this);
    layout->addWidget(enviarButton);
}

void Cliente::convertirTextoAArchivo() {
    QString textToConvert = mensajeInput->toPlainText();
    QString fileName = QFileDialog::getSaveFileName(this, tr("Guardar Archivo"), "", tr("Archivos de texto (*.txt)"));

    if (!textToConvert.isEmpty() && !fileName.isEmpty()) {
        std::string filename = fileName.toStdString();
        std::ofstream outfile(filename);
        outfile << textToConvert.toStdString();
        outfile.close();

        // Leer el contenido del archivo .txt y encriptarlo
        std::ifstream infile(filename);
        std::string fileContent((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
        infile.close();

        unsigned char key[16];
        unsigned char iv[AES_BLOCK_SIZE];
        Encriptador::generarClaveYIV(key, iv);

        std::string encryptedFileContent;
        Encriptador::encrypt(fileContent, key, iv, encryptedFileContent);

        std::ofstream encryptedFile(filename + "_encrypted.txt");
        encryptedFile << encryptedFileContent;
        encryptedFile.close();

        QMessageBox::information(this, "Archivo Convertido y Encriptado", "El archivo se convirtió en un archivo .txt y está encriptado.");

        // Enviar la clave y el IV al servidor
        enviarClaveYIVAlServidor(key, iv);

        // Aquí podrías enviar el archivo al servidor para su desencriptación y mostrar el texto en el servidor
    } else {
        QMessageBox::warning(this, "Advertencia", "El texto está vacío o el nombre de archivo no es válido.");
    }
}

void Cliente::enviarMensaje() {
    try {
        if (!mensajeInput->toPlainText().isEmpty()) {
            if (!socket->isOpen() || socket->state() != QAbstractSocket::ConnectedState) {
                socket->connectToHost("localhost", 12345);
                if (!socket->waitForConnected(1000)) {
                    throw std::runtime_error("No se pudo conectar al servidor.");
                }
            }

            std::string messageText = mensajeInput->toPlainText().toStdString();
            unsigned char key[16];
            unsigned char iv[AES_BLOCK_SIZE];
            Encriptador::generarClaveYIV(key, iv);

            std::string encryptedMessage;
            Encriptador::encrypt(messageText, key, iv, encryptedMessage);

            // Enviar la clave y el IV al servidor
            enviarClaveYIVAlServidor(key, iv);

            // Verificar si el socket está abierto y conectado antes de escribir
            if (socket->isOpen() && socket->state() == QAbstractSocket::ConnectedState) {
                socket->write(QString::fromStdString(encryptedMessage).toUtf8());
                mensajeInput->clear();
            } else {
                QMessageBox::warning(this, "Advertencia", "No se pudo enviar el mensaje, el socket no está abierto o conectado.");
            }
        }
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

void Cliente::enviarClaveYIVAlServidor(const unsigned char* key, const unsigned char* iv) {
    // Enviar la clave y el IV al servidor
    QByteArray keyIVData(reinterpret_cast<const char*>(key), 16);
    QByteArray ivData(reinterpret_cast<const char*>(iv), AES_BLOCK_SIZE);

    QByteArray totalData;
    totalData.append(keyIVData);
    totalData.append(ivData);

    socket->write(totalData);
}
