#!/usr/bin/env python3

import numpy

LAYOUT = (
    ("latitude", "10.4f", "vlnzw2p2dnxyzltln.tbl.txt", 9, 1.0),
    ("longitude", "10.4f", "vlnzw2p2dnxyzltln.tbl.txt", 10, 1.0),
    ("elevation (m)", "15.1f", "vlnzw2p2dnxyzltln.tbl.txt", 8, -1000.0),
    ("density (kg/m**3)", "19.1f", "vlnzw2p2dnxyzltln.tbl.txt", 3, 1000.0),
    ("Vp (m/s)", "9.1f", "vlnzw2p2dnxyzltln.tbl.txt", 0, 1000.0),
    ("Vs (m/s)", "9.1f", "vlnzw2p2dnxyzltln.tbl.txt", 2, 1000.0),
    ("Qp", "7.1f", "Qpnzw2p3xyzltln.tbl.txt", 0, 1.0),
    ("Qs", "7.1f", "Qsnzw2p3xyzltln.tbl.txt", 0, 1.0),
)

header = "# "
for column in LAYOUT:
    fstr = "{:>%s}" % (column[1].split(".")[0])
    header += fstr.format(column[0])
header += "\n"

data = None
fmt = []
for name, fstr, filename, col, scale in LAYOUT:
    cdata = numpy.loadtxt(filename, usecols=(col,), skiprows=2) * scale
    fmt.append("%"+fstr)
    if not data is None:
        data = numpy.vstack((data, cdata))
    else:
        data = cdata
data = data.transpose()
        
with open("nz_cvm-2-3.txt", "w") as fout:
    fout.write(header)
    numpy.savetxt(fout, data, fmt=fmt)
