from typing import List, Tuple

class colors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'
    ENDC = '\033[0m'

# Function to verify if the binary string is valid and convert to blocks of 8 bits
def verify8BitsBlocks(binary: str) -> Tuple[List[Tuple[int, int, int, int, int, int, int, int]], str]:
    binary2 = binary.replace(' ', '').replace('\n', '')
    blocks = []
    for i in range(0, len(binary2), 8):
        block = binary2[i:i+8]
        blocks.append((
            int(block[0]),
            int(block[1]),
            int(block[2]),
            int(block[3]),
            int(block[4]),
            int(block[5]),
            int(block[6]),
            int(block[7])
        ))
    return blocks, binary2

# Function to convert Tuple to int
def tupleToInt(block: Tuple[int, int, int, int, int, int, int, int]) -> int:
    return int(''.join([str(bit) for bit in block]), 2)

# Function to operate the Fletcher checksum
def fletcher(blocks: List[Tuple[int, int, int, int, int, int, int, int]]) -> int:
    sum1, sum2 = 0, 0
    for block in blocks:
        number = tupleToInt(block)
        sum1 = (sum1 + number) % 255
        sum2 = (sum2 + sum1) % 255
    checksum = (sum2 << 4) | (sum1 & 0x0F)
    checksum = checksum & 0xFF
    return checksum

# Function to convert int to binary on 8 bits
def intToBinary8Bits(number: int) -> str:
    number = number % 256
    binary = bin(number)[2:]
    if len(binary) < 8:
        binary = '0' * (8 - len(binary)) + binary
    return binary

# Function to convert binary to string
def binaryToString(binary: str) -> str:
    string = ''
    for i in range(0, len(binary), 8):
        block = binary[i:i+8]
        string += chr(int(block, 2))
    return string

# Function to calculate parity bits
def ParityBits(index: int, r: int) -> List[int]:
    if (index & (index - 1)) == 0:
        return []
    binaryIndex = [0] * r
    for i in range(r):
        binaryIndex[i] = (index >> i) & 1
    binaryIndex.reverse()
    return binaryIndex

# Function to encode data with Hamming
def hamming_encode(data_bits: List[int], n: int, r: int) -> List[int]:
    p = [0] * r
    combine = []
    ind = 0
    for i in range(n+1):
        if (i & (i - 1)) == 0:
            combine.append(0)
        else:
            combine.append(data_bits[ind])
            ind += 1
    
    for i in range(len(combine)):
        bits = ParityBits(i, r)
        for j in range(len(bits)):
            p[j] += bits[j] * combine[i]
            p[j] %= 2

    # Print parity bits
    print(f'Bits de paridad: {p}')
    
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
                encoded.append(0)
    return encoded

def trans_bitstr_to_list(bitstr: str) -> List[int]:
    return [int(c) for c in bitstr if c in '01']

def find_optimal_hamming_parameters(m: int):
    r = 1
    while (m + r + 1) > 2 ** r:
        r += 1
    n = m + r
    return n, r

def pad_message(message: str, block_size: int = 8) -> str:
    padding_length = (block_size - len(message) % block_size) % block_size
    return message + '0' * padding_length

def stringToBinary(string: str) -> str:
    binary = ''
    for char in string:
        charBinary = bin(ord(char))[2:]
        if len(charBinary) < 8:
            charBinary = '0' * (8 - len(charBinary)) + charBinary
        binary += charBinary
    return binary

def main():
    message = input("Ingrese un mensaje en binario (solo unos y ceros): ")
    if not all(c in '01' for c in message):
        exit("El mensaje debe contener solo '0' y '1'.")

    print("Seleccione el tipo de código a aplicar:")
    print("1. Hamming(n,m)")
    print("2. Fletcher Checksum")
    option = input("Ingrese 1 o 2: ")
    
    if option == "1":
        m = len(message)
        n, r = find_optimal_hamming_parameters(m)
        print(f'Valores óptimos: n = {n}, m = {m}, r = {r}')
        
        blocks = [message[i:i+m] for i in range(0, len(message), m)]
        print(f'Bloques de {m} bits: {colors.BOLD+colors.OKBLUE} {blocks} {colors.ENDC}')
        
        hamming_codes = []
        for block in blocks:
            data_bits = trans_bitstr_to_list(block)
            encoded_bits = hamming_encode(data_bits, n, r)
            encoded_str = ''.join(map(str, encoded_bits))
            print(f'Bloque: {block} -> Hamming({n},{m}): {colors.BOLD+colors.OKBLUE} {encoded_str} {colors.ENDC}')
            hamming_codes.append(encoded_str)
        
        final_message = ''.join(hamming_codes)
        print(f'Mensaje final codificado en una línea binaria: {colors.BOLD+colors.OKGREEN} {final_message} {colors.ENDC}')
    
    elif option == "2":
        padded_message = pad_message(message, 8)
        print(f'Mensaje con padding: {colors.BOLD+colors.OKBLUE}{padded_message} {colors.ENDC}')
        
        blocks, _ = verify8BitsBlocks(padded_message)
        fletcher_checksum = fletcher(blocks)
        print(f'Bloques de 8 bits: {blocks}')
        print(f'Fletcher Checksum: {colors.BOLD+colors.OKBLUE}{intToBinary8Bits(fletcher_checksum)} {colors.ENDC}')
        
        final_message = intToBinary8Bits(fletcher_checksum) + padded_message
        print(f'Mensaje final con checksum:{colors.BOLD+colors.OKGREEN} {final_message} {colors.ENDC}')

if __name__ == '__main__':
    main()
