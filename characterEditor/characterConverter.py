import json
import sys


if len(sys.argv) != 2:
    print("Usage: characterConverter.py <input file>")
    sys.exit(1)

file = open(sys.argv[1], "r")

fileRawData = file.readline()
print(fileRawData)

data = json.loads(fileRawData)

print(data)


output = [0] * 0x8000;

outputInd = 0


for character in data:
    offset = 0
    row = data[character]
    print(character)
    print(row)

    for idx, col in enumerate(row):
        output[outputInd + offset] = int(col, 2)
        offset += 256
    outputInd += 1

print(output)


with open("outFile.bin", "wb") as outputFile:
    outputFile.write(bytes(output))
    outputFile.close()