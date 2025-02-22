<link href="style.css" rel="stylesheet"></link>

## Sys Tools

--

* Installation
	
    ```
    which meson || sudo apt install meson
    git clone https://github.com/hotnuma/systools.git
    cd systools
    ./install.sh
    ```

* Build hoedown
    
    ```
    which make || sudo apt install build-essential
    git clone https://github.com/hoedown/hoedown.git
    cd hoedown
    make && sudo make install
    ```


#### Programs

* appinst

    download and install binaries and scripts
    
    example :
    
    `sudo appinst "https://github.com/yt-dlp/yt-dlp/releases/latest/download/yt-dlp_linux"`

* apt-upgrade

    execute apt upgrade & apt full-upgrade
    
    check upgradable : `apt-upgrade check`

    full upgrade : `apt-upgrade full`
    
* builddep

    List dependencies needed to build a source package.
    
    The script runs `apt-cache showsrc` and reads
    the `Build-Depends:` informations.
    
    example : `builddep thunar`

* colortest

    show terminal colors

* cstyle
    
    style c code with allman settings
    
    `cstyle -r`

* extract.sh

    basic extract script

* ff
    
    wrapper to `find` with errors redirected to /dev/null

* getcss
    
    copy `$HOME/.config/style.css` into the current directory.
    this way the same css file can be used in any markdown project.

* git-check

    execute git-status on all subdirectories.
    
    in current dir : `git-check`
    
    in parent dir : `git-check ..`
    
* git-init

    `git-init home repos token`
    
* git-push

    `git-push "comment"`
    
* git-status

    test repository status

* mdconv

    convert md file to html
    
    convert one file : `mdconv file.md`
    
    in current directory : `mdconv`

* psmem
    
    a memory usage script forked from https://github.com/pixelb/ps_mem
    
    `sudo psmem`

* pdfcount
    
    pdf word count

* pdfmode
    
    disable fullscreen

* qninja
    
    ninja wrapper to use with QtCreator

* restyle

    change single line c comments into cpp comments
    
    `restyle file.c`

* rpimg

    copy an image file to a device
    
    `rpimg "my_file.img" /dev/sdc`

* testdd
    
    test drive speed

* wallset
    
    download random wallpaper and set it using hsetroot

* yfname
    
    rename all youtube downloaded files in the current directory
    in order to remove the "[xxxxxxxxxxx]" part at the end of the
    filename.
    
    in current dir : `yfname`
    in a specific directory : `yfname <directory>`


