#!/usr/bin/bash

ALL=0

while [[ $# > 0 ]]; do
    key="$1"
    case $key in
        all)
        ALL=1
        shift
        ;;
        *)
        shift
        ;;
    esac
done

meson build -Dbuildtype=plain
ninja -C build
sudo ninja -C build install

if [[ $ALL != 1 ]]; then
    exit 0
fi

# Install libraries ----------------------------------------------------
#
dest=~/DevFiles/libtinycpp
if [[ ! -d $dest ]]; then
	cd ~/DevFiles
	git clone https://github.com/hotnuma/libtinycpp.git
	cd $dest
	./install.sh
fi

dest=~/DevFiles/libtinyui
if [[ ! -d $dest ]]; then
	cd ~/DevFiles
	git clone https://github.com/hotnuma/libtinyui.git
	cd $dest
	./install.sh
fi

# Install tools --------------------------------------------------------
#
dest=~/DevFiles/cpuload
if [[ ! -d $dest ]]; then
	cd ~/DevFiles
	git clone https://github.com/hotnuma/cpuload.git
	cd $dest
	./install.sh
fi

dest=~/DevFiles/firebook
if [[ ! -d $dest ]]; then
	cd ~/DevFiles
	git clone https://github.com/hotnuma/firebook.git
	cd $dest
	./install.sh
fi

dest=~/DevFiles/hoedown
if [[ ! -d $dest ]]; then
	cd ~/DevFiles
	git clone https://github.com/hoedown/hoedown.git
	cd $dest
	make && sudo make install
fi

dest=~/DevFiles/memload
if [[ ! -d $dest ]]; then
	cd ~/DevFiles
	git clone https://github.com/hotnuma/memload.git
	cd $dest
	./install.sh
fi

dest=~/DevFiles/mpvcmd
if [[ ! -d $dest ]]; then
	cd ~/DevFiles
	git clone https://github.com/hotnuma/mpvcmd.git
	cd $dest
	./install.sh
fi

dest=~/DevFiles/prcquery
if [[ ! -d $dest ]]; then
	cd ~/DevFiles
	git clone https://github.com/hotnuma/prcquery.git
	cd $dest
	./install.sh
fi

dest=~/DevFiles/prcrun
if [[ ! -d $dest ]]; then
	cd ~/DevFiles
	git clone https://github.com/hotnuma/prcrun.git
	cd $dest
	./install.sh
fi

dest=~/DevFiles/svcquery
if [[ ! -d $dest ]]; then
	cd ~/DevFiles
	git clone https://github.com/hotnuma/svcquery.git
	cd $dest
	./install.sh
fi

dest=~/DevFiles/syslog
if [[ ! -d $dest ]]; then
	cd ~/DevFiles
	git clone https://github.com/hotnuma/syslog.git
	cd $dest
	./install.sh
fi


