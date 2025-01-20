project(
    'systools',
    ['c'],
    version : '1.0',
    license : 'GPL-2.0',
)

# install css file in $HOME/.config/

fs = import('fs')
config_dir = fs.expanduser('~/.config')
install_data(
    ['src/style.css',],
    install_dir : config_dir,
)

# install sys tools in /usr/local/bin/

systools_files = [
    'src/apt-upgrade',
    'src/builddep',
    'src/colortest',
    'src/cstyle',
    'src/extract.sh',
    'src/ff',
    'src/getcss',
    'src/git-check',
    'src/git-init',
    'src/git-push',
    'src/git-status',
    'src/mdconv',
    'src/pdfcount',
    'src/pdfmode',
    'src/qninja',
    'src/rpimg',
    'src/testdd',
    'src/yfname',
]

install_data(
    systools_files,
    install_dir : 'bin',
)

