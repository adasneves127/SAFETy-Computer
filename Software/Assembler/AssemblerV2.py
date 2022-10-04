import assemblyDef   #This is the file that contains the assembly instructions
from sys import argv #This is the library that allows us to get the command line arguments
import re
from color import printColor

class Queue:
    
    def __init__(self):
        self.size = 0
        self.items = []

    def isEmpty(self):
        return self.items == []

    def enqueue(self, item):
        self.items.insert(0, item)
        self.size = len(self.items)
        return

    def dequeue(self):
        self.size = len(self.items)
        return self.items.pop()

    

#This is the assembler for the SAFE assembly language
#It takes in a file, and outputs a file with the machine code.
#The output files are named with the SAF.ml.bin extension.
#This also puts out a file to be used with the simulator, .SAF.ml.txt, and a debugging "List" file, .SAF.ml.lst.

#Step 1: Read in the file, and store it into a list of lines.
#Step 2: Create an intermediate array, which contains where instructions will land, and what they are.
#Step 3: Create a dict of labels, and their locations.
#Step 4: Parse the lines, and store the instructions into the final array, replacing all labels with their locations.
#Step 5: Output the final array to the files chosen by the user.

argc = len(argv)
debug = True

outputFile = ""
noListing = False
noSimulator = False
imputFile = ""
lines = []
labelQueue = Queue()

REGS = { 
    "A": 0, 
    "B": 1, 
    "X": 2, 
    "Y": 3 }


print(argv)
if argc < 2 and not debug:
    printColor("Err: ", "red", end="")
    printColor("python3 Assembler.py <filename>", "yellow")
    exit()
elif "-h" in argv:
    print("""SAFE Assembler
    Usage: python3 Assembler.py [-o] [-nL] [-nS] <filename>
    -o: Output to a file.
    -nL: No listing output.
    -nS: No simulator output.""")
    exit()

if "-o" in argv:
    outputFile = argv[argv.index("-o") + 1][0:len(argv[argv.index("-o") + 1]) - 4] + "SAF.ml."

if "-nL" in argv:
    print("No listing output.")
    noListing = True

if "-nS" in argv:
    print("No simulator output.")
    noSimulator = True

inputFile = argv[-1]


#Step 1: Read in the file, and store it into a list of lines.
print("Reading in file...")
with open(inputFile, "r") as file:
    lines = file.readlines()
    file.close()

#Step 2: Create an intermediate array, which contains where instructions will land, and what they are.
print("Creating intermediate array...")
intermediateArray = [""] * 0x8 #This is the maximum size possible for a program in ROM.
intIndex = 0
labelDict = {}
for line in lines:
    currentLine = line.strip();
    if currentLine == "" or currentLine[0] == ";":
        continue #This is a comment or a blank line, so ignore it.
    if line.split(" ")[0] in assemblyDef.macros:
        #This is a macro. Process it.
        intIndex += len(assemblyDef.macros[line.split(" ")[0]])

    
    ins = re.match(r'ld(a|b|x|y)|st(a|b|x|y)', currentLine)
    if ":" in currentLine:
        labelDict[currentLine[0::currentLine.index(":")]] = intIndex
        continue
    
    if ins is None:
        intermediateArray[intIndex] = currentLine
        intIndex += assemblyDef.opCodes[currentLine][1]
    else:
        print(ins)
        print(ins.group(0))
        if "$" in currentLine:
            #This is a memory line
            intermediateArray[intIndex] = assemblyDef.opCodes[str(ins.group(0)) + " $"]
            intIndex += assemblyDef.opCodes[ins.group(0) + " $"][1]
        else:
            #This is a register line
            intermediateArray[intIndex] = assemblyDef.opCodes[str(ins.group(0)) + " #"]
            intIndex += assemblyDef.opCodes[ins.group(0) + " #"][1]
print(intermediateArray)
    
for label in assemblyDef.reservedLabels:
    labelDict[label] = assemblyDef.reservedLabels[label]
print(labelDict)
del intermediateArray
del labelQueue

finalArray = [""] * 0x80

#Step 4: Parse the lines, and store the instructions into the final array, replacing all labels with their locations.
print("Parsing lines...")
intIndex = 0
for line in lines:
    currentLine = line.strip();
    if currentLine == "" or currentLine[0] == ";" or ":" in currentLine:
        continue #This is a comment or a blank line, so ignore it.

    
    ins = re.match(r'ld(a|b|x|y)|st(a|b|x|y)', currentLine)
    if line.split(" ")[0] in assemblyDef.macros:
        #This is a macro. Process it.
        splitLine = line.split(" ")
        searchLine = splitLine[0]
        regA = 0
        regB = 0
        regC = 0
        regD = 0
        if(len(splitLine) == 2):
            regA = splitLine[1]
            searchLine += " {$1}"
        if(len(splitLine) == 3):
            regA = splitLine[1]
            regB = splitLine[2]
            searchLine += " {$1} {$2}"
        regC = 0 if regA != 0 and regB != 0 else 1 if regA != 1 and regB != 1 else 2 if regA != 2 and regB != 2 else 3
        regD = 0 if (regA != 0 and regB != 0 and regC != 0) else 1 if (regA != 1 and regB != 1 and regC != 1) else 2 if (regA != 2 and regB != 2 and regC != 3) else 3
        for inst in assemblyDef.macros[searchLine]:
            finalArray[intIndex] = [line, inst.replace("$1", regA).replace("$2", regB).replace("$3", regC).replace("$4", regD)]
            intIndex += 1
    
    if ins is None:
        intIndex += assemblyDef.opCodes[currentLine][1]
        finalArray[intIndex] = [currentLine, assemblyDef.opCodes[currentLine][0]]

    else:
        print(ins.group(0))
        if "$" in currentLine:
            #This is a memory line
            finalArray[intIndex] = [currentLine, assemblyDef.opCodes[ins.group(0) + " $"][0]]
            if "@" in currentLine:
                #This is a memory line with a label
                finalArray[intIndex + 1] = ["", labelDict[currentLine[currentLine.index("@") + 1:]] & 0xFF]
                finalArray[intIndex + 2] = ["", labelDict[currentLine[currentLine.index("@") + 1:]] >> 8]

            intIndex += assemblyDef.opCodes[ins.group(0) + " $"][1]

        else:
            #This is an immediate line
            finalArray[intIndex] = [currentLine, assemblyDef.opCodes[ins.group(0) + " #"][0]]
            intIndex += assemblyDef.opCodes[ins.group(0) + " #"][1]
            finalArray[intIndex] = ["", int(currentLine[currentLine.index("#") + 1:])]


print("Writing to file...")
ext = [
    "bin",
    "lst",
    "txt"
]
if outputFile == "":
    outputFile = inputFile[0:len(inputFile)-4] + ".SAF.ml."

byteArray = []
for line in finalArray:
    if line == "":
        continue
    byteArray.append(line[1])

with open(outputFile + ext[0], "wb") as file:
    file.write(bytearray(byteArray))
    file.close()



with open(outputFile + ext[2], "w") as file:
    for line in finalArray:
        if line == "":
            continue
        file.write(str(line[1]) + "\n")
    file.close()

