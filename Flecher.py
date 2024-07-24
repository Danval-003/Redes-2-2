from typing import List, Tuple

# Vars to colors on ascii
class colors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'
    ENDC = '\033[0m'

# Function to verify if the binary string is valid
def verify8BitsBlocks(binary: str) -> Tuple[List[Tuple[int, int, int, int, int, int, int, int]], str]:
    # Erase spaces and new lines
    binary2 = binary.replace(' ', '')
    binary2 = binary2.replace('\n', '')
    print(binary2)
    # Verify if the string is empty
    if len(binary2) == 0:
        error = colors.FAIL + 'The binary string is empty' + colors.ENDC
        raise ValueError(error)
    # Verify if only has 0 and 1
    for bit in binary2:
        if bit != '0' and bit != '1':
            error = colors.FAIL + 'The binary string must have only 0 and 1' + colors.ENDC
            raise ValueError(error)
    # Verify if the length is multiple of 8
    if len(binary2) % 8 != 0:
        error = colors.FAIL + 'The binary string must have a length multiple of 8' + colors.ENDC
        raise ValueError(error)
    
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

# Function to operate the fletcher checksum
def fletcher(blocks: List[Tuple[int, int, int, int, int, int, int, int]]) -> int:
    # Inicializar las variables
    sum1 = 0
    sum2 = 0
    
    # Iterar sobre los bloques
    for block in blocks:
        # Convertir el bloque a un entero
        number = tupleToInt(block)
        print(f'Block: {number}')
        # Sumar el bloque a sum1
        sum1 = (sum1 + number) % 255
        print(f'Sum1: {sum1}')
        # Sumar sum1 a sum2
        sum2 = (sum2 + sum1) % 255
        print(f'Sum2: {sum2}')
        print()

    # Calcular el checksum
    checksum = (sum2 << 4) | (sum1 & 0x0F)
    checksum = checksum & 0xFF
    print(f'Checksum: {checksum}')
    return checksum

# Function to convert int to binary on 8 bits
def intToBinary8Bits(number: int) -> str:
    # Convert number on only 8 bits
    number = number % 256
    print(number)
    # Convert number to binary
    binary = bin(number)[2:]
    print(binary)
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

# Fucntion to convert binary str with flecher checksum to string
def binaryWithChecksumToString(binary: str) -> str:
    blocks, binary2 = verify8BitsBlocks(binary)
    checksum = fletcher(blocks)
    checksumBinary = intToBinary8Bits(checksum)
    print(f'Checksum: {checksumBinary}')
    result = checksumBinary + binary2
    return result

def stringToBinary(string: str) -> str:
    binary = ''
    for char in string:
        # Convert char to binary, using 8 bits
        charBinary = bin(ord(char))[2:]
        if len(charBinary) < 8:
            charBinary = '0' * (8 - len(charBinary)) + charBinary
        binary += charBinary
    return binary


def main():
    message = 'Hola, mundo!'
    binary = stringToBinary(message)
    print(f'The binary is: {binary}')
    print(len(binary))
    try:
        string = binaryWithChecksumToString(binary)
        print(f'The string is: {string}')
    except ValueError as error:
        print(error)
    

if __name__ == '__main__':
    main()

