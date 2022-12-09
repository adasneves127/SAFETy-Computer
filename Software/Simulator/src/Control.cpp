#include "Control.h"
#include <string.h>


controlUnit::controlUnit(Registers* _reg, ALU* _alu, Memory* _mem){
    //Gives the control unit access to the registers, ALU, and memory.
    this->_reg = _reg;
    this->_alu = _alu;
    this->_mem = _mem;
    instName = (char* )malloc(15 * sizeof(char));

    //Resets the control unit.
    this->reset();
}


void controlUnit::decode(uint8_t instruction){
    int rdNum = (instruction & 0b00001100) >> 2;
    int rsNum = (instruction & 0b00000011);
    this->RD = this->_reg->getRegister(rdNum);
    this->RS = this->_reg->getRegister(rsNum);
    this->GetOperands(instruction);
}
void controlUnit::GetOperands(uint8_t instruction){
    this->operands[0] = this->_mem->read(this->_mem->getPC() + 1);
    this->operands[1] = this->_mem->read(this->_mem->getPC() + 2);
}

void controlUnit::execute(uint8_t instruction){
    int upper = (instruction & 0b11110000) >> 4;
    int lower = (instruction & 0b00001111);

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
            break;
        case 0x03:
            //BRK
            strcpy(this->instName, "BRK");
            break;
        case 0x04:
            //CTN
            strcpy(this->instName, "CTN");
            break;
        case 0x05:
            //CLC
            strcpy(this->instName, "CLC");;
            break;
        case 0x06:
            //MUL
            strcpy(this->instName, "CLN");;
            break;
        case 0x07:
            //DIV
            strcpy(this->instName, "CLZ");;
            break;
        case 0x08:
            //AND
            strcpy(this->instName, "CLV");;
            break;
        case 0x0A:
            //CALL
            strcpy(this->instName, "CALL");;
            break;
        case 0x10:
        case 0x14:
        case 0x18:
        case 0x1C:
            //Rotate Left
            strcpy(this->instName, "ROL");;
            break;
        case 0x11:
        case 0x15:
        case 0x19:
        case 0x1D:
            //Rotate Right
            strcpy(this->instName, "ROR");;
            break;
        case 0x12:
        case 0x16:
        case 0x1A:
        case 0x1E:
            //Add Immediate
            strcpy(this->instName, "ADD");;
            break;
        case 0x13:
        case 0x17:
        case 0x1B:
        case 0x1F:
            //Subtract Immediate
            strcpy(this->instName, "SUB");;
            break;
        case 0x20:
        case 0x24:
        case 0x28:
        case 0x2C:
            //INC
            strcpy(this->instName, "INC");;
            break;
        case 0x21:
        case 0x25:
        case 0x29:
        case 0x2D:
            //DEC
            strcpy(this->instName, "DEC");;
            break;
        case 0x30:
        case 0x34:
        case 0x38:
        case 0x3C:
            //POP
            strcpy(this->instName, "POP");;
            break;
        case 0x31:
        case 0x35:
        case 0x39:
        case 0x3D:
            //TOP -- View the top of the stack. Does not pop.
            strcpy(this->instName, "TOP");;
            break;
        case 0x40:
        case 0x41:
            //Store Mem (xy indexed)
            strcpy(this->instName, "SM");;
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


        case 0xF0:
        case 0xF4:
        case 0xF8:
        case 0xFC:
            //Load Mem (Address is next two bytes)
            sprintf(this->instName, "LD%c $%04x" , this->RD->getName(), this->operands[0] << 8 | this->operands[1]);
            this->RD->set(this->_mem->read(this->operands[0] << 8 | this->operands[1]));
            break;
        case 0xF1:
        case 0xF5:
        case 0xF9:
        case 0xFD:
            //Load Immediate (Next byte)
            sprintf(this->instName, "LD%c #%04x" , this->RD->getName(), this->operands[0]);
            this->RD->set(this->operands[0]);
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
        }
    std::cout << "Instruction: " << this->instName << std::endl;
    _reg->printDebug();
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
void controlUnit::printDebug(){}

