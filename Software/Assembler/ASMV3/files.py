
from typing import List

def load_file(path: str) -> List[str]:
    # Load the file
    with open(path, "r") as file:
        content = file.readlines()
    return content
