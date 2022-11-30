import abc
from enum import Enum


from typing import List

import assemblyDef


class LineIdentification(Enum):
    INST = 0
    LABEL = 1
    DATA = 2
    DIRECTIVE = 3
    FUNC = 4
    MACRO = 5


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
        for instruction in self.content:
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





class PreProcessorAssembly(ISafetyAssembly):
    def __init__(self, content):
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
        self.assembly = None


    pass