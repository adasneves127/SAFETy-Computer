import abc
from enum import Enum


from typing import List

import assemblyDef

from Exceptions import InvalidInstructionException


class LineIdentification(Enum):
    INST = 0
    LABEL = 1
    DATA = 2
    DIRECTIVE = 3
    MACRO = 4
    INVALID = -1


class ISafetyAssembly(metaclass=abc.ABCMeta):
    @classmethod
    def __subclasshook__(cls, subclass):
        return (hasattr(subclass, 'assemble') and
                callable(subclass.assemble))
    
    @abc.abstractmethod
    def assemble(self, *args, **kwargs):
        """Assemble to next stage"""
        raise NotImplementedError

# .org
# .db $ = hex, # = dec
# .inc
# .def

class Line:
    def __init__(self, lineID: LineIdentification, content):
        self.id = lineID
        self.content = content

    
class PrePreProcessorAssembly(ISafetyAssembly):
    def __init__(self, content: List[str]):
        self.content = content
    
    def assemble(self):
        prepreprocessed = []
        for lineNum, instruction in enumerate(self.content):
            instruction = instruction.split(";")[0].strip()
            if not instruction:
                continue

            split_line = instruction.split(" ")
            if split_line[0] in assemblyDef.macros.keys():
                lineId = LineIdentification.MACRO
                prepreprocessed.append(Line(lineId, instruction))
            elif split_line[0] in assemblyDef.reservedLabels.keys():
                lineId = LineIdentification.LABEL
                prepreprocessed.append(Line(lineId, instruction))
            elif split_line[0].endswith(":"):
                lineId = LineIdentification.LABEL
                prepreprocessed.append(Line(lineId, instruction))
            elif split_line[0].startswith("."):
                lineId = LineIdentification.DIRECTIVE
                prepreprocessed.append(Line(lineId, instruction))
            elif any(x.startswith(split_line[0]) for x in assemblyDef.opCodes.keys()):
                lineId = LineIdentification.INST
                prepreprocessed.append(Line(lineId, instruction))
            else:
                raise InvalidInstructionException(instruction, lineNum)
        
        return PreProcessorAssembly(prepreprocessed)
            


# INST


class PreProcessorAssembly(ISafetyAssembly):
    def __init__(self, content: List[Line]):
        self.content = content

    def assemble(self, *args, **kwargs):
        print("Starting PreProcessing")

        # .org
        # .db 
        # .inc

        # .def
        # .equ




        

        return None
    




class SafetyAssembler:
    def __init__(self, path):
        with open(path, "r") as file:
            content = file.readlines()

        self.assembly: ISafetyAssembly = PrePreProcessorAssembly(content)

    
    def nextStage(self):
        self.assembly = self.assembly.assemble()

    pass