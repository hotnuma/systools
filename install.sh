#!/usr/bin/bash

meson setup build -Dbuildtype=plain
ninja -C build
sudo ninja -C build install


