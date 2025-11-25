#!/usr/bin/bash

infiles="/var/lib/apt/lists/archive.raspberrypi.com*arm64_Packages"
grep -h ^Package: $infiles | grep -v -- -dbgsym$ | grep -v -- -dbg$ | cut -d ' ' -f 2

