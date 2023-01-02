# Import all the libraries we need.
import re                                   # Regex Library
from multipledispatch import dispatch       # Function Overloading
from typing import List                     # Type hints

# This class will store infomration regaring directives, such as 
class Directive:
    def __init__(self, dir: str, addr: int):
        self.address = addr
        self.len = 0
        self.data = []
        if(dir[0] == "."):
            dir = dir[1:]
        if(dir[0:3] == "org"):
            self.isOrg = True
            self.orgAddr = dir[4::]
        if(dir[0:2] == "db"):
            if dir[3] == "\"":
                #loop through the string and add it to the data.
                i = 4
                while(dir[i] != "\""):
                    if(dir[i] == "\\"):
                        pass
                    self.data.append(ord(dir[i]))
                    i += 1
                    self.len += 1
            elif dir[3] == "'":
                self.data.append(ord(dir[4]))
                self.len = 1
            else:
                data = dir[3::].split(" ")
                for b in data:
                    self.data.append(int(b, 16))
                    self.len += 1
    def __len__(self):
        return self.len
    def output(self):
        return bytearray(self.data)
    def getAddr(self) -> int:
        if self.__dict__.get("isOrg", None):
            return int(self.orgAddr, 16) & 0x7FFF
        else:
            return -1

class Label:
    def __init__(self, name: str, address: int):
        self.name = name
        self.addr = address & 0x7FFF
    def __repr__(self):
        return f"Label {self.name} at Address {self.addr}"

# This class is what is responsible for actually doing the conversion.
class Instruction:
    @dispatch(str, int)
    # This will give an error because pylance does not realize the dispatch decorator.
    # Hence, we add the type: ignore line which removes the error.
    def __init__(self, inst: str, address: int):  #type: ignore
        self.__init__(decode(inst), address)
    @dispatch(re.Match, int)
    def __init__(self, instData: re.Match, address: int):

        if instData.group(0)[0] == "!":
            self.isResolved = False
            self.isMem = True
            self.isImm = False
            self.labelData = re.match("^!(jmp|jsr|ret|jse|jsn|bne|beq) ([A-Za-z]+)$", instData.group(0))
            self.labelName = self.labelData.group(2) if self.labelData is not None else ""
            self.instName = self.labelData.group(1) if self.labelData is not None else ""
            self.address = address
            return

        self.address = address
        self.isMem = None
        self.isImm = None
        self.data = None
        
        # Decode instData
        for i in range(1, instData.span()[1]):
            try:
                instData.group(i)
            except IndexError:
                continue
            if instData.group(i) is not None:
                if i == 1:
                    self.instName = instData.group(i)
                elif i == 2:
                    self.RS = instData.group(i)
                elif i == 3:
                    self.RD = instData.group(i)
                elif i == 4:
                    self.isMem = (instData.group(i) == "$")
                    self.isImm = (instData.group(i) == "#")
                elif i == 5:
                    if(self.isMem):
                        self.mem = int(instData.group(i), 16)
                    if(self.isImm):
                        self.imm = int(instData.group(i), 16)
    def __repr__(self):
        retStr = ""
        data = [f"{x}: {self.__dict__[x]}"for x in self.__dict__ if self.__dict__[x] is not None]
        for i in data:
            retStr += i
            if i != data[-1]:
                retStr += "\n"
        return retStr

    def __len__(self):
        if(self.isMem):
            return 3
        if(self.isImm):
            return 2
        return 1
    
    def lblGet(self, labels: List[Label]):
        if self.__dict__.get("isResolved") is None or self.__dict__.get("isResolved"):
            return
        lbl = self.labelName
        for label in labels:
            if label.name == lbl:
                self.mem = label.addr
                self.isResolved = None
                break
        self.convert()

    def convert(self) -> None:
        if self.__dict__.get("isResolved") is not None and self.__dict__.get("isResolved") == False:
            return
        self.data = [0] * len(self)
        if(self.isMem):
            self.data[2] = self.mem & 0xFF
            self.data[1] = (self.mem >> 8) & 0xFF
        if(self.isImm):
            self.data[1] = self.imm & 0xFF
        self.data[0] = decodeData(self)
    def output(self) -> bytearray | None:
        if(self.data is None):
            self.convert()
        else:
            return bytearray(self.data)

def getReg(reg: str):
    if(reg == "a"):
        return 0x00
    elif(reg == "b"):
        return 0x01
    elif(reg == "x"):
        return 0x02
    elif(reg == "y"):
        return 0x03
    else:
        print("Syntax Error: Invalid Register!")
        exit()
        
#This function is total bullshit
def decodeData(ins: Instruction):
    instName = ins.instName
    if(ins.isMem == True):
        if(instName[0:-1] == "ld"):
            return 0xF0 + (getReg(instName[-1]) << 2)
        elif(instName[0:-1] == "st"):
            return 0xC0 + getReg(instName[-1])
        elif(instName == "jmp"):
            return 0x70
        elif(instName == "jsr"):
            return 0x71
        elif(instName == "ret"):
            return 0x72
        elif(instName == "jse"):
            return 0x73
        elif(instName == "jsr"):
            return 0x74
        elif(instName == "bne"):
            return 0x75
        elif(instName == "beq"):
            return 0x76
        else:
            print("Syntax Error: Invalid Memory Instruction!")
            exit()
    elif(ins.isImm == True):
        if(instName == "add"):
            return 0x12 + (getReg(ins.RD) << 2)
        elif(instName == "sub"):
            return 0x13 + (getReg(ins.RD) << 2)
        elif(instName[0:-1] == "ld"):
            return 0xF1 + (getReg(instName[-1]) << 2)
        else:
            print("Syntax Error: Invalid Immediate Instruction!")
            exit()
    else:
        if(instName == "nop"):
            return 0x00
        elif(instName == "rst"):
            return 0x01
        elif(instName == "hlt"):
            return 0x02
        elif(instName == "brk"):
            return 0x03
        elif(instName == "ctn"):
            return 0x04
        elif(instName == "clc"):
            return 0x05
        elif(instName == "cln"):
            return 0x06
        elif(instName == "clz"):
            return 0x07
        elif(instName == "clv"):
            return 0x08
        elif(instName == "rol"):
            return 0x10 + (getReg(ins.RD) << 2)
        elif(instName == "ror"):
            return 0x11 + (getReg(ins.RD) << 2)
        elif(instName == "inc"):
            return 0x20 + (getReg(ins.RD) << 2)
        elif(instName == "dec"):
            return 0x21 + (getReg(ins.RD) << 2)
        elif(instName == "pop"):
            return 0x30 + (getReg(ins.RD) << 2)
        elif(instName == "top"):
            return 0x31 + (getReg(ins.RD) << 2)
        elif(instName == "sm"):
            if ins.RD == 'x' or ins.RD == 'y':
                print("Syntax Error: sm can't use x or y as source!")
                exit()
            return 0x40 + getReg(ins.RD)
        elif(instName == "t"):
            if(ins.RS == ins.RD):
                print("WARN: t with same source and destination! -> nop")
                return 0x00
            return 0x50 + (getReg(ins.RD) << 2) + getReg(ins.RS)
        elif(instName == "add"):
            return 0x80 + (getReg(ins.RD) << 2) + getReg(ins.RS)
        elif(instName == "sub"):
            return 0x90 + (getReg(ins.RD) << 2) + getReg(ins.RS)
        elif(instName == "nand"):
            return 0xA0 + (getReg(ins.RD) << 2) + getReg(ins.RS)
        elif(instName == "psh"):
            return 0xB0 + getReg(ins.RD)
        elif(instName == "cmp"):
            return 0xD0 + (getReg(ins.RD) << 2) + getReg(ins.RS)
        elif(instName == "lm"):
            if ins.RD == 'x' or ins.RD == 'y':
                print("Syntax Error: lm can't use x or y as source!")
                exit()
            return 0xF3 + (getReg(ins.RD) << 2)
        elif(instName == "lim"):
            if(ins.RD == 'x'):
                print("Syntax Error: lim can't use x or y as source!")
                exit()
            return 0xF2 + (getReg(ins.RD) << 2)
        else:
            print(f"Error: Instruction {ins.instName} not supported!")
            exit()

def decode(ins: str):
    instructionRegex = """^((?:nop|rst|hlt|brk|ctn|clc|cln|clz|clv|cli|add|rol|ror|t|jmp|jsr|ret|jse|jsn|bne|beq|inc|dec|sub|psh|pop|(?:ld[abxy])|(?:st[abxy])|lhm|lm|sm|cmp|nand)) ?(?:(?:(?:([abxy]) ?([abxy]|#[0-9A-Fa-f]+)?)|(?:(\$|#)([0-9A-Fa-f]+?))))?$"""
    regexResult = re.match(instructionRegex, ins)
    if regexResult is not None:
        return regexResult
    labeledJumpRegex = """^!(jmp|jsr|ret|jse|jsn|bne|beq) ([A-Za-z]+)$"""
    jumpResult = re.match(labeledJumpRegex, "!" + ins)
    if jumpResult is not None:
        return jumpResult

    print(f"Error: Instruction {ins} not supported!")
    exit()
    



with open("file.txt", "r") as f:
    instArr: List[Instruction | Directive] = []
    labelList: List[Label] = []
    addr: int = 0
    for line in f:
        line = line.split(";")[0]
        insLine = line.strip()
        if(insLine == ""):
            continue
        if(insLine[0] == "."):
            currDir = Directive(insLine, addr)
            if currDir.getAddr() != -1:
                addr = currDir.getAddr()
                continue
            instArr.append(currDir)
            addr += len(currDir)



        elif(":" in insLine):
            lbl = Label(line[0:line.find(":")], addr)
            labelList.append(lbl)
            print(lbl)
        else:
            #Pylance doesn't know about the decorator, so it throws an error. This fixes it.
            x = Instruction(insLine, addr) #type: ignore
            print(x.instName)
            instArr.append(x)
            x.convert()
            addr += len(x)
    for line in instArr:
        line.lblGet(labelList) if isinstance(line, Instruction) else ""
    with open("file.txt.out", 'bw') as outputFile:
        outputAddr = 0
        for inst in instArr:
            print(inst)
            while(outputAddr != inst.address):
                outputAddr+= 1
                outputFile.write(b'\x00')
                print(outputAddr)
            data = inst.output()
            if data is not None:
                outputFile.write(data)
            else:
                print(f"Error: data not found for inst:\n {inst}")
            outputAddr += len(inst)