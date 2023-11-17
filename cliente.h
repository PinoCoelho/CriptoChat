// cliente.h
#ifndef CLIENTE_H
#define CLIENTE_H

#include <QWidget>
#include <QTcpSocket>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTextEdit>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <QTcpSocket>
#include <QMessageBox>



class Cliente : public QWidget {
    Q_OBJECT

public:
    explicit Cliente(QWidget *parent = nullptr);

public slots:
    void convertirTexto();
    void slotDatosRecibidos();


signals:
    void enviarMensajeSignal(const QString &mensaje);

private:
    QTextEdit *mensajeInput;
    QPushButton *enviarButton;
    QPushButton *convertirButton;
    QTcpSocket *tcpSocket;

    void setupUI();
    void handleErrors();
    void encrypt(const std::string& plaintext, const unsigned char* key, const unsigned char* iv, std::string& ciphertext);
    void enviarMensaje();

};

#endif // CLIENTE_H
