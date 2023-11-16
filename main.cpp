#include <QApplication>
#include "cliente.h"
#include "server.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Crear las instancias de las ventanas del cliente y el servidor
    Server serverWindow;
    Cliente clienteWindow;

    // Mostrar las ventanas
    serverWindow.show();
    clienteWindow.show();

    // Conectar señales entre el cliente y el servidor si es necesario
    QObject::connect(&clienteWindow, &Cliente::enviarMensaje, &serverWindow, &Server::recibirMensaje);
    // Puedes conectar más señales según sea necesario para la comunicación entre cliente y servidor

    return a.exec();
}
