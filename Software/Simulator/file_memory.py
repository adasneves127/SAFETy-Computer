class memory:
    def __init__(self, file_name, write_enable):
        if write_enable:
            self.stream = open(file_name, "+wb")
        else:
            self.stream = open(file_name, "rb")
            
    def read(self, addr = None):
        if addr is None:
            return self.stream.read(1)
        else:
            old_head = self.stream.tell()
            self.stream.seek(addr)
            data = self.stream.read()
            self.stream.seek(old_head)
            return data
    
    def write(self, data, addr = None):
        if addr is None:
            return self.stream.write(bytes(data))
        else:
            old_head = self.stream.tell()
            self.stream.seek(addr)
            data = self.stream.read()
            self.stream.seek(old_head)
            return data