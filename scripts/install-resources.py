#!/usr/bin/env python

import shutil, errno, sys

print(sys.argv)

def copyDirectory(source, dest):
    try:
        try:
            shutil.rmtree(dest)
        except:
            pass
        shutil.copytree(source, dest)
    except OSError as exc:
        if exc.errno == errno.ENOTDIR:
            shutil.copy(source, dest)
        else: raise

if len(sys.argv) != 3:
    print("Incorrect number of args")
    sys.exit(1)

try:
    copyDirectory(sys.argv[1], sys.argv[2])
except Exception as err:
    print(err)
    print("Failed to copy directory")
    sys.exit(1)