#!/usr/bin/env python3

import os, sys, subprocess
appname = os.path.basename(__file__)

def error_exit(msg):
    if msg == "": msg = "an error occurred"
    print("*** %s\nabort..." % msg)
    exit(1)

def usage_exit():
    print("*** usage :")
    print("%s /path/to/file.txt" % appname)
    print("%s -opt val" % appname)
    print("abort...")
    exit(1)

def runcmd(cmd):
    ret = subprocess.run(cmd, shell=True, capture_output=True)
    result = ret.stdout.decode()
    return (result, ret.returncode)

inpath="/var/lib/apt/lists/archive.raspberrypi.com_debian_dists_trixie_main_binary-arm64_Packages"

cmd = "dpkg -l"
result = runcmd(cmd)
output = result[0]

with open(inpath) as file:
    for line in file:
        line = line.rstrip()
        if not line.startswith("Package: "):
            continue
        if line.endswith("-dbgsym") or line.endswith("-dbg"):
            continue
        if len(line) < 10:
            continue
        line = line[9:]
        print(line)





