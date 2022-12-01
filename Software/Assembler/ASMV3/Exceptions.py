
class InvalidInstructionException(Exception):
    def __init__(self, instruction, lineNum = -1):
        self.instruction = instruction
        self.lineNum = lineNum

    def __str__(self):
        return f"Invalid instruction{' on line ' + str(self.lineNum) if self.lineNum != -1 else ''}: '{self.instruction}'"
        