#include <QApplication>
#include "cliente.h"
#include "server.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Crear instancias de las ventanas del cliente y el servidor
    Server serverWindow;
    Cliente clienteWindow;

    // Mostrar las ventanas
    serverWindow.show();
    clienteWindow.show();

    // Conectar la señal enviarMensajeSignal del cliente a la función manejarMensaje del servidor
    QObject::connect(&clienteWindow, &Cliente::enviarMensajeSignal, &serverWindow, &Server::manejarMensaje);

    return a.exec();
}
