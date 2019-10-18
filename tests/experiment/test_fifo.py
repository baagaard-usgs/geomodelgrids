#!/usr/bin/env python3

import os
import numpy
import threading

FILENAME_IN = "test_in.txt"
FILENAME_OUT = "test_out.txt"

def writer(data):
    print("Starting writing")
    with open(FILENAME_IN, "w") as fout:
        numpy.savetxt(fout, data)
    print("Finished writing")
    return

def processor():
    print("Starting processing")
    with open(FILENAME_IN, "r") as fin:
        data = numpy.loadtxt(fin)
    with open(FILENAME_OUT, "w") as fout:
        numpy.savetxt(fout, data)
    print("Finished processing")
    return
              
def reader(result):
    print("Starting reading")
    with open(FILENAME_OUT, "r") as fin:
        data = numpy.loadtxt(fin)
    result["data"] = data
    print("Finished reading")
    return

def mkfifo(filename):
    if os.path.exists(filename):
        os.unlink(filename)
    os.mkfifo(filename)
    return

mkfifo(FILENAME_IN)
mkfifo(FILENAME_OUT)

twrite = threading.Thread(target=writer, args=[numpy.array([1,2,3], dtype=numpy.float)])
twrite.start()

tprocess = threading.Thread(target=processor)
tprocess.start()

result = {}
tread = threading.Thread(target=reader, args=[result])
tread.start()

twrite.join()
tprocess.join()
tread.join()

print("  Result {}".format(result["data"]))

os.unlink(FILENAME_IN)
os.unlink(FILENAME_OUT)
