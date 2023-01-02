#!/usr/bin/env python3
import sys

from typing import List

import assembler

from files import load_file


def main(argc: int, argv: List[str]) -> None:
    # Load the file using command line arguments

    asm = assembler.SafetyAssembler(argv[1])

    # Resolve preprocessor directives
    asm.nextStage()

    # Resolve macros
    asm.nextStage()

    # Resolve Functions
    asm.nextStage()

    # Resolve labels
    asm.nextStage()

    

    



if __name__ == "__main__":
    main(len(sys.argv), sys.argv)
