// server.h
#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

class Server : public QWidget {
    Q_OBJECT

public:
    explicit Server(QWidget *parent = nullptr);

public slots:
    void nuevaConexion(); // Declaración de la función nuevaConexion
    void recibirMensaje(); // Declaración de la función recibirMensaje
    void descifrarTexto();
    void cargarArchivoEncriptado();

private:
    QTcpServer *server;
    QTcpSocket *socket;
    QTextEdit *bandejaEntrada;
    QPushButton *descifrarButton;
    QPushButton *cargarArchivoButton;

    void setupUI();
};

#endif // SERVER_H
