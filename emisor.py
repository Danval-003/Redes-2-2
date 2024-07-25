from typing import List

def ParityBits(index, r):
    # Verificar si el número es una potencia de 2
    if (index & (index - 1)) == 0:
        return []

    binaryIndex = [0] * r
    for i in range(r):
        binaryIndex[i] = (index >> i) & 1

    binaryIndex.reverse()
    return binaryIndex


def hamming_encode(data_bits: List[int], n: int, r: int) -> List[int]:
    # Initialize parity bits
    p = [0] * r
    
    combine = []
    ind = 0
    for i in range(n+1):
        if (i & (i - 1)) == 0:
            combine.append(0)
        else:
            print(data_bits[ind], end='')
            combine.append(data_bits[ind])
            ind += 1
    print()
    
    
    for i in range(len(combine)):
        bits = ParityBits(i, r)
        for j in range(len(bits)):
            p[j] += bits[j] * combine[i]
            p[j] %= 2
    
        
    print(p)
    
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

def fletcher8(data: str) -> str:
    sum1, sum2 = 0, 0
    for char in data:
        sum1 = (sum1 + int(char)) % 15  # Limitar a 15 (4 bits)
        sum2 = (sum2 + sum1) % 15  # Limitar a 15 (4 bits)
    checksum = (sum2 << 4) | sum1  # 4 bits para cada parte
    return f"{checksum:08b}"

def pad_message(message: str, block_size: int = 8) -> str:
    padding_length = (block_size - len(message) % block_size) % block_size
    return message + '0' * padding_length

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
        padded_message = pad_message(binary_message, 8)
        print(f'Mensaje con padding: {padded_message}')
        
        # Calcular el checksum
        fletcher_checksum = fletcher8(padded_message)
        print(f'Fletcher Checksum: {fletcher_checksum}')
        
        # Combinar mensaje y checksum
        final_message = padded_message + fletcher_checksum
        print(f'Mensaje final con checksum: {final_message}')