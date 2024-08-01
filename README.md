# Error-Detection-Strategies

## Laboratorio 2 del curso Redes

Este proyecto implementa un emisor en Python y un receptor en C++ para demostrar estrategias de detección y corrección de errores en la transmisión de datos.

### Algoritmos utilizados:
- **Hamming(n,m)**: Utilizado para la corrección de errores.
- **Fletcher Checksum**: Utilizado para la detección de errores.

### Compilación
Para compilar el receptor en un sistema Linux, utiliza el siguiente comando:

```sh
g++ -I./include -o Receptor Receptor.cpp ./src/* -pthread
```

### Emisor (Python)

El emisor en Python utiliza las siguientes bibliotecas y archivos:

```python
import socket
from typing import List, Tuple
import numpy as np
import random
import uuid
import json
import threading
import time
import faker
```

### Receptor (C++)

El receptor en C++ incluye las siguientes bibliotecas y archivos:

```cpp
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
```

### Estructura del Proyecto

#### Emisor (Python)
El código del emisor se encuentra en el archivo `emisor.py` y realiza las siguientes funciones:

- Verificación de bloques de 8 bits.
- Operación de Fletcher checksum.
- Generación de matriz generadora de Hamming.
- Codificación de Hamming.
- Adición de ruido al mensaje.
- Envío de mensaje al receptor.

#### Receptor (C++)
El código del receptor se encuentra en el archivo `Receptor.cpp` y realiza las siguientes funciones:

- Creación del archivo de resultados.
- Recepción de datos del socket.
- Decodificación de Hamming y Fletcher.
- Escritura de resultados en un archivo JSON.

### Uso

1. Compila el receptor con el comando mencionado anteriormente.
2. Ejecuta el receptor:
    ```sh
    ./Receptor
    ```
3. Ejecuta el emisor en Python:
    ```sh
    python emisor.py
    ```

### Ejemplo de Código del Receptor

```cpp
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

        if (message.empty()) {
            close(socket);
            return nullptr;
        }

        char type = message[0];
        message = message.substr(1);

        if (message.empty()) {
            close(socket);
            return nullptr;
        }

        if (type == '1') {
            string decodedMessage = DecoHammingMtoNToBinary(message);
            if (decodedMessage.empty()) {
                lock_guard<mutex> lock(mtx);
                Result res = {message, false, "Hamming", identifier, message};
                resultsQueue.push(res);
            } else {
                lock_guard<mutex> lock(mtx);
                string decodedMessageString = DecoBitstoString(decodedMessage);
                Result res = {decodedMessageString, true, "Hamming", identifier, message};
                resultsQueue.push(res);
            }
        } else {
            string decodedMessage = DecoFletchertoString(message);
            if (decodedMessage.empty()) {
                lock_guard<mutex> lock(mtx);
                Result res = {message, false, "Fletcher", identifier, message};
                resultsQueue.push(res);
            } else {
                lock_guard<mutex> lock(mtx);
                string decodedMessageString = DecoBitstoString(decodedMessage);
                Result res = {decodedMessageString, true, "Fletcher", identifier, message};
                resultsQueue.push(res);
            }
        }
    }
    close(socket);
    pthread_exit(nullptr);
}
```

### Contribuciones
Para contribuir a este proyecto, puedes clonar el repositorio, crear una rama, realizar tus cambios y enviar una solicitud de extracción. Las contribuciones son bienvenidas.

### Licencia
Este proyecto está bajo la Licencia MIT.
