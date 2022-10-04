#Labels which are reserved by the assembler.
reservedLabels = {
    "IOa":      0x7FF0,
    "Keyboard": 0x7FF0,
    "IOb":      0x7FF1,
    "LCD":      0x7FF1,
    "IOc":      0x7FF2,
    "IOd":      0x7FF3,
    "IOe":      0x7FF4,
    "IOf":      0x7FF5,
    "IOg":      0x7FF6,
    "IOh":      0x7FF7,
    "VIDMEML":  0x0200,
    "VIDMEMH":  0x05E8,
    "LINEIDX":  0x05e9,
    "GRAPHMODE":0x05ea,
    "PROGLOW":  0x4000,
    "PROGHIGH": 0x7FFF,
}

#These are small functions that are built into the assembler as pseudos.
macros = {
    "and": [
        "nand $1 $2", 
        "nand $1 $1"
        ],
    "or": [
        "not $1", 
        "not $2", 
        "nand $1 $2"
        ],
    "not": [
        "nand $1 $1"
        ],
    "xor": [
        "push $3", 
        "push $2", 
        "nand $2 $2", 
        "nand $2 $1", 
        "nand $1 $1", 
        "pop $3",
        "nand $1 $3", 
        "nand $1 $2", 
        "pop $3"
        ],
        
    "lsl": [
        "ROL $1", 
         "and $1 #0xFE"],
    "lsr": [
        "ROR $1", 
        "and $1 #0x7F"
        ],
    "pek": [
        "POP $1", 
        "PSH $1"
        ],
}

#List of all op codes, with their lengths, and machine code conversions.
#Should someone wish to change the op codes, they should only need to change the
#assembly codes, and recompile the program.
opCodes = {
#   ASM     ML    Length
    "nop": [0x00, 1],           #No Operation
    "rst": [0x01, 1],           #Reset
    "hlt": [0x02, 1],           #Halt (Stop the CPU. Requires Hard Reset)
    "brk": [0x03, 1],           #Break (Breakpoint)
    "ctn": [0x04, 1],           #Continue (Continue execution)
    "clc": [0x05, 1],           #Clear Carry Flag
    "cln": [0x06, 1],           #Clear Non-Zero Flag
    "clz": [0x07, 1],           #Clear Zero Flag
    "clv": [0x08, 1],           #Clear Overflow Flag
    "cli": [0x09, 1],           #Clear Interrupt Flag
    "add a a": [0x80, 1],       #Add A = A + A
    "add a b": [0x81, 1],       #Add A = A + B
    "add a x": [0x82, 1],       #Add A = A + X
    "add a y": [0x83, 1],       #Add A = A + Y
    "add b a": [0x84, 1],       #Add B = B + A
    "add b b": [0x85, 1],       #Add B = B + B
    "add b x": [0x86, 1],       #Add B = B + X
    "add b y": [0x87, 1],       #Add B = B + Y
    "add x a": [0x88, 1],       #Add X = X + A
    "add x b": [0x89, 1],       #Add X = X + B
    "add x x": [0x8A, 1],       #Add X = X + X
    "add x y": [0x8B, 1],       #Add X = X + Y
    "add y a": [0x8C, 1],       #Add Y = Y + A
    "add y b": [0x8D, 1],       #Add Y = Y + B
    "add y x": [0x8E, 1],       #Add Y = Y + X
    "add y y": [0x8F, 1],       #Add Y = Y + Y
    "add a #": [0x12, 2],       #Add A = A + #
    "add b #": [0x16, 2],       #Add B = B + #
    "add x #": [0x1A, 2],       #Add X = X + #
    "add y #": [0x1E, 2],       #Add Y = Y + #
    "rol a": [0x10, 1],         #Logical Shift Left A
    "rol b": [0x14, 1],         #Logical Shift Left B
    "rol x": [0x18, 1],         #Logical Shift Left X
    "rol y": [0x1C, 1],         #Logical Shift Left Y
    "ror a": [0x11, 1],         #Logical Shift Right A
    "ror b": [0x15, 1],         #Logical Shift Right B
    "ror x": [0x19, 1],         #Logical Shift Right X
    "ror y": [0x1D, 1],         #Logical Shift Right Y
    "tab": [0x61, 1],           #Transfer A to B
    "tax": [0x62, 1],           #Transfer A to X
    "tay": [0x63, 1],           #Transfer A to Y
    "tba": [0x64, 1],           #Transfer B to A
    "tbx": [0x66, 1],           #Transfer B to X
    "tby": [0x67, 1],           #Transfer B to Y
    "txa": [0x68, 1],           #Transfer X to A
    "txb": [0x69, 1],           #Transfer X to B
    "txy": [0x6B, 1],           #Transfer Y to A
    "tya": [0x6C, 1],           #Transfer Y to B
    "tyb": [0x6D, 1],           #Transfer Y to X
    "tyx": [0x6E, 1],           #Transfer Y to X
    "jmp": [0x70, 3],           #Jump
    "jsr": [0x71, 3],           #Jump to Subroutine
    "ret": [0x72, 1],           #Return from Subroutine
    "jse": [0x73, 3],           #Jump to Subroutine if Equal
    "jsn": [0x74, 3],           #Jump to Subroutine if Not Equal
    "bne": [0x75, 3],           #Branch if Not Equal
    "beq": [0x76, 3],           #Branch if Equal
    "inc a": [0x20, 1],         #Increment A
    "inc b": [0x24, 1],         #Increment B
    "inc x": [0x28, 1],         #Increment X
    "inc y": [0x2C, 1],         #Increment Y
    "dec a": [0x21, 1],         #Decrement A
    "dec b": [0x25, 1],         #Decrement B
    "dec x": [0x29, 1],         #Decrement X
    "dec y": [0x2D, 1],         #Decrement Y
    "inc $": [0xB4, 3],
    "dec $": [0xB7, 3],
    "sub a a": [0x90, 1],      #Subtract A = A - A
    "sub a b": [0x91, 1],      #Subtract A = A - B
    "sub a x": [0x92, 1],      #Subtract A = A - X
    "sub a y": [0x93, 1],      #Subtract A = A - Y
    "sub b a": [0x94, 1],      #Subtract B = B - A
    "sub b b": [0x95, 1],      #Subtract B = B - B
    "sub b x": [0x96, 1],      #Subtract B = B - X
    "sub b y": [0x97, 1],      #Subtract B = B - Y
    "sub x a": [0x98, 1],      #Subtract X = X - A
    "sub x b": [0x99, 1],      #Subtract X = X - B
    "sub x x": [0x9A, 1],      #Subtract X = X - X
    "sub x y": [0x9B, 1],      #Subtract X = X - Y
    "sub y a": [0x9C, 1],      #Subtract Y = Y - A
    "sub y b": [0x9D, 1],      #Subtract Y = Y - B
    "sub y x": [0x9E, 1],      #Subtract Y = Y - X
    "sub y y": [0x9F, 1],      #Subtract Y = Y - Y
    "sub a #": [0xBC, 2],      #Subtract A = A - #
    "sub b #": [0xBD, 2],      #Subtract B = B - #
    "sub x #": [0xBE, 2],      #Subtract X = X - #
    "sub y #": [0xBF, 2],      #Subtract Y = Y - #
    "psh a": [0xB0, 1],
    "psh b": [0xB1, 1],
    "psh x": [0xB2, 1],
    "psh y": [0xB3, 1],
    "pop a": [0x30, 1],
    "pop b": [0x34, 1],
    "pop x": [0x38, 1],
    "pop y": [0x3C, 1],
    "lda $": [0xF0, 3],
    "ldb $": [0xF4, 3],
    "ldx $": [0xF8, 3],
    "ldy $": [0xFC, 3],
    "lda #": [0xF1, 2],
    "ldb #": [0xF5, 2],
    "ldx #": [0xF9, 2],
    "ldy #": [0xFD, 2],
    "sta $": [0xC0, 3],
    "stb $": [0xC1, 3],
    "stx $": [0xC2, 3],
    "sty $": [0xC3, 3],
    "lhm a": [0xF2, 1],
    "lhm b": [0xF6, 1],
    "lhm y": [0xFE, 1],
    "lm a": [0xF3, 1],
    "lm b": [0xF7, 1],
    "sm a": [0x40, 1],
    "sm b": [0x41, 1],
    
    "cmp a a": [0xD0, 1],      #Compare A with A
    "cmp a b": [0xD1, 1],      #Compare A with B
    "cmp a x": [0xD2, 1],      #Compare A with X
    "cmp a y": [0xD3, 1],      #Compare A with Y

    "cmp b a": [0xD1, 1],      #Compare A with B
    "cmp b b": [0xD5, 1],      #Compare B with B
    "cmp b x": [0xD6, 1],      #Compare B with X
    "cmp b y": [0xD7, 1],      #Compare B with Y

    "cmp x a": [0xD2, 1],      #Compare A with X
    "cmp x b": [0xD6, 1],      #Compare B with X
    "cmp x x": [0xDA, 1],      #Compare X with X
    "cmp x y": [0xDB, 1],      #Compare X with Y

    "cmp y a": [0xD3, 1],      #Compare A with Y
    "cmp y b": [0xD7, 1],      #Compare B with Y
    "cmp y x": [0xDB, 1],      #Compare X with Y
    "cmp y y": [0xDF, 1],      #Compare Y with Y

    "cmp a #": [0xE0, 2],      #Compare A with #
    "cmp b #": [0xE1, 2],      #Compare B with #
    "cmp x #": [0xE2, 2],      #Compare X with #
    "cmp y #": [0xE3, 2],      #Compare Y with #
    
    "nand a, a": [0xA0, 1],    #NAND A with A
    "nand a, b": [0xA1, 1],    #NAND A with B
    "nand a, x": [0xA2, 1],    #NAND A with X
    "nand a, y": [0xA3, 1],    #NAND A with Y
    "nand b, a": [0xA4, 1],    #NAND B with A
    "nand b, b": [0xA5, 1],    #NAND B with B
    "nand b, x": [0xA6, 1],    #NAND B with X
    "nand b, y": [0xA7, 1],    #NAND B with Y
    "nand x, a": [0xA8, 1],    #NAND X with A
    "nand x, b": [0xA9, 1],    #NAND X with B
    "nand x, x": [0xAA, 1],    #NAND X with X
    "nand x, y": [0xAB, 1],    #NAND X with Y
    "nand y, a": [0xAC, 1],    #NAND Y with A
    "nand y, b": [0xAD, 1],    #NAND Y with B
    "nand y, x": [0xAE, 1],    #NAND Y with X
    "nand y, y": [0xAF, 1],    #NAND Y with Y
}
