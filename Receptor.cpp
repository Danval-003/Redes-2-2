#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <pthread.h>
#include <mutex>
#include <locale>
#include <codecvt>
#include <queue>
#include <condition_variable>
#include <fstream>
#include "include/Colors.h"
#include "include/Hamming.h"
#include "include/Fletcher.h"
#include "include/nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;

const std::string fileResults = "results_receptor.json";
std::mutex mtx;


struct Result {
    string message;
    bool success;
    string method;
    string identifier;
    string originalMessage;
};

queue<Result> resultsQueue = queue<Result>();


void createResultsFile() {
    json j;
    ofstream file(fileResults);
    file << j.dump() << endl;
    file.close();
}


std::string cleanUTF8(const std::string& str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
    std::wstring wide_str;
    try {
        wide_str = convert.from_bytes(str);
    } catch (...) {
        // Handle conversion errors if necessary
        return "";
    }
    return convert.to_bytes(wide_str);
}


void* writeRes(void *arg) {
    while (true)
    {
        if (resultsQueue.empty()) {
            continue;
        }
        ifstream file(fileResults);
        json j;
        file >> j;
        file.close();
        mtx.lock();
        while (!resultsQueue.empty()) {
            Result res = resultsQueue.front();
            json r;
            r["message"] = cleanUTF8(res.message);
            r["success"] = res.success;
            r["method"] = res.method;
            r["identifier"] = res.identifier;
            r["originalMessage"] = res.originalMessage;
            resultsQueue.pop();
            j.push_back(r);
        }
        mtx.unlock();
        ofstream file2(fileResults);
        file2 << j.dump() << endl;
        file2.close();
    }
}

char DecoBitstoChar(const string& binary) {
    int value = 0;
    for (int i = 0; i < 8; i++) {
        value += (binary[i] - '0') << (7 - i);
    }
    return (char)value;
}

string DecoBitstoString(const string& binary) {
    // Verify that the binary message is a multiple of 8
    if (binary.size() % 8 != 0) {
        //cout << redShell << "El mensaje binario no es múltiplo de 8" << resetShell << endl;
        return "";
    }

    string message = "";
    for (size_t i = 0; i < binary.size(); i += 8) {
        message += DecoBitstoChar(binary.substr(i, 8));
    }
    return message;
}

void* receiveData(void* socket_ptr) {
    int socket = *((int*)socket_ptr);
    delete (int*)socket_ptr;

    char buffer[1024] = {0};
    ssize_t valread = read(socket, buffer, sizeof(buffer) - 1);

    if (valread > 0) {
        buffer[valread] = '\0';
        string message = buffer;
        string identifier = "";

        size_t pos = message.find("|");
        if (pos == string::npos) {
            //cout << redShell << "Mensaje vacío" << resetShell << endl;
            close(socket);
            return nullptr;
        } else {
            identifier = message.substr(0, pos);
            if (pos + 1 < message.size()) {
                message = message.substr(pos + 1);
            } else {
                message = "";
            }
        }

        //cout << blue << "Mensaje recibido: " << message << resetShell << endl;
        //cout << yellow << "Identificador: " << identifier << resetShell << endl;

        if (message.empty()) {
            //cout << redShell << "Mensaje vacío" << resetShell << endl;
            close(socket);
            return nullptr;
        }

        char type = message[0];
        message = message.substr(1);

        if (message.empty()) {
            //cout << redShell << "Mensaje vacío" << resetShell << endl;
            close(socket);
            return nullptr;
        }

        if (type == '1') {
            string decodedMessage = DecoHammingMtoNToBinary(message);
            if (decodedMessage.empty()) {
                // using lock guard
                lock_guard<mutex> lock(mtx);
                //cout << redShell << "Error al decodificar el mensaje" << resetShell << endl;
                Result res = {message, false, "Hamming", identifier, message};
                resultsQueue.push(res);

            } else {
                lock_guard<mutex> lock(mtx);
                //cout << greenShell << "Mensaje recibido usando Hamming: " << decodedMessage << resetShell << endl;
                string decodedMessageString = DecoBitstoString(decodedMessage);
                //cout << greenShell << "Mensaje decodificado: " << decodedMessageString << resetShell << endl;
                Result res = {decodedMessageString, true, "Hamming", identifier, message};
                resultsQueue.push(res);
            }
        } else {
            string decodedMessage = DecoFletchertoString(message);
            if (decodedMessage.empty()) {
                lock_guard<mutex> lock(mtx);
                //cout << redShell << "Error al decodificar el mensaje" << resetShell << endl;
                Result res = {message, false, "Fletcher", identifier, message};
                resultsQueue.push(res);
            } else {
                lock_guard<mutex> lock(mtx);
                //cout << greenShell << "Mensaje recibido usando Fletcher: " << decodedMessage << resetShell << endl;
                string decodedMessageString = DecoBitstoString(decodedMessage);
                Result res = {decodedMessageString, true, "Fletcher", identifier, message};
                resultsQueue.push(res);
            }
        }
    }
    close(socket);
    pthread_exit(nullptr);
}

int main() {
    int server_fd;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    createResultsFile();

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Error al crear el socket");
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(9000);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Error al vincular el socket");
        close(server_fd);
        return 1;
    }

    if (listen(server_fd, SOMAXCONN) < 0) {
        perror("Error al escuchar");
        close(server_fd);
        return 1;
    }

    //cout << "Servidor en espera de conexiones..." << endl;
    pthread_t writeResThread;
    if (pthread_create(&writeResThread, nullptr, writeRes, nullptr) != 0) {
        perror("Error al crear el hilo");
    }

    while (true) {
        int new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
        if (new_socket < 0) {
            perror("Error al aceptar la conexión");
            continue;
        }

        pthread_t thread_id;
        int* new_sock = new int(new_socket);
        if (pthread_create(&thread_id, nullptr, receiveData, new_sock) != 0) {
            perror("Error al crear el hilo");
            delete new_sock;
            continue;
        }
        pthread_detach(thread_id);
    }

    pthread_join(writeResThread, nullptr);


    close(server_fd);
    return 0;
}
