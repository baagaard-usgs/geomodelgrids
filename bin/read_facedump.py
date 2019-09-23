import re

FILENAME = "facedump.txt"

def read_names(lines, start):
    id_mapping = {}
    pattern = r"^\[([0-9]+)\]\s([\S]+.+)"
    for line in lines[start:]:
        match = re.search(pattern, line.strip())
        if match and len(match.groups()) == 2:
            groups = match.groups()
            id_mapping[groups[1]] = groups[0]
        else:
            break
    return id_mapping


with open(FILENAME, "r") as fin:
    lines = fin.readlines()

for iline, line in enumerate(lines):
    if line.startswith("Fault Block Names"):
        fault_blocks = read_names(lines, iline+1)
    if line.startswith("Zone Names"):
        zones = read_names(lines, iline+1)

print(fault_blocks)
print(zones)
