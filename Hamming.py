

# Function to find the best n to a given m, where (m + r + 1) <= 2^r
def findBestN(m: int) -> int:
    r = 0
    while (m + r + 1) > 2 ** r:
        r += 1
    return r


# Funtion to decode the hamming code



if __name__ == '__main__':
    print(findBestN(11))



