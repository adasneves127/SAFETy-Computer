# Import all the libraries we need.
from multipledispatch import dispatch  # Function Overloading #type: ignore
from typing import List  # Type hints
from sys import stderr, argv
import json, re, os



# Global Includes Directory
includeFiles = []
headerLines = {}
headerPath = ""


# This is a basic class to hold labels.
# It will hold the name and the address.
class Label:
    def __init__(self, name: str, address: int):
        self.name = name
        self.addr = address & 0x7FFF

    def __repr__(self):
        return f"Label {self.name} at Address {self.addr}"

class Call:
    def __init__(self, data: re.Match, addr: int):
        self.mem = 0
        self.instName = "call"
        self.raw = data
        self.address = addr
        if data.group(2) not in includeFiles:
            print(includeFiles)
            print(f"Error: File {data.group(2)} not included!", file=stderr)
            exit()

        with open(f"{headerPath}/{data.group(2)}.json", 'r') as f:
            fileData = json.load(f)
            if(data.group(3) in headerLines.keys()):
                pass
                #TODO: Populate data values with address.
                return
            print(fileData)
            headerLines[data.group(2) + "." + data.group(3)] = {"translation": fileData['translations'][data.group(2) + "." + data.group(3)], "data": []}
            headerLines[data.group(2) + "." + data.group(3)]['data'].extend(fileData['definitions'][headerLines[data.group(2) + "." + data.group(3)]['translation']])
            print(headerLines)

    def convert(self):
        self.data = [0x0A, (self.mem >> 8) & 0xFF, self.mem & 0xFF]

    def __len__(self):
        return 3
    def output(self) -> bytearray:
        return bytearray(self.data)
    def lblGet(self, labels: List[Label]):
        print(labels)
        for label in labels:
            if self.raw.group(2) + "." + self.raw.group(3) == label.name:
                print(label.addr)
                self.mem = label.addr
                self.convert()
    def __str__(self) -> str:
        ret_str = f"{self.instName} {self.raw.group(3)}"
        ret_str += " " * (13 - len(ret_str)) + f"| {' '.join([str(x) for x in self.data])}\n" 
        return ret_str

        
# This class will store infomration regaring directives, such as org, inc, and db
class Directive:
    # When we initialize, we want to get the directive, as well as the address where it will be placed.
    def __init__(self, dir: str, addr: int):
        self.raw = dir
        # Save the address
        self.address = addr

        # Create 2 variables, len and data
        self.len = 0
        self.data = []
        # If the dir still has the leading ".", remove it.
        if dir[0] == ".":
            dir = dir[1:]

        # Check for what directives we are using.
        if dir[0:3] == "inc":
            # Include a file
            includeFiles.append(dir[4::])
        if dir[0:3] == "org":
            self.isOrg = True
            self.orgAddr = dir[4::]
        if dir[0:2] == "db":
            # For a databyte, we have strings, characters, and hex numbers.
            # Strings:
            if dir[3] == '"':
                # loop through the string and add it to the data.
                i = 4
                # While we are not at the end of the string:
                while dir[i] != '"':
                    # Append the current character to data
                    self.data.append(ord(dir[i]))
                    # increment i and len
                    i += 1
                    self.len += 1
                self.len += 1
                self.data.append(0)
            # Chars:
            elif dir[3] == "'":
                # Append the single character to the data
                self.data.append(ord(dir[4]))
                # Set the len to 1
                self.len = 1
            else:

                # This is a string of hex numbers
                data = dir[3::].split(" ")
                for num in data:
                    dataNum = int(num, 16)
                    # If the number is too large:
                    if dataNum > 255:
                        print(
                            f"Error: Databyte is too large! Read: {num}, expected: 0x00 - 0xFF", file=stderr
                        )
                        exit()
                    # Otherwise, add it to the list, and add 1 to the length
                    self.data.append(dataNum)
                    self.len += 1

    # Get the length of the directive
    def __len__(self):
        return self.len

    # Return the bytes from data
    def output(self):
        return bytearray(self.data)

    # Get the address where the directive is.
    def getAddr(self) -> int:
        if self.__dict__.get("isOrg", None):
            return int(self.orgAddr, 16) & 0x7FFF
        else:
            return -1

    def __str__(self):
        # Build a string:
        output = f"{self.raw}{' ' * (12 - len(self.raw))} | {self.data}\n"
        return output




# This class is what is responsible for actually doing the conversion.
class Instruction:
    @dispatch(str, int)
    # This will give an error because pylance does not realize the dispatch decorator.
    # Hence, we add the type: ignore line which removes the error.
    def __init__(self, inst: str, address: int):  # type: ignore
        self.__init__(decode(inst), address) #type:ignore

    @dispatch(re.Match, int)
    def __init__(self, instData: re.Match, address: int):
        self.raw = instData
        # The code that runs this function will add a ! if it thinks it is a labeled memory instruction.
        if instData.group(0)[0] == "!":
            self.isResolved = False
            self.isMem = True
            self.isImm = False
            # Use this to capture the information that we need
            self.labelData = re.match(
                "^!(jmp|jsr|ret|jse|jsn|bne|beq|st[abxy]|ld[abxy]) ([A-Za-z]+)$",
                instData.group(0),
            )
            # Set the label name and instruction name to what was captured.
            self.labelName = (
                self.labelData.group(2) if self.labelData is not None else ""
            )
            self.instName = (
                self.labelData.group(1) if self.labelData is not None else ""
            )
            # Set the address to what we need
            self.address = address
            return
        # If this is not a labeled instruction, we do the following.

        # Set the address
        self.address = address
        # Initialize some values
        self.isMem = None
        self.isImm = None
        self.data = None

        # Decode instData
        # Go through the span that the instruction takes up
        for i in range(1, instData.span()[1]):
            # If instData.group(i) does not exist, just continue.
            try:
                instData.group(i)
            except IndexError:
                continue
            # If the current group is not none
            if instData.group(i) is not None:
                # Group 1 is the instruction name
                if i == 1:
                    self.instName = instData.group(i)
                # Instruction Group 2 is the first register given
                elif i == 2:
                    self.RS = instData.group(i)
                # Instruction Group 3 is the second register given
                elif i == 3:
                    self.RD = instData.group(i)
                # Group 4 tells us if it is an immediate or a memory instruction
                elif i == 4:
                    self.isMem = instData.group(i) == "$"
                    self.isImm = instData.group(i) == "#"
                # Group 5 contains the memory or the immediate.
                elif i == 5:
                    if self.isMem:
                        self.mem = int(instData.group(i), 16)
                    if self.isImm:
                        self.imm = int(instData.group(i), 16)

    # Debugging Repr method just returns a weirdly formatted string...
    def __repr__(self):
        retStr = ""
        data = [
            f"{x}: {self.__dict__[x]}"
            for x in self.__dict__
            if self.__dict__[x] is not None
        ]
        for i in data:
            retStr += i
            if i != data[-1]:
                retStr += "\n"
        return retStr

    # Get the length of the data.
    # All instructions with mem have a len of 3
    # All immediate instructions have a len of 2
    # All other instructions have a len of 1.
    def __len__(self):
        if self.isMem:
            return 3
        if self.isImm:
            return 2
        return 1

    # Decode the labels that are given
    def lblGet(self, labels: List[Label]):
        # If the label is resolved, or if the instruction does not need to be resolved
        if self.__dict__.get("isResolved") is None or self.__dict__.get("isResolved"):
            return
        # Short hand for label name
        lbl = self.labelName
        # For each label in the list
        for label in labels:
            # If the label is equal to the current instruction's label
            if label.name == lbl:
                # Set the address to the memory address of the label
                self.mem = label.addr
                # Set the "isResolved" to None.
                self.isResolved = None
                # exit the loop
                break
        # Convert the current data in the instruction
        self.convert()

    # Many will be upset with me hijacking a __str__ method, but im lazy...
    def __str__(self):
        # Build a string:
        output = f"{self.raw.group(0)}{' ' * (12 - len(self.raw.group(0)))} | {self.data[0] if self.data else ''}\n"
        try:
            output += f"{' ' * 12} | {self.data[1] if self.data else ''}\n"
        except IndexError:
            pass

        try:
            output += f"{' ' * 12} | {self.data[2] if self.data else ''}\n"
        except IndexError:
            pass
        return output

    def convert(self) -> None:
        # If the label is not resolved, exit. (We DO NOT want to resolve right now, just in case the label has not been found yet.)
        if (
            self.__dict__.get("isResolved") is not None
            and self.__dict__.get("isResolved") == False
        ):
            return
        # Create a list with the length of the current instruction
        self.data = [0] * len(self)
        # If it is a memory instruction, fill out the important information
        if self.isMem:
            self.data[2] = self.mem & 0xFF
            self.data[1] = (self.mem >> 8) & 0xFF

        # If it is an immediate instruction, get the immediate
        if self.isImm:
            self.data[1] = self.imm & 0xFF
        # Set the instruction to decoded data.
        self.data[0] = decodeData(self)

    # Get the binary list output of the instruction.
    def output(self) -> bytearray:
        if self.data is None:
            self.convert()
        # If the instruction is not decoded:
        if self.data:
            return bytearray(self.data)
        else:
            print(
                f"WARN: Inst {self.instName} at address {self.address} is not decoded correctly! Replaced with NOP"
            )
            return b'\x00' #type: ignore


# This function will return a number between [0-4) for the register.
def getReg(reg: str):
    if reg == "a":
        return 0x00
    elif reg == "b":
        return 0x01
    elif reg == "x":
        return 0x02
    elif reg == "y":
        return 0x03
    else:
        print("Syntax Error: Invalid Register!", file=stderr)
        exit()


# This function is total bullshit
def decodeData(ins: Instruction):
    # Get the instruction name
    instName = ins.instName

    # If the instruction is a memory instruction, decode it with those conversions.
    if ins.isMem == True:
        if instName[0:-1] == "ld":
            return 0xF0 + (getReg(instName[-1]) << 2)
        elif instName[0:-1] == "st":
            return 0xC0 + getReg(instName[-1])
        elif instName == "jmp":
            return 0x70
        elif instName == "jsr":
            return 0x71
        elif instName == "jse":
            return 0x73
        elif instName == "jsr":
            return 0x74
        elif instName == "bne":
            return 0x75
        elif instName == "beq":
            return 0x76
        # If the instruction cannot be found, print it, and exit.
        else:
            print(f"Syntax Error: Invalid Memory Instruction {instName}", file=stderr)
            exit()
    # Otherwise, decode it if it is an immediate instruction
    elif ins.isImm == True:
        if instName == "add":
            return 0x12 + (getReg(ins.RD) << 2)
        elif instName == "sub":
            return 0x13 + (getReg(ins.RD) << 2)
        elif instName[0:-1] == "ld":
            return 0xF1 + (getReg(instName[-1]) << 2)
        else:
            print(f"Syntax Error: Invalid Immediate Instruction {instName}", file=stderr)
            exit()
    # Otherwise, decode it like normal.
    else:
        if instName == "nop":
            return 0x00
        elif instName == "rst":
            return 0x01
        elif instName == "hlt":
            return 0x02
        elif instName == "brk":
            return 0x03
        elif instName == "ctn":
            return 0x04
        elif instName == "clc":
            return 0x05
        elif instName == "cln":
            return 0x06
        elif instName == "clz":
            return 0x07
        elif instName == "clv":
            return 0x08
        elif instName == "rol":
            return 0x10 + (getReg(ins.RD) << 2)
        elif instName == "ror":
            return 0x11 + (getReg(ins.RD) << 2)
        elif instName == "inc":
            return 0x20 + (getReg(ins.RD) << 2)
        elif instName == "dec":
            return 0x21 + (getReg(ins.RD) << 2)
        elif instName == "pop":
            return 0x30 + (getReg(ins.RS) << 2)
        elif instName == "top":
            return 0x31 + (getReg(ins.RD) << 2)
        elif instName == "sm":
            if ins.RD == "x" or ins.RD == "y":
                print("Syntax Error: sm can't use x or y as source!", file=stderr)
                exit()
            return 0x40 + getReg(ins.RD)
        elif instName == "t":
            if ins.RS == ins.RD:
                print("WARN: t with same source and destination! -> nop", file=stderr)
                return 0x00
            return 0x50 + (getReg(ins.RD) << 2) + getReg(ins.RS)
        elif instName == "add":
            return 0x80 + (getReg(ins.RD) << 2) + getReg(ins.RS)
        elif instName == "sub":
            return 0x90 + (getReg(ins.RD) << 2) + getReg(ins.RS)
        elif instName == "nand":
            return 0xA0 + (getReg(ins.RD) << 2) + getReg(ins.RS)
        elif instName == "psh":
            return 0xB0 + getReg(ins.RS)
        elif instName == "cmp":
            return 0xD0 + (getReg(ins.RD) << 2) + getReg(ins.RS)
        elif instName == "lm":
            if ins.RD == "x" or ins.RD == "y":
                print("Syntax Error: lm can't use x or y as source!", file=stderr)
                exit()
            return 0xF3 + (getReg(ins.RD) << 2)
        elif instName == "lim":
            if ins.RD == "x":
                print("Syntax Error: lim can't use x or y as source!", file=stderr)
                exit()
            return 0xF2 + (getReg(ins.RD) << 2)
        elif instName == "ret":
            return 0x72
        elif instName == "rfc":
            return 0x0b
        else:
            print(f"Error: Instruction {ins.instName} not supported!", file=stderr)
            exit()


# Decode the instruction and check if it is valid.
def decode(ins: str):
    # This regex checks to see if the instruction is valid, and if it is a register, imm, or mem instruction.
    instructionRegex = """^((?:nop|rst|hlt|brk|ctn|clc|cln|clz|clv|cli|add|rol|ror|t|jmp|jsr|ret|jse|jsn|bne|beq|inc|dec|sub|psh|pop|rfc|(?:ld[abxy])|(?:st[abxy])|lhm|lm|sm|cmp|nand)) ?(?:(?:(?:([abxy]) ?([abxy]|#[0-9A-Fa-f]+)?)|(?:(\$|#)([0-9A-Fa-f]+?))))?$"""  # type: ignore
    regexResult = re.match(instructionRegex, ins)
    if regexResult is not None:
        # If the result exists, then return it.
        return regexResult
    # Otherwise, check if it is a labeled memory instuction
    labeledJumpRegex = (
        """^!(jmp|jsr|ret|jse|jsn|bne|beq|ld[abxy]|st[abxy]) ([A-Za-z]+)$"""
    )
    # We added an ! to the beginning of the instruction to let us know down the line that this is not yet resolved.
    jumpResult = re.match(labeledJumpRegex, "!" + ins)
    if jumpResult is not None:
        return jumpResult

    #Check if it is a "call" instruction
    callRegex = "^(call) ([a-zA-Z]+).([a-zA-Z]+)$"
    callResult = re.match(callRegex, ins)
    if callResult is not None:
        return (callResult, 1)
    # Otherwise, return an error.
    print(f"Error: Instruction {ins} not supported!", file=stderr)
    exit()


def main(argc: int, argv: List[str]):
    global settings
    print(settings)
    # Step 1: Parse the command line arguments.
    # The last item should be the input file.
    inputFile = argv[-1]
    outputFile = inputFile + ".bin"
    options = 0

    # Iterate through the arguments, excluding the output file
    for i in range(len(argv[1:-1])):
        if argv[i] == "-nL":
            options |= 0b00000001
        if argv[i] == "-nB":
            options |= 0b00000010
        if argv[i] == "-o":
            i += 1
            outputFile = argv[i] + ".bin"

    try:
        open(inputFile).close()
    except FileNotFoundError:
        print(f"Error: File {inputFile} not found!", file=stderr)
        exit()

    with open(inputFile, "r") as f:
        instArr: List[Instruction | Directive | Call] = []
        labelList: List[Label] = []
        for name, addr in settings["DefaultLabels"].items():
            labelList.append(Label(name, addr))
        for name, addr in settings["CustomLabels"].items():
            labelList.append(Label(name, addr))
        
        addr: int = 0
        for line in f:
            line = line.split(";")[0]
            insLine = line.strip()
            if insLine == "":
                continue
            if insLine[0] == ".":
                currDir = Directive(insLine, addr)
                if currDir.getAddr() != -1:
                    addr = currDir.getAddr()
                    continue
                instArr.append(currDir)
                addr += len(currDir)

            elif ":" in insLine:
                lbl = Label(line[0 : line.find(":")], addr)
                labelList.append(lbl)
                print(lbl)
            else:
                # Pylance doesn't know about the decorator, so it throws an error. This fixes it.
                regexData = decode(insLine)
                x: Call | Instruction
                if isinstance(regexData, tuple):
                    x = Call(regexData[0], addr) #type:ignore
                    raise NameError("Instruction CALL is not supported by the current software.")
                    exit(-1)
                else:
                    x = Instruction(insLine, addr)  # type: ignore
                print(x.instName)
                instArr.append(x)
                x.convert()
                addr += len(x)
        #Compile the headers!
        headerFileName = outputFile + ".hdr"
        hdrOutput: List[Instruction|Call] = []
        hdrLabel = []
        hdrIdx = 0
        for func in headerLines.items():
            hdrLabel.append(Label(func[0], hdrIdx))
            for ins in func[1]['data']:
                x = Instruction(ins, hdrIdx)
                hdrOutput.append(x)
                hdrIdx += len(x)
        with open(headerFileName, 'bw') as f:

            for line in hdrOutput:
                line.lblGet(hdrLabel)
                line.convert()

                f.write(line.output())
        for line in instArr:
            line.lblGet(labelList) if isinstance(line, Instruction) else ""
            line.lblGet(hdrLabel) if isinstance(line, Call) else ""
        #print(headerLines)
        if options & 0b00000010 == 0:
            with open(outputFile, "bw") as f:
                outputAddr = 0
                for inst in instArr:
                    while outputAddr != inst.address:
                        outputAddr += 1
                        f.write(b"\x00")
                        print(outputAddr)
                    data = inst.output()
                    if data is not None:
                        f.write(data)
                    else:
                        print(f"Error: data not found for inst:\n {inst}", file=stderr)
                    outputAddr += len(inst)
        if options & 0b00000001 == 0:
            with open(outputFile + ".lst", "w") as f:
                outputAddr = 0
                for inst in instArr:
                    f.write(str(inst))


if __name__ == "__main__":
    global settings
    dir_path = os.path.dirname(os.path.realpath(__file__))
    settings = {}
    try:
        with open(f"{dir_path}/config.json", 'r') as f:
            settings = json.load(f)
            headerPath = settings["headerPath"]
    except:
        print(f"Err: config.json file not found at {dir_path}/config.json", file=stderr)
        exit()
    main(len(argv), argv)
