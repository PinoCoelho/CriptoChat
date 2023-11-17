// loginwindow.cpp
#include "login.h"

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

    connect(botonIniciarSesion, &QPushButton::clicked, this, &LoginWindow::iniciarSesion);
}

void LoginWindow::iniciarSesion() {
    // Verifica el usuario y la contraseña (puedes hacerlo más seguro en una implementación real)
    if (usuarioInput->text() == "usuario" && contrasenaInput->text() == "contrasena") {
        // Si las credenciales son correctas, abre la ventana del servidor
        Server *serverWindow = new Server;
        serverWindow->show();
        close();  // Cierra la ventana de inicio de sesión
    } else {
        // Si las credenciales son incorrectas, muestra un mensaje de error
        QMessageBox::critical(this, "Error", "Credenciales incorrectas. Inténtalo de nuevo.");
    }
}
