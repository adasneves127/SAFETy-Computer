#include "SAFETy.h"

SAFETy::SAFETy(int settings, std::vector<std::string> argv){
    argVals = settings;
    if(argVals & 0x08){
        _mem.fill(0x00);
    }else{
        _mem.init(argv[0], argv[1]);
    }
}