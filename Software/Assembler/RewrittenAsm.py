#!/usr/bin/env python3
import re

from typing import List, Dict, Tuple

import assemblyDef

from color import printColor


# DEBUGGING FLAG
debug = True



def usage() -> None:
    print("""SAFE Assembler
    Usage: python3 Assembler.py [-o] [-nL] [-nS] <filename>
    -o: Output to a file.
    -nL: Do not output a listing file.
    -nS: Do not output a simulator file.
    """)


def readFile(infile) -> List[str]:
    with open(infile, "r") as file:
        lines = file.readlines()
    return lines



class Queue:
    def __init__(self):
        self.size = 0
        self.items = []

    def isEmpty(self):
        return self.items == []

    def enqueue(self, item):
        self.items.insert(0, item)
        self.size = len(self.items)

    def dequeue(self):
        self.size = len(self.items)
        return self.items.pop()


REGS = { 
    "A": 0, 
    "B": 1, 
    "X": 2, 
    "Y": 3 }


class Assembler:
    def __init__(self, argc, argv):

        self.outputFile = ""
        self.noListing = False
        self.noSimulator = False
        self.lines = []
        self.labelQueue = Queue()

        if argc < 2 and not debug:
            printColor("Err: ", "red", end="")
            printColor("python3 Assembler.py <filename>", "yellow")
            exit()

        if "-h" in argv:
            usage()
            exit(-1)

        if "-o" in argv:
            stem = argv[argv.index("-o") + 1]
            self.outputFile = stem[len(stem) - 4] + "SAF.ml."

        if "-nL" in argv:
            print("No listing output.")
            self.noListing = True

        if "-nS" in argv:
            print("No simulator output.")
            self.noSimulator = True
        
        self.inputFile = argv[-1]

    
        

    def run(self):
        self.lines = readFile(self.inputFile)

        intArray, labelQueue = self.parse_array()

        labelDict = self.parse_labels(intArray, labelQueue)

        del intArray
        del labelQueue

        


        


        


        pass

    def parse_array(self) -> Tuple[List[str], Queue]:
        print("Creating intermediate array...")

        labelQueue = Queue()

        intermediateArray = [""] * 0x8 #This is the maximum size possible for a program in ROM.
        intIndex = 0

        for line in self.lines:
            currentLine = line.strip()

            if currentLine == "" or currentLine[0] == ";":
                continue # This is a comment or a blank line, so ignore it.

            if line.split(" ")[0] in assemblyDef.macros:
                # This is a macro. Process it.
                intIndex += len(assemblyDef.macros[line.split(" ")[0]])
            
            instruction = re.match(r'ld(a|b|x|y)|st(a|b|x|y) (\$|#)', currentLine)

            if ":" in currentLine:
                labelQueue.enqueue(intIndex, currentLine[0:currentLine.index(":")])
            
            if instruction is None:
                intermediateArray[intIndex] = currentLine
                intIndex += assemblyDef.opCodes[currentLine]
            else:
                print(instruction.group(0))
                if "$" in currentLine:
                    intermediateArray[intIndex] = assemblyDef.opCodes[str(instruction.group(0)) + " $"]
                    intIndex += assemblyDef.opCodes[str(instruction.group(0)) + " $"][1]
                else:
                    intermediateArray[intIndex] = assemblyDef.opCodes[str(instruction.group(0)) + " #"]
                    intIndex += assemblyDef.opCodes[str(instruction.group(0)) + " #"][1]
            
        print(intermediateArray)

        return intermediateArray, labelQueue
    

    def parse_labels(self, intermediateArray: List[str], labelQueue: Queue) -> Dict[str, List[int, int]]:
        #Step 3: Create a dict of labels, and their locations.

        print("Creating label dictionary...")
        labelDict: Dict[str, List[int, int]] = {}

        # Raise error if label already exists
        # Raise error if label is reserved


        for addr, line in enumerate(intermediateArray):
            pass
            

        raise NotImplementedError()
    

    def construct_final(self, labelDict: Dict[str, List[int]]) -> List[List[str | int]]:
        finalArray = [""] * 0x80

        print("Parsing lines...")

        intIndex = 0

        for line in self.lines:
            currentLine = line.strip()

            if currentLine == "" or currentLine[0] == ";":
                continue # This is a comment or a blank line, so ignore it.

            instruction = re.match(r'ld(a|b|x|y)|st(a|b|x|y) (\$|#)', currentLine)

            if line.split(" ")[0] in assemblyDef.macros:
                # This is a macro. Process it.
                splitLine = line.split(" ")
                searchLine = splitLine[0]

                regA = 0
                regB = 0
                regC = 0
                regD = 0

                if (len(splitLine) == 2):
                    regA = splitLine[1]
                    searchLine += " {$1}"
                elif (len(splitLine) == 3):
                    regA = splitLine[1]
                    regB = splitLine[2]
                    searchLine += " {$1} {$2}"
                
                regC = 0 if regA != 0 and regB != 0 else 1 if regA != 1 and regB != 1 else 2 if regA != 2 and regB != 2 else 3
                regD = 0 if (regA != 0 and regB != 0 and regC != 0) else 1 if (regA != 1 and regB != 1 and regC != 1) else 2 if (regA != 2 and regB != 2 and regC != 3) else 3

                for inst in assemblyDef.macros[searchLine]:
                    finalArray[intIndex] = [line, inst.replace("$1", regA).replace("$2", regB).replace("$3", regC).replace("$4", regD)]
                    intIndex += 1
            
            if instruction is None:
                intIndex += assemblyDef.opCodes[currentLine]
                finalArray[intIndex] = line
            
            else:
                print(instruction.group(0))

                if "$" in currentLine:
                    # This is a memory line
                    finalArray[intIndex] = [currentLine, assemblyDef.opCodes[str(instruction.group(0)) + " $"][0]]
                    if "@" in currentLine:
                        # This is a memory line with a label.
                        finalArray[intIndex + 1] = ["", labelDict[currentLine[currentLine.index("@") + 1:]] & 0xFF]
                        finalArray[intIndex + 2] = ["", labelDict[currentLine[currentLine.index("@") + 1:]] >> 8]
                    
                    intIndex += assemblyDef.opCodes[str(instruction.group(0)) + " $"][1]
                else:
                    # This is an immediate line
                    finalArray[intIndex] = [currentLine, assemblyDef.opCodes[str(instruction.group(0)) + " #"][0]]
                    intIndex += assemblyDef.opCodes[str(instruction.group(0)) + " #"][1]
                    finalArray[intIndex] = ["", int(currentLine[currentLine.index("#") + 1:])]
        
        return finalArray
    

    def write_output(self, finalArray: List[List[str, int]]):
        print("Writing to file...")

        if self.outputFile is None or self.outputFile == "":
            self.outputFile = self.inputFile[:-4] + ".SAF.ml."
        
        byteArray: List[str | int] = []

        for line in finalArray:
            if line == "":
                continue
            byteArray.append(line[1])
        
        with open(f"{self.outputFile}.bin", "wb") as outfile:
            outfile.write(bytearray(byteArray))
        
        with open(f"{self.outputFile}.txt", "w") as outfile:
            for line in finalArray:
                if line == "":
                    continue
                outfile.write(f"{line[1]}\n")


def main() -> None:
    assembler = Assembler()
    assembler.run()
    return


if __name__ == "__main__":
    main()

#exit(-2)




## Constants / Helpful variables

macroNames = [key.split(" ")[0] for key in assemblyDef.macros.keys()]

## Helper functions

def getInstructionLength(instruction: str) -> int:
    if any(instruction.startswith(key) for key in assemblyDef.opCodes.keys()):
        return assemblyDef.opCodes[instruction][1]
    elif instruction.startswith(".db"):
        return 1
    else:
        return -1


def getInstructionML(instruction: str) -> int:
    if any(instruction.startswith(key) for key in assemblyDef.opCodes.keys()):
        return assemblyDef.opCodes[instruction][0]
    elif instruction.startswith(".db"):
        return instruction
    else:
        return -1



# Step 1: Open input file into lines
# Step 2: Handle Preprocessor directives
# Step 3: Recusively parse macros
# Step 4: Resolve labels
# Step 5: Compile into binary
# Step 6: Write output


infile = "zzTest.SAF"

# .org
# .db
# .inc



def process_preprocessor_directive(directive: str):
    raise NotImplementedError()



def process_macro(macro: str) -> List[str]:
    raise NotImplementedError()




def readfile(filename: str) -> List[str]:
    with open(filename, "r") as infile:
        return [line.strip() for line in infile.readlines() if line.strip() != "" or line.strip()[0] != ";"]

def writefile(filename: str, lines: List[int]):
    with open(filename, "wb") as outfile:
        outfile.write(bytearray(lines))



def macro_pass(lines: List[str]) -> Tuple[List[str], bool]:
    newLines = []
    processedMacro = False
    for line in lines:
        if line.split(" ")[0] in macroNames:
            newLines.extend(process_macro(line))
            processedMacro = True
        else:
            newLines.append(line)
    
    return newLines, processedMacro



def process_macros(lines: List[str]) -> List[str]:
    oldLines = lines[:]
    processedMacro = True

    while processedMacro:
        newLines, processedMacro = macro_pass(oldLines[:])
        oldlines = newLines
    
    return lines


def version2():
    lines = readfile(infile)
    macro_to_labels = process_macros(lines[:])
    labels_to_binary = resolve_labels(macro_to_labels[:])
    full_binary = compile_to_binary(labels_to_binary)
    writefile("outfile", full_binary) # TODO - Actual fucking filename
    
def resolve_labels(lines: List[str]) -> List[str]:
    
    
    addrLines: List[Tuple[int, str]] = []
    
    
    labelDict: Dict[str, int] = {}

    for (addr, line) in addrLines:
        if ":" in line:
            labelDict[line.split(":")[0]] = addr
    
    newLines: List[str] = []

    for line in lines:
        newline = line
        for key, value in labelDict.items():
            if key in newline:
                newline = newline.replace(key, f"{value}")
        newLines.append(newline)

    return newLines


def get_addresses(lines: List[str]) -> List[Tuple[int, str]]:
    addr = 0
    addrLines: List[Tuple[int, str]] = []
    
    for line in lines:
        addrLines.append((addr, line))
        addr += getInstructionLength(line)
    
    return addrLines



def compile_to_binary(lines: List[str]) -> List[int]:
    binary = []
    for line in lines:
        binary.extend(assemble_instruction(line))

    return binary


def format_int(num: str) -> int:
    value = num.replace("$", "0x")
    value = value.replace("#", "0x")
    return int(num, 16)


def assemble_instruction(line: str) -> List[int]:
    length = getInstructionLength(line)

    if length not in [1, 2, 3]:
        raise ValueError(f"Invalid instruction length: {length}")

    if length == 1:
        return [getInstructionML(line)]
    elif length == 2:
        return [getInstructionML(line), format_int(line.split(" ")[1])]
    elif length == 3:
        return [getInstructionML(line), format_int(line.split(" ")[2]), format_int(line.split(" ")[1])]
    else:
        raise ValueError(f"Invalid instruction length: {line}")

