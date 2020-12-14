from operator import mul
from functools import reduce

def gcd(a, b):
    if b == 0:
        return (a, 1, 0)
    else:
        d, si, ti = gcd(b, a % b)
        return (d, ti, si - (a // b) * ti)

f = open('input','r').readlines()

schedule = [int(bus) if bus != "x" else None for bus in f[1].split(',')]

lcm = reduce(mul, (s for s in schedule if s is not None), 1)

time = 0

for offset, bus in enumerate(schedule):
    if bus is None: continue
    
    M = lcm // bus
    d, _, y = gcd(bus, M)

    if d != 1: raise ValueError("Not coprime")

    time = (time + ((bus - offset) % bus) * y * M) % lcm

print(time)
