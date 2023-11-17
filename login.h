// loginwindow.h
#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include "server.h"  // Incluye el archivo de encabezado del servidor

class Login : public QWidget {
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = nullptr);

private slots:
    void iniciarSesion();

private:
    QLineEdit *usuarioInput;
    QLineEdit *contrasenaInput;
    QPushButton *botonIniciarSesion;
};

#endif // LOGINWINDOW_H
