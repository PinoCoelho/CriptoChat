// server.h
#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTextEdit>
#include "cliente.h"

class Server : public QWidget {
    Q_OBJECT

public:
    explicit Server(QWidget *parent = nullptr);

public slots:
    void recibirMensaje();
    void descifrarTexto(bool checked);
    void nuevaConexion();
    void cargarArchivoEncriptado();
    void manejarMensaje(const QString &mensaje);
    void decrypt(const std::string& ciphertext, const unsigned char* key, const unsigned char* iv, std::string& decryptedText);

private:
    QTextEdit *bandejaEntrada;
    QPushButton *descifrarButton;
    QPushButton *cargarArchivoButton;
    QTcpServer *tcpServer;
    QTcpSocket *clienteSocket;

    void setupUI();
};

#endif // SERVER_H
