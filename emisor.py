import numpy as np
from typing import List

def hamming_encode(data_bits: List[int], n: int, r: int) -> List[int]:
    # Initialize parity bits
    p = [0] * r
    
    # Calculate parity bits
    for i in range(r):
        parity_position = 2 ** i
        parity_value = 0
        for j in range(1, n + 1):
            if j & parity_position != 0 and (j - 1) < len(data_bits):
                parity_value ^= data_bits[j - 1]
        p[i] = parity_value
    
    # Combine data and parity bits
    encoded = []
    j = 0
    for i in range(1, n + 1):
        if i == 2 ** j:
            encoded.append(p[j])
            j += 1
        else:
            if (i - j - 1) < len(data_bits):
                encoded.append(data_bits[i - j - 1])
            else:
                encoded.append(0)  # Padding if data_bits are exhausted
    return encoded

def trans_bitstr_to_list(bitstr: str) -> List[int]:
    bits = []
    for c in bitstr:
        if c == '1':
            bits.append(1)
        elif c == '0':
            bits.append(0)
        else:
            exit(f'Invalid character {c} in message')
    return bits

def find_optimal_hamming_parameters(m: int):
    r = 1
    while (m + r + 1) > 2 ** r:
        r += 1
    n = m + r
    return n, r

# Main program
if __name__ == "__main__":
    # Solicitar un mensaje en binario
    binary_message = input("Ingrese un mensaje en binario (solo unos y ceros): ")
    if not all(c in '01' for c in binary_message):
        exit("El mensaje debe contener solo '0' y '1'.")

    # Solicitar opción de código a aplicar
    print("Seleccione el tipo de código a aplicar:")
    print("1. Hamming(n,m)")
    print("2. Fletcher Checksum")
    option = input("Ingrese 1 o 2: ")
    
    if option == "1":
        # Obtener la longitud de la entrada binaria
        m = len(binary_message)
        
        # Calcular los valores óptimos de n y r
        n, r = find_optimal_hamming_parameters(m)
        print(f'Valores óptimos: n = {n}, m = {m}, r = {r}')
        
        # Dividir el mensaje en bloques de m bits
        blocks = [binary_message[i:i+m] for i in range(0, len(binary_message), m)]
        print(f'Bloques de {m} bits: {blocks}')
        
        # Encodificar cada bloque con Hamming
        hamming_codes = []
        for block in blocks:
            data_bits = trans_bitstr_to_list(block)
            encoded_bits = hamming_encode(data_bits, n, r)
            encoded_str = ''.join(map(str, encoded_bits))
            print(f'Bloque: {block} -> Hamming({n},{m}): {encoded_str}')
            hamming_codes.append(encoded_str)
        
        # Concatenar todos los códigos Hamming en una sola línea binaria
        final_message = ''.join(hamming_codes)
        print(f'Mensaje final codificado en una línea binaria: {final_message}')
    
    elif option == "2":
        # Lógica para Fletcher Checksum (por el momento, continuar o pasar)
        pass