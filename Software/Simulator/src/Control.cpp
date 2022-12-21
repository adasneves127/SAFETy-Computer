#include "Control.h"


void controlUnit::run(){
    while(!(flags & 0b10000000)){
        uint8_t inst = _mem->nextIns();
        this->decode(inst);
        this->execute(inst);
    }
}

controlUnit::controlUnit(Registers* _reg, ALU* _alu, Memory* _mem){
    //Gives the control unit access to the registers, ALU, and memory.
    this->_reg = _reg;
    this->_alu = _alu;
    this->_mem = _mem;
    this->flagsptr = &flags;
    _alu->setFlags(this->flagsptr);

    this->flags = 0;
    instName = (char* )malloc(15 * sizeof(char));

    //Resets the control unit.
    this->reset();
}


void controlUnit::decode(uint8_t instruction){
    int rdNum = (instruction & 0b00001100) >> 2;
    int rsNum = (instruction & 0b00000011);
    this->RD = this->_reg->getRegister(rdNum);
    this->RS = this->_reg->getRegister(rsNum);
    this->opcode = instruction;
    this->GetOperands(instruction);
}
void controlUnit::GetOperands(uint8_t instruction){
    this->operands[0] = this->_mem->read(this->_mem->getPC() + 1);
    this->operands[1] = this->_mem->read(this->_mem->getPC() + 2);
}

void controlUnit::execute(uint8_t instruction){
    int upper = (instruction & 0b11110000) >> 4;
    int lower = (instruction & 0b00001111);
    int instructionLen = 0;

    switch(instruction){
        case 0x00:
            //NOP
            strcpy(this->instName, "NOP");
            break;
        case 0x01:
            //RST
            strcpy(this->instName, "RST");
            _mem ->reset();
            _alu->reset();
            _reg->reset();
            break;
        case 0x02:
            //HLT
            strcpy(this->instName, "HLT");
            this->flags |= 0b10000000;
            break;
        case 0x03:
            //BRK
            strcpy(this->instName, "BRK");
            this->flags |= 0b01000000;
            break;
        case 0x04:
            //CTN
            strcpy(this->instName, "CTN");
            this->flags &= 0b10111111;
            break;
        case 0x05:
            //CLC
            strcpy(this->instName, "CLC");
            this->flags &= 0b11111110;
            break;
        case 0x06:
            //MUL
            strcpy(this->instName, "CLN");
            this->flags &= 0b11111101;
            break;
        case 0x07:
            //DIV
            strcpy(this->instName, "CLZ");
            this->flags |= 0b00000010;
            break;
        case 0x08:
            //AND
            strcpy(this->instName, "CLV");
            this->flags &= 0b11111011;
            break;
        case 0x0A:
            //CALL
            strcpy(this->instName, "CALL");
            instructionLen = 2;
            break;
        case 0x10:
        case 0x14:
        case 0x18:
        case 0x1C:
            //Rotate Left
            strcpy(this->instName, "ROL");
            break;
        case 0x11:
        case 0x15:
        case 0x19:
        case 0x1D:
            //Rotate Right
            strcpy(this->instName, "ROR");
            break;
        case 0x12:
        case 0x16:
        case 0x1A:
        case 0x1E:
            //Add Immediate
            strcpy(this->instName, "ADD");
            this->_alu->add(this->RD, operands[0]);
            instructionLen = 1;
            _mem->nextIns();
            break;
        case 0x13:
        case 0x17:
        case 0x1B:
        case 0x1F:
            //Subtract Immediate
            strcpy(this->instName, "SUB");
            this->_alu->sub(this->RD, operands[0]);
            instructionLen = 1;
            _mem->nextIns();
            break;
        case 0x20:
        case 0x24:
        case 0x28:
        case 0x2C:
            //INC
            strcpy(this->instName, "INC");
            this->_alu->add(this->RD, 1);
            break;
        case 0x21:
        case 0x25:
        case 0x29:
        case 0x2D:
            //DEC
            strcpy(this->instName, "DEC");
            this->_alu->add(this->RD, -1);
            break;
        case 0x30:
        case 0x34:
        case 0x38:
        case 0x3C:
            //POP
            strcpy(this->instName, "POP");
            this->_mem->pop(this->RD);
            break;
        case 0x31:
        case 0x35:
        case 0x39:
        case 0x3D:
            //TOP -- View the top of the stack. Does not pop.
            strcpy(this->instName, "TOP");
            this->_mem->top(this->RD);
            break;
        case 0x40:
        case 0x41:
            //Store Mem (xy indexed)
            strcpy(this->instName, "SM");
            break;
        case 0x60:
        case 0x61:
        case 0x62:
        case 0x63:
        case 0x64:
        case 0x65:
        case 0x66:
        case 0x67:
        case 0x68:
        case 0x69:
        case 0x6A:
        case 0x6B:
        case 0x6C:
        case 0x6D:
        case 0x6E:
        case 0x6F:
            // Transfer Registers
            sprintf(this->instName, "T%c%c", this->RD->getName(), this->RS->getName());
            this-> RS->set(this->RD->get());
            break;

        case 0x70:
            sprintf(this->instName, "JMP %04x", this->operands[0] << 8 | this->operands[1]);
            this->_mem->jump(this->operands[0] << 8 | this->operands[1]);
            break;

        case 0x80:
        case 0x81:
        case 0x82:
        case 0x83:
        case 0x84:
        case 0x85:
        case 0x86:
        case 0x87:
        case 0x88:
        case 0x89:
        case 0x8A:
        case 0x8B:
        case 0x8C:
        case 0x8D:
        case 0x8E:
        case 0x8F:
            //Add Registers
            sprintf(this->instName, "ADD %c %c", this->RD->getName(), this->RS->getName());
            this->_alu->add(this->RD, this->RS);
            break;

        case 0x90:
        case 0x91:
        case 0x92:
        case 0x93:
        case 0x94:
        case 0x95:
        case 0x96:
        case 0x97:
        case 0x98:
        case 0x99:
        case 0x9A:
        case 0x9B:
        case 0x9C:
        case 0x9D:
        case 0x9E:
        case 0x9F:
            //Sub Registers
            sprintf(this->instName, "SUB %c %c", this->RD->getName(), this->RS->getName());
            this->_alu->sub(this->RD, this->RS);
            break;

        case 0xA0:
        case 0xA1:
        case 0xA2:
        case 0xA3:
        case 0xA4:
        case 0xA5:
        case 0xA6:
        case 0xA7:
        case 0xA8:
        case 0xA9:
        case 0xAA:
        case 0xAB:
        case 0xAC:
        case 0xAD:
        case 0xAE:
        case 0xAF:
            //NAND Registers
            sprintf(this->instName, "AND %c %c", this->RD->getName(), this->RS->getName());
            this->_alu->nand(this->RD, this->RS);
            break;
        case 0xB0:
        case 0xB1:
        case 0xB2:
        case 0xB3:
            //PUSH Registers
            sprintf(this->instName, "PUSH %c", this->RD->getName());
            this->_mem->push(this->RD);
            break;
        case 0xC0:
        case 0xC1:
        case 0xC2:
        case 0xC3:
            //Store Register
            sprintf(this->instName, "ST%c $%04x", this->RD->getName(), (operands[0] << 8) | operands[1]);
            this->_mem->store((operands[0] << 8) | operands[1], this->RD);
            instructionLen = 2;
        break;

        case 0xD0:
        case 0xD1:
        case 0xD2:
        case 0xD3:
        case 0xD4:
        case 0xD5:
        case 0xD6:
        case 0xD7:
        case 0xD8:
        case 0xD9:
        case 0xDA:
        case 0xDB:
        case 0xDC:
        case 0xDD:
        case 0xDE:
        case 0xDF:
            //Compare Registers
            sprintf(this->instName, "CMP %c %c", this->RD->getName(), this->RS->getName());
            this->_alu->cmp(this->RD, this->RS);
            break;
        case 0xE0:
        case 0xE4:
        case 0xE8:
        case 0xEC:
            //CMP Immediate
            sprintf(this->instName, "CMP %c %x", this->RD->getName(), this->operands[0]);
            this->_alu->cmp(this->RD, this->operands[0]);
            break;
        case 0xF0:
        case 0xF4:
        case 0xF8:
        case 0xFC:
            //Load Mem (Address is next two bytes)
            sprintf(this->instName, "LD%c $%04x" , this->RD->getName(), this->operands[0] << 8 | this->operands[1]);
            this->RD->set(this->_mem->read(this->operands[0] << 8 | this->operands[1]));
            instructionLen = 2;
            break;
        case 0xF1:
        case 0xF5:
        case 0xF9:
        case 0xFD:
            //Load Immediate (Next byte)
            sprintf(this->instName, "LD%c #%02x" , this->RD->getName(), this->operands[0]);
            this->RD->set(this->operands[0]);
            instructionLen = 1;
            break;
        case 0xF2:
        case 0xF6:
        case 0xFE:
            //Load Mem (Address is X indexed)
            sprintf(this->instName, "LIM %c, x" , this->RD->getName());
            this->RD->set(this->_mem->read(this->_reg->get(3) + 0xff00));
            break;
        case 0xF3:
        case 0xF7:
            //LM (Address is XY indexed)
            sprintf(this->instName, "LM %c, %02x%02x", this->RD->getName(), this->_reg->get(2), this->_reg->get(3));
            this->RD->set(this->_mem->read(this->_reg->get(2) << 8 | this->_reg->get(3)));
            break;
        default:
            printf("Unknown opcode: %02x", this->opcode);
            return;
        }
    this->_mem->jump(this->_mem->getPC() + instructionLen);
}

void controlUnit::reset(){
    //Resets the control unit.
    this->flags = 0;
    this->opcode = 0;
    this->RD = nullptr;
    this->RS = nullptr;
    this->operands[0] = -1;
    this->operands[1] = -1;
}
void controlUnit::printDebug(){
    printf("%s  \t IR: %02x\t", this->instName, this->opcode);
    printf("PC: %04x\n", this->_mem->getPC());
}

