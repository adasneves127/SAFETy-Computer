#!/usr/bin/env python3
import sys

from typing import List


from files import load_file


def main(argc: int, argv: List[str]) -> None:
    # Load the file using command line arguments
    content = load_file(argv[1])

    


    # Resolve preprocessor directives

    # Resolve macros

    # Resolve Functions
    
    # Resolve labels



    
    pass


if __name__ == "__main__":
    main(len(sys.argv), sys.argv)
