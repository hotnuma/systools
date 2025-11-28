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

def israspi(pakname, repository):
    cmd = "apt-cache policy %s" % pakname
    result = runcmd(cmd)
    output = result[0]
    keep = False
    for line in output.splitlines():
        if line.startswith(" *** "):
            keep = True
        elif line.startswith("        "):
            if keep == True and repository in line:
                print(pakname)
                return
        elif line.startswith("     "):
            keep = False

repository = "archive.raspberrypi.com"
cmd = "dpkg -l"
result = runcmd(cmd)
output = result[0]

for line in output.splitlines():
    parts = line.split()
    if len(parts) < 2:
        continue
    if parts[0] != "ii":
        continue
    pakname = parts[1].split(":")[0]
    israspi(pakname, repository)

