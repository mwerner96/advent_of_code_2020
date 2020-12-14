import re

class Mask:
    def __init__(self, mask="000000000000000000000000000000000000"):
        self.xormasks = []
        self.ormask = int(mask.replace('X', '0'), base=2)
        mask = mask.replace('1', '0')
        self.buildmasks(mask)

    def buildmasks(self, mask, idx=0):
        if idx == len(mask):
            self.xormasks.append(int(mask, base=2))
        elif mask[idx] == "X":
            self.buildmasks(mask[:idx] + "0" + mask[idx+1:], idx+1)
            self.buildmasks(mask[:idx] + "1" + mask[idx+1:], idx+1)
        else:
            self.buildmasks(mask, idx+1)

    def apply(self, address):
        return [(address ^ xormask) | self.ormask for xormask in self.xormasks]

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
            val = int(match.group(2))
            for idx in self.mask.apply(int(match.group(1))):
                self.memory[idx] = val
            return
        raise ValueError

    def sum_mem(self):
        return sum(self.memory.values())

interpreter = Interpreter()
for line in open("input", "r").readlines():
    interpreter.exec(line)

print(interpreter.sum_mem())
