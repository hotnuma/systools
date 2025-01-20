# Sys Tools

--

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


#### Programs

* apt-upgrade

    execute apt upgrade & apt full-upgrade

* builddep

    build dependencies

* colortest

    show terminal colors

* cstyle

* extract.sh

    basic extract script

* ff

* getcss
    
    copy `$HOME/.config/style.css` in the current directory.
    this way the same css file can be used in any markdown project.

* git-check

    execute git-status on all subdirectories.
    
    on current dir : `git-check`
    
    on parent dir : `git-check ..`
    
* git-init

    `git-init home repos token`
    
* git-push

    `git-push "comment"`
    
* git-status

    test repository status

* mdconv

    convert md file to html
    
    convert one file : `mdconv file.md`
    
    parse current directory : `mdconv`

* pdfcount

* pdfmode

* qninja
    
    ninja wrapper to use with QtCreator

* rpimg

    `rpimg "my_file.img" /dev/sdc`

* style.css
    
    user css file for markdown projects,
    it will be installed in `$HOME/.config/style.css`,
    then the `getcss` can be used to copy the css file
    into the current directory.

* testdd

* yfname


