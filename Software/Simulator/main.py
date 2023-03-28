from typing import List
class Register:
    def __init__(self, name):
        self.name = name
        self.value = 0
    def get(self) -> int:
        return self.value

    def set(self, value) -> None:
        self.value = value
    
    def __add__(self, other: 'Register') -> int:
        self.value += other.value
        self.value &= 255
        return self.value
    
    def __sub__(self, other: 'Register') -> int:
        self.value -= other.value
        self.value &= 255
        return self.value
    
    def __and__(self, other: 'Register') -> int:
        self.value &= other.value
        self.value &= 255
        return self.value
    
    def __not__(self) -> int:
        self.value = -self.value
        self.value &= 255
        return self.value
    
    def __str__(self):
        return f"{self.name}: {self.value}"

    def __repr__(self):
        return str(self)

class ALU:
    def __init__(self):
        self.isZero = False
    
    def add(self, RD: Register, RS: Register) -> int:
        val = RD + RS
        self.isZero = (val == 0)  
        return val

    def sub(self, RD: Register, RS: Register) -> int:
        val = RD - RS
        self.isZero = (val == 0)
        return val
    
    def nand(self, RD: Register, RS: Register):
        _ = (RD & RS)
        val = not(RD)
        self.isZero = (val == 0)
        
        
        
    def __str__(self):
        return f"Zero Flag: {'Set' if self.isZero else 'Not Set'}"
    
    def __repr__(self):
        return str(self)
    


class SAFETy:
    def __init__(self):
        self.reset()
        self.alu = ALU()
        
        
    def reset(self):
        self.regs = [
            Register('A'),
            Register('B'),
            Register('X'),
            Register('Y')
        ]
        self.PC = self.IR = self.SP = 0
    
    def doAdd(self):
        self.alu.add(self.regs[0], self.regs[1])
        
    def __str__(self):
        return f"""SAFETy Instance
    ALU Data:
    {self.alu}
    
    Register Data:
    {[x for x in self.regs]}"""
        
if __name__ == "__main__":
    instances: List[SAFETy] = []
    instances.append(SAFETy())
    #instances[0].regs[1].set(10)
    instances[0].doAdd()
    print(instances[0])
    
    