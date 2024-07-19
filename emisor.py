import numpy as np
from typing import List

def hamming_encode(data_bits: List[int]) -> List[int]:
    # Data bits
    d = data_bits
    
    # Initialize parity bits
    p = [0, 0, 0, 0]
    
    # Calculate parity bits
    p[0] = d[0] ^ d[1] ^ d[3] ^ d[4] ^ d[6]
    p[1] = d[0] ^ d[2] ^ d[3] ^ d[5] ^ d[6]
    p[2] = d[1] ^ d[2] ^ d[3]
    p[3] = d[4] ^ d[5] ^ d[6]
    
    # Combine data and parity bits
    encoded = [p[0], p[1], d[0], p[2], d[1], d[2], d[3], p[3], d[4], d[5], d[6]]
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

def char_to_7bit_ascii(c: str) -> str:
    ascii_code = f'{ord(c):07b}'
    return ascii_code

# Main program
if __name__ == "__main__":
    # Solicitar un mensaje en mayúsculas
    message = input("Ingrese un mensaje en mayúsculas: ")
    if not message.isupper():
        exit("El mensaje debe estar en mayúsculas.")
    
    # Convertir cada carácter a su código ASCII de 7 bits y verificar longitud
    ascii_codes = []
    for c in message:
        ascii_code = char_to_7bit_ascii(c)
        if len(ascii_code) != 7:
            exit(f"Error: Código ASCII de {c} no tiene 7 bits.")
        print(f'Carácter: {c} -> ASCII: {ascii_code}')
        ascii_codes.append(ascii_code)
    
    # Encodificar cada código ASCII con Hamming
    hamming_codes = []
    for ascii_code in ascii_codes:
        data_bits = trans_bitstr_to_list(ascii_code)
        encoded_bits = hamming_encode(data_bits)
        encoded_str = ''.join(map(str, encoded_bits))
        print(f'ASCII: {ascii_code} -> Hamming(11,7): {encoded_str}')
        hamming_codes.append(encoded_str)
    
    # Concatenar todos los códigos Hamming en una sola línea binaria
    final_message = ''.join(hamming_codes)
    print(f'Mensaje final codificado en una línea binaria: {final_message}')
