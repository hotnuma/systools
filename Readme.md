# Sys Tools

* Installation
	
    ```
    git clone https://github.com/hotnuma/systools.git
    cd systools
    ./install.sh
    ```

* Build hoedown
    
    ```
    git clone https://github.com/hoedown/hoedown.git
    cd hoedown
    make && sudo make install
    ```

* apt-update

    Execute apt upgrade & apt full-upgrade
    
* conf-reload

    Reload xfconf daemon
    
* git-check

    Execute git-status on all subdirectories
    
* git-init

    ```
    git-init home repos token
    ```
* git-push

    Push local changes to git
    ```
    git-push "comment"
    ```
* git-status

    Test repository status
    
* libshell

    Shell library.
    
* mdconv

    Convert md file to html.
    ```
    mdconv file.md
    ```
    Convert all md files in current directory.
    ```
    mdconv
    ```
* qninja

    Ninja wrapper to use with QtCreator.
    
* rpimg

    Write an img file to SD Card
    ```
    rpimg 'my_file.img' /dev/sdc
    ```
