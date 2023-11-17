// loginwindow.cpp
#include "loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    usuarioInput = new QLineEdit(this);
    usuarioInput->setPlaceholderText("Usuario");
    layout->addWidget(usuarioInput);

    contrasenaInput = new QLineEdit(this);
    contrasenaInput->setPlaceholderText("Contraseña");
        contrasenaInput->setEchoMode(QLineEdit::Password);
    layout->addWidget(contrasenaInput);

    botonIniciarSesion = new QPushButton("Iniciar Sesión", this);
                         layout->addWidget(botonIniciarSesion);

    // Declarar la instancia de Server aquí
    serverWindow = new Server;
    serverWindow->setEnabled(false);  // Deshabilitar la ventana al principio

    connect(botonIniciarSesion, &QPushButton::clicked, this, &LoginWindow::iniciarSesion);
}

void LoginWindow::iniciarSesion() {
    // Verifica el usuario y la contraseña (puedes hacerlo más seguro en una implementación real)
    if (usuarioInput->text() == "usuario" && contrasenaInput->text() == "contrasena") {
        // Muestra la ventana del servidor solo si las credenciales son correctas
        emit credencialesAprobadas();    // Emite la señal
        serverWindow->show();
        close();                         // Cierra la ventana de inicio de sesión
    } else {
        // Si las credenciales son incorrectas, muestra un mensaje de error
        QMessageBox::critical(this, "Error", "Credenciales incorrectas. Inténtalo de nuevo.");
    }
}
