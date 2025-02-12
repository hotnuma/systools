#!/usr/bin/bash

meson setup build -Dbuildtype=plain
sudo meson install -C build


