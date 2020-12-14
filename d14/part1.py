import re

class Mask:
    def __init__(self, mask="000000000000000000000000000000000000"):
        self.andmask = int(mask.replace('X', '1'), base=2)
        self.ormask = int(mask.replace('X', '0'), base=2)

    def __str__(self):
        return "ANDMASK: " + bin(self.andmask) + " ORMASK: " + bin(self.ormask)

    def apply(self, value):
        value &= self.andmask
        value |= self.ormask
        return value

class Interpreter:
    def __init__(self):
        self.memory = dict()
        self.mask = Mask()
        self.re_mask = re.compile(r"mask = (.*)")
        self.re_assign = re.compile(r"mem\[(.*)\] = (.*)")

    def exec(self, command):
        match = self.re_mask.match(command)
        if match:
            self.mask = Mask(match.group(1))
            return
        match = self.re_assign.match(command)
        if match:
            idx = match.group(1)
            val = int(match.group(2))
            self.memory[idx] = self.mask.apply(val)
            return
        raise ValueError

    def sum_mem(self):
        return sum(self.memory.values())

interpreter = Interpreter()
for line in open("input", "r").readlines():
    interpreter.exec(line)

print(interpreter.sum_mem())
