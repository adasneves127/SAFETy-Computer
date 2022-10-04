//
// Created by Alex Dasneves on 9/8/22.
//

// This will be the main file for the simulator

#pragma region Includes
#include <stdio.h>
#include <stdbool.h>
#pragma endregion

#pragma region Defines
// Define the maximum size of the memory
#define MAX_MEMORY_SIZE 0x80000000

// Define the maximum value of the program counter
#define MAX_PC_VAL 0xFFFF

// Define special locations in memory
#define IO_A 0xFFF0
#define KEYBOARD IO_A
#define IO_B 0xFFF1
#define LCD IO_B
#define IO_C 0xFFF2
#define IO_D 0xFFF3
#define IO_E 0xFFF4
#define IO_F 0xFFF5
#define IO_G 0xFFF6
#define IO_H 0xFFF7
#define VID_OUT 0xFFF8 // Video Output will now be handled by writing to this location, rather than indexing within memory.

#define VID_MODE 0xFFF9
#define MEMPAGEL 0xFFFA
#define MEMPAGEH 0xFFFB

#define REGA registers.A
#define REGB registers.B
#define REGX registers.X
#define REGY registers.Y
#define FLAGS registers.F
#define STCKPTR registers.SP
#define INSTR registers.IR
#define PROGCNTR registers.PC
#define STACKBASE 0x7F00

#pragma endregion

#pragma region Structs
// Define the registers
struct Registers{
    unsigned char* A;
    unsigned char* B;
    unsigned char* X;
    unsigned char* Y;
    unsigned char* F;
    unsigned char* SP;
    unsigned char* IR;
    unsigned short * PC;
};

struct Memory{
    unsigned char* RAM[0x8000];
    unsigned char* ROM[0xFF][0xFF][0x8000];
};
#pragma endregion

#pragma region Global Variables


struct Memory memory;
unsigned short PC = 0x0000;

struct Registers registers;
struct Memory mem;


unsigned char mpL = 0x00;
unsigned char mpH = 0x00;

bool debug = false;
bool halted = false;
bool brk = false;


#pragma endregion

#pragma region Function Prototypes
int main(int argc, char *argv[]);
void readFile(char *argv[]);
int doInstruction();
void readMemIncPC();
unsigned char* ReadNext();
void push(unsigned char* val);
void pop(unsigned char* val);
void top(unsigned char* val);
unsigned char Read(unsigned short addr);
void write(unsigned char* low, unsigned char* high, unsigned char* val);
void add(unsigned char* r1, unsigned char* r2);
void sub(unsigned char* r1, unsigned char* r2);
void cmp(unsigned char* r1, unsigned char* r2);
void nand(unsigned char* r1, unsigned char* r2);
void inc(unsigned char* r1);
void dec(unsigned char* r1);
void ROR(unsigned char* val);
void ROL(unsigned char* val);
void printDebug();
#pragma endregion

#pragma region Main Function
int main(int argc, char* argv[]) {
    // Check if the user has specified Debug Mode
    if(argc == 2){
        if(*argv[0] == 'd'){
            debug = true;
        }
    }


    // Check if the user has provided a file to run, if not, exit
    if(argc > 1){
        readFile(argv);
    }
    else{
        printf("No file specified!");
        return 0;
    }

    while(!halted && (PC <= 65535)) {
        int res = doInstruction();
        if (debug) {
            printDebug();
        }
        if(res == -1){
            printf("Program encountered an error!");
            return 0;
        }
    }

}

void printDebug(){

}

#pragma endregion


void readFile(char *argv[]){
    FILE* f=fopen(argv[1],"r");

    int i;
    //Fill memory with 0x00 (NOP)
    for(i=0; i<0xFFFF; i++)
    {
        (*memory.RAM)[i] = 0x00;
    }
    i=0x8000;



    //Read file into memory
    for(unsigned char memHigh = 0; memHigh < 0xFF; memHigh++){
        for(unsigned char memLow = 0; memLow < 0xFF; memLow++){
            for(i = 8000; i < 0xFFFF; i++){
                int c;
                fscanf(f,"%x\n",&c);
                (*memory.ROM)[memHigh][memLow][i] = c;
            }
        }
    }

    fclose(f);
}

int doInstruction(){
    //Fetch
    readMemIncPC();

    //Decode/Execute
    unsigned char opcode = *INSTR;



    switch(opcode){
        case 0x00:
            //NOP
            break;
        case 0x01:
            //RST
            break;
        case 0x02:
            //HLT
            break;
        case 0x03:
            //BRK
            break;
        case 0x04:
            //CTN
            break;
        case 0x05:
            //CLC
            (*FLAGS) &= 0b11111011;
            break;
        case 0x06:
            //CLN
            (*FLAGS) &= 0b11111101;
            break;
        case 0x07:
            //CLZ
            (*FLAGS) &= 0b11111110;
            break;
        case 0x08:
            //CLV
            (*FLAGS) &= 0b11110111;
            break;
        case 0x10:
            //ROL A
            (*REGA) = (*REGA) >> 1 | (((*REGA) << 7) & 0x80);
            break;
        case 0x11:
            //ROR A
            ROR(REGA);
            break;
        case 0x12:
            //ADD A IMM
            add(REGA, ReadNext());
            break;
        case 0x13:
            //SUB A IMM
            sub(REGA, ReadNext());
            break;
        case 0x14:
            //ROL B
            ROL(REGB);
            break;
        case 0x15:
            //ROR B
            ROR(REGB);
            break;
        case 0x16:
            //ADD B IMM
            add(REGB, ReadNext());
            break;
        case 0x17:
            //SUB B IMM
            sub(REGB, ReadNext());
            break;
        case 0x18:
            //ROL X
            ROL(REGX);
            break;
        case 0x19:
            //ROR X
            ROR(REGX);
            break;
        case 0x1A:
            //ADD X IMM
            add(REGX, ReadNext());
            break;
        case 0x1B:
            sub(REGX, ReadNext());
            break;
        case 0x1C:
            ROL(REGY);
            break;
        case 0x1D:
            ROR(REGY);
            break;
        case 0x1E:
            sub(REGY, ReadNext());
            break;
        case 0x1F:
            sub(REGY, ReadNext());
            break;
        case 0x20:
            inc(REGA);;
            break;
        case 0x21:
            dec(REGA);;
            break;
        case 0x24:
            inc(REGB);;
            break;
        case 0x25:
            dec(REGB);;
            break;
        case 0x28:
            inc(REGX);;
            break;
        case 0x29:
            dec(REGX);;
            break;
        case 0x2C:
            inc(REGY);;
            break;
        case 0x2D:
            dec(REGY);;
            break;
        case 0x30:
            pop(REGA);
            break;
        case 0x31:
            top(REGA);
            break;
        case 0x34:
            pop(REGB);
            break;
        case 0x35:
            top(REGB);
            break;
        case 0x38:
            pop(REGX);
            break;
        case 0x39:
            top(REGX);
            break;
        case 0x3C:
            pop(REGY);
            break;
        case 0x3D:
            top(REGY);
            break;
        case 0x40:
            write(REGX, REGY, REGA);
            break;
        case 0x41:
            write(REGX, REGY, REGB);
            break;
        case 0x61:
            (*REGB) = (*REGA);
            break;
        case 0x62:
            (*REGX) = (*REGA);
            break;
        case 0x63:
            (*REGY) = (*REGA);
            break;
        case 0x64:
            (*REGA) = (*REGB);
            break;
        case 0x66:
            (*REGX) = (*REGB);
            break;
        case 0x67:
            (*REGY) = (*REGB);
            break;
        case 0x68:
            (*REGA) = (*REGX);
            break;
        case 0x69:
            (*REGB) = (*REGX);
            break;
        case 0x6B:
            (*REGY) = (*REGX);
            break;
        case 0x6C:
            (*REGA) = (*REGY);
            break;
        case 0x6D:
            (*REGB) = (*REGY);
            break;
        case 0x6E:
            (*REGX) = (*REGY);
            break;
        case 0x70:
            //JMP
            break;
        case 0x71:
            //JSR
            break;
        case 0x72:
            //RET
            break;
        case 0x73:
            //JSE
            break;
        case 0x74:
            //JSN
            break;
        case 0x75:
            //BNE
            break;
        case 0x76:
            //BEQ
            break;
        case 0x80:
            add(REGA, REGA);
            break;
        case 0x81:
            add(REGA, REGB);
            break;
        case 0x82:
            add(REGA, REGX);
            break;
        case 0x83:
            add(REGA, REGY);
            break;
        case 0x84:
            add(REGB, REGA);
            break;
        case 0x85:
            add(REGB, REGB);
            break;
        case 0x86:
            add(REGB, REGX);
            break;
        case 0x87:
            add(REGB, REGY);
            break;
        case 0x88:
            add(REGX, REGA);
            break;
        case 0x89:
            add(REGX, REGB);
            break;
        case 0x8A:
            add(REGX, REGX);
            break;
        case 0x8B:
            add(REGX, REGY);
            break;
        case 0x8C:
            add(REGY, REGA);
            break;
        case 0x8D:
            add(REGY, REGB);
            break;
        case 0x8E:
            add(REGY, REGX);
            break;
        case 0x8F:
            add(REGY, REGY);
            break;
        case 0x90:
            sub(REGA, REGA);
            break;
        case 0x91:
            sub(REGA, REGB);
            break;
        case 0x92:
            sub(REGA, REGX);
            break;
        case 0x93:
            sub(REGA, REGY);
            break;
        case 0x94:
            sub(REGB, REGA);
            break;
        case 0x95:
            sub(REGB, REGB);
            break;
        case 0x96:
            sub(REGB, REGX);
            break;
        case 0x97:
            sub(REGB, REGY);
            break;
        case 0x98:
            sub(REGX, REGA);
            break;
        case 0x99:
            sub(REGX, REGB);
            break;
        case 0x9A:
            sub(REGX, REGX);
            break;
        case 0x9B:
            sub(REGX, REGY);
            break;
        case 0x9C:
            sub(REGY, REGA);
            break;
        case 0x9D:
            sub(REGY, REGB);
            break;
        case 0x9E:
            sub(REGY, REGX);
            break;
        case 0x9F:
            sub(REGY, REGY);
            break;
        case 0xB0:
            push(REGA);
            break;
        case 0xB1:
            push(REGB);
            break;
        case 0xB2:
            push(REGX);
            break;
        case 0xB3:
            push(REGY);
            break;
        case 0xC0:
            write(REGA, ReadNext(), ReadNext());
            break;
        case 0xC1:
            write(REGB, ReadNext(), ReadNext());
            break;
        case 0xC2:
            write(REGX, ReadNext(), ReadNext());
            break;
        case 0xC3:
            write(REGY, ReadNext(), ReadNext());
            break;
        case 0xA0:
            nand(REGA, REGA);
            break;
        case 0xA1:
            nand(REGA, REGB);
            break;
        case 0xA2:
            nand(REGA, REGX);
            break;
        case 0xA3:
            nand(REGA, REGY);
            break;
        case 0xA4:
            nand(REGB, REGA);
            break;
        case 0xA5:
            nand(REGB, REGB);
            break;
        case 0xA6:
            nand(REGB, REGX);
            break;
        case 0xA7:
            nand(REGB, REGY);
            break;
        case 0xA8:
            nand(REGX, REGA);
            break;
        case 0xA9:
            nand(REGX, REGB);
            break;
        case 0xAA:
            nand(REGX, REGX);
            break;
        case 0xAB:
            nand(REGX, REGY);
            break;
        case 0xAC:
            nand(REGY, REGA);
            break;
        case 0xAD:
            nand(REGY, REGB);
            break;
        case 0xAE:
            nand(REGY, REGX);
            break;
        case 0xAF:
            nand(REGY, REGY);
            break;
        case 0xB0:
            push(REGA);
            break;
        case 0xB1:
            push(REGB);
            break;
        case 0xB2:
            push(REGX);
            break;
        case 0xB3:
            push(REGY);
            break;
        case 0xC0:
            write(REGA, ReadNext(), ReadNext());
            break;
        case 0xC1:
            write(REGB, ReadNext(), ReadNext());
            break;
        case 0xC2:
            write(REGX, ReadNext(), ReadNext());
            break;
        case 0xC3:
            write(REGY, ReadNext(), ReadNext());
            break;
        case 0xE0:
            cmp(REGA, ReadNext());
            break;
        case 0xE1:
            cmp(REGB, ReadNext());
            break;
        case 0xE2:
            cmp(REGX, ReadNext());
            break;
        case 0xE3:
            cmp(REGY, ReadNext());
            break;

            case 0xF0:

                break;

        case 0xD0:
        case 0xD5:
        case 0xDA:
        case 0xDF:
            (*FLAGS) |= 0b00000001;
            break;
        default:
            printf("Invalid Opcode: %x", opcode);
            return -1;
    }
#pragma region Flag Check
    if(opcode >= 0x80 && opcode <= 0x83 || opcode >= 0x90 && opcode <= 0x93){
        if((*REGA) == 0) {
            *FLAGS &= 0b11111101;
            *FLAGS |= 0b00000001;

        }else{
            *FLAGS &= 0b11111110;
            *FLAGS |= 0b00000010;
        }
    }
    if(opcode >= 0x84 && opcode <= 0x87 || opcode >= 0x94 && opcode <= 0x97){
        if((*REGB) == 0) {
            *FLAGS &= 0b11111101;
            *FLAGS |= 0b00000001;

        }else{
            *FLAGS &= 0b11111110;
            *FLAGS |= 0b00000010;
        }
    }
    if(opcode >= 0x88 && opcode <= 0x8B || opcode >= 0x98 && opcode <= 0x9B){
        if((*REGX) == 0) {
            *FLAGS &= 0b11111101;
            *FLAGS |= 0b00000001;

        }else{
            *FLAGS &= 0b11111110;
            *FLAGS |= 0b00000010;
        }
    }
#pragma endregion
return 0;
}

unsigned char* ReadNext(){
    if(PC > 0x8000){
        return mem.ROM[mpH][mpL][(PC - 0x8000)];
    }
    else{
        return mem.RAM[PC];
    }
}

unsigned char Read(unsigned short addr){
    if(addr > 0x8000){
        return (*mem.ROM)[mpH][mpL][(addr - 0x8000)];
    }
    else{
        return *(mem.RAM)[PC];
    }
    return 0;
}

void readMemIncPC(){
    if(PC > 0x8000){
        registers.IR = mem.ROM[mpH][mpL][(PC - 0x8000)];
    }
    else{
        registers.IR = mem.RAM[PC];
    }

    registers.PC++;
}

void push(unsigned char* val){
    mem.RAM[*STCKPTR + STACKBASE] = val;
    (*STCKPTR)++;
}
void pop(unsigned char* val){
    (*STCKPTR)--;
    val = mem.RAM[*STCKPTR + STACKBASE];
}

void top(unsigned char* val){
    val = mem.RAM[*STCKPTR + STACKBASE];
}

void write(unsigned char* low, unsigned char* high, unsigned char* val){
    unsigned short addr = *low | (*high << 8);
}

void ROL(unsigned char* val){
    unsigned char temp = *val;
    (*val) = (*val) << 1 | (*val) >> 7;
}

void ROR(unsigned char* val){
    unsigned char temp = *val;
    (*val) = (*val) >> 1 | (*val) << 7;
}

void doReset(){
    unsigned char resetCode[] = {
            0x90,
            0x95,
            0x9A,
            0x9F,
            0x05,
            0x06,
            0x07,
            0x08
    };

    for(int i = 0; i < 8; i++){
        (*INSTR) = resetCode[i];
        doInstruction();
    }
}

void add(unsigned char* r1, unsigned char* r2){
    unsigned char temp = *r1 + *r2;
    if(temp == 0x00){
        *FLAGS &= 0b11111101;
        *FLAGS |= 0b00000001;
    }
    else{
        *FLAGS &= 0b11111110;
        *FLAGS |= 0b00000010;
    }
    *r1 = temp;

}

void sub(unsigned char* r1, unsigned char* r2){
    unsigned short temp = *r1 - *r2;
    if(temp == 0x00){
        *FLAGS &= 0b11111101;
        *FLAGS |= 0b00000001;
    }
    else{
        *FLAGS &= 0b11111110;
        *FLAGS |= 0b00000010;
    }
    *r1 = temp;
}

void nand(unsigned char* r1, unsigned char* r2){
    unsigned char temp = ~(*r1 & *r2);
    if(temp == 0x00){
        *FLAGS &= 0b11111101;
        *FLAGS |= 0b00000001;
    }
    else{
        *FLAGS &= 0b11111110;
        *FLAGS |= 0b00000010;
    }
    *r1 = temp;
}

void cmp(unsigned char* r1, unsigned char* r2){
    unsigned char temp = *r1 - *r2;
    if(temp == 0x00){
        *FLAGS &= 0b11111101;
        *FLAGS |= 0b00000001;
    }
    else{
        *FLAGS &= 0b11111110;
        *FLAGS |= 0b00000010;
    }
}

void inc(unsigned char* r1){
    unsigned char temp = *r1 + 1;
    if(temp == 0x00){
        *FLAGS &= 0b11111101;
        *FLAGS |= 0b00000001;
    }
    else{
        *FLAGS &= 0b11111110;
        *FLAGS |= 0b00000010;
    }
    *r1 = temp;
}

void dec(unsigned char* r1){
    unsigned char temp = *r1 - 1;
    if(temp == 0x00){
        *FLAGS &= 0b11111101;
        *FLAGS |= 0b00000001;
    }
    else{
        *FLAGS &= 0b11111110;
        *FLAGS |= 0b00000010;
    }
    *r1 = temp;
}

void loadMem(unsigned char* r1, unsigned short addr){
    r1 = Read(addr);
}

void loadImm(unsigned char* r1){
    r1 = ReadNext();
}