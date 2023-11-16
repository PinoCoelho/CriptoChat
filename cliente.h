#ifndef CLIENTE_H
#define CLIENTE_H

#include <QWidget>
#include <QTcpSocket>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

class Cliente : public QWidget {
    Q_OBJECT

public:
    explicit Cliente(QWidget *parent = nullptr);
    void enviarMensaje(); // Mover la declaración a la sección public
    void convertirTextoAArchivo();
    void enviarClaveYIVAlServidor(const unsigned char* key, const unsigned char* iv);
    void mostrarTextoEncriptado(const QString& filePath);
    //std::string encryptAES(const std::string& plaintext, const std::string& key);

signals:
    void enviarMensajeSignal(); // Cambiar el nombre de la señal para evitar conflicto

private:
    QTcpSocket *socket;
    QTextEdit *mensajeInput;
    QPushButton *enviarButton;
    QPushButton *convertirButton;

    void setupUI();
};

#endif // CLIENTE_H
