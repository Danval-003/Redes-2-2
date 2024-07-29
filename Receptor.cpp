#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include "Include/Colors.h"
#include "Include/Hamming.h"
#include "Include/Fletcher.h"

using namespace std;

void* receiveData(void* arg) {
    int* new_socket = (int*)arg;

    while (true) {
        // Recibir datos
        char buffer[1024] = {0};
        ssize_t valread = read(*new_socket, buffer, sizeof(buffer) - 1);
        if (valread > 0) {
            string binary = buffer;
            string hamming = DecoHammingMtoNToBinary(binary);
            string fletcher = DecoFletchertoString(hamming);
            if (hamming != "") {
                cout << green << "Mensaje recibido y cifrado con hamming: " << reset << hamming << endl;
            } else {
                if (fletcher != "") {
                    cout << green << "Mensaje recibido y cifrado con flecher: " << reset << fletcher << endl;
                } else {
                    cout << red << "Error en la transmisión" << reset << endl;
                }
            }
        }
    }

    close(*new_socket);
    return NULL;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    // Crear un socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Error al crear el socket");
        return 1;
    }

    // Definir la dirección del servidor
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(9000);

    // Vincular el socket
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Error al vincular el socket");
        close(server_fd);
        return 1;
    }

    // Escuchar conexiones entrantes
    if (listen(server_fd, SOMAXCONN) < 0) {
        perror("Error al escuchar");
        close(server_fd);
        return 1;
    }

    // Aceptar una conexión entrante
    new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
    if (new_socket < 0) {
        perror("Error al aceptar la conexión");
        close(server_fd);
        return 1;
    }

    // Recibir datos
    char buffer[1024] = {0};
    ssize_t valread = read(new_socket, buffer, sizeof(buffer) - 1);
    if (valread > 0) {
        cout << "Recibido: " << buffer << endl;
    }

    // Cerrar los sockets
    close(new_socket);
    close(server_fd);

    return 0;
}
