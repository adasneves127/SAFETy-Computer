from multipledispatch import dispatch  # Function Overloading #type: ignore
from typing import List  # Type hints
from sys import stderr, argv
import json, re, os

includeFiles = []
headerLines = {}
headerPath = ""


class Label:
    def __init__(self, name: str, address: int):
        self.name = name
        self.addr = address & 0x7fff
    def __repr__(self):
        return f"Label {self.name} at Address {self.addr}"