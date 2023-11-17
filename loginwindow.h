// loginwindow.h
#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include "server.h"

class LoginWindow : public QWidget {
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = nullptr);

signals:
    void credencialesAprobadas();

private slots:
    void iniciarSesion();

private:
    QLineEdit *usuarioInput;
    QLineEdit *contrasenaInput;
    QPushButton *botonIniciarSesion;

    // Variable miembro para almacenar la instancia de Server
    Server *serverWindow;
};

#endif // LOGINWINDOW_H
