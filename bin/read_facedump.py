import re

FILENAME = "facedump.txt"

with open(FILENAME, "r") as fin:
    lines = fin.readlines()

for iline, line in enumerate(lines):
    if line.startswith("Fault Block Names"):
        read_fault_blocks(lines, iline+1)
    if line.startswith("Zone Names"):
        read_zones(lines, iline+1)

def read_fault_blocks(lines, start):
    format = 
    for line in lines[start:]:
        
        if not line.strip():
            break
        
