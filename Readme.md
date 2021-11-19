# Misc Tools

* Installation
	
	apt :
	```
	sudo apt install build-essential git meson ninja-build
	```
	pacman :
	```
	sudo pacman -S base-devel git meson ninja
	```
	First utilisation of git :
	```
	git config --global user.name "John Doe"
	git config --global user.email johndoe@example.com
	```
	Download sources :
	```
	git clone https://github.com/hotnuma/systools.git
	cd systools
	./install.sh
	```
* src/git-init

    Initialise git repos :
	```
    git-init user repos token
	```
* src/git-push

    Push changes :
	```
    git-push "comment"
	```
* src/libshell

    Shell library.
    
* src/qninja

    Wrapper to use ninja with QtCreator.
    
* src/rpimg

    Write image to SD Card :
	```
    rpimg 'my_file.img' /dev/sdc
	```
* src/testsh

    Test script.
    
