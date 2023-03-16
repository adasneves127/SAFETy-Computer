
#include "Memory.h"
#include "ALU.h"
#include "Registers.h"
#include <vector>
#include <string>
class SAFETy{
    public:
        SAFETy(int settings, std::vector<std::string> argv);
        ~SAFETy();
        void run();
    private:
        int argVals;
        Memory _mem;
        ALU _alu;
        Registers _reg;
};