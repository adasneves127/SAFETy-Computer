import abc, re
from enum import Enum


from typing import List

import assemblyDef

from Exceptions import InvalidInstructionException, InvalidDataByteException


class LineIdentification(Enum):
    INST = 0
    LABEL = 1
    DATA = 2
    DIRECTIVE = 3
    INVALID = -1


class ISafetyAssembly(metaclass=abc.ABCMeta):
    @classmethod
    def __subclasshook__(cls, subclass):
        return (hasattr(subclass, 'assemble') and
                callable(subclass.assemble))
    
    @abc.abstractmethod
    def assemble(self):
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


class Section:
    def __init__(self, origin: int, content: List[Line]):
        self.origin = origin
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
            if split_line[0] in assemblyDef.reservedLabels.keys():
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
            


# TODO - .inc, .def allowed chars
class PreProcessorAssembly(ISafetyAssembly):
    def __init__(self, content: List[Line]):
        self.content = content

    def assemble(self):
        print("Starting PreProcessing")

        # .def
        defines = {}
        for line in self.content:
            if line.id == LineIdentification.DIRECTIVE:
                if line.content.startswith(".def"):
                    split_line = line.content.split(" ")
                    defines[split_line[1]] = split_line[2]
        
        self.filterDirective(".def")
        
        for line in self.content:
            for key, value in defines.items():
                line.content = re.sub(f"[ \\+\\-\\=\\(\\)]{key}[ \\n\\+\\-\\=\\(\\)]", f" {value} ", line.content)

        # .db
        for lineNum, line in enumerate(self.content):
            if line.id == LineIdentification.DIRECTIVE:
                if line.content.startswith(".db"):
                    split_line = line.content.split(" ")
                    if split_line[1].startswith("$"):
                        line.content = int(split_line[1][1:], 16)
                    elif split_line[1].startswith("#"):
                        line.content = int(split_line[1][1:])
                    else:
                        raise InvalidDataByteException("Invalid .db directive", lineNum)
                
        # .inc
        includedSection = Section(0, [])
        includes = {}
        
        for lineNum, line in enumerate(self.content):
            if line.id == LineIdentification.DIRECTIVE:
                if line.content.startswith(".inc"):
                    split_line = line.content.split(" ")
                    

        # .org
        sections = []
        currentSection = Section(0, [])

        for lineNum, line in enumerate(self.content):
            if line.id == LineIdentification.DIRECTIVE:
                if line.content.startswith(".org"):
                    split_line = line.content.split(" ")
                    split_first = split_line[1]
                    if split_first.startswith("$"):
                        origin = int(split_first[1:], 16)
                    elif split_first.startswith("#"):
                        origin = int(split_first[1:])
                    else:
                        raise InvalidDataByteException("Invalid .org directive", lineNum)
                    sections.append(currentSection)
                    currentSection = Section(origin, [])
                    
            else:
                currentSection.content.append(line)
        sections.append(currentSection)

        # .inc finalize
        sections.append(includedSection)

        sections.sort(key=lambda x: x.origin)

        return MacroFunctionResolver(sections)

    
    def filterDirective(self, directive: str):
        self.content = [x for x in self.content if not (x.id == LineIdentification.DIRECTIVE and x.content.startswith(directive))]



class MacroFunctionResolver(ISafetyAssembly):
    def __init__(self, content: List[Section]):
        self.content = content
    
    def assemble(self):

        pass



class ProcessorAssembly(ISafetyAssembly):
    def __init__(self, content: List[Section]):
        self.content = content

    def assemble(self):
        print("Starting Processing")
        for section in self.content:
            print(f"Processing section at {section.origin}")
            for line in section.content:
                if line.id == LineIdentification.INST:
                    print(line.content)
                elif line.id == LineIdentification.LABEL:
                    print(line.content)
                elif line.id == LineIdentification.DIRECTIVE:
                    print(line.content)
                elif line.id == LineIdentification.DATA:
                    print(line.content)
                else:
                    print("Invalid line")
        return None



class SafetyAssembler:
    def __init__(self, path):
        with open(path, "r") as file:
            content = file.readlines()

        self.assembly: ISafetyAssembly = PrePreProcessorAssembly(content)

    
    def nextStage(self):
        self.assembly = self.assembly.assemble()

    pass