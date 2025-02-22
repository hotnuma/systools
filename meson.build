project(
    'systools',
    ['c'],
    default_options: ['c_std=c99'],
    version : '1.0',
    license : 'GPL-2.0',
)

# build c code

c_args = [
    '-Wall',
    '-Wextra',
    '-O2',
    '-D_GNU_SOURCE',
]

app_deps = [
    dependency('gdlib'),
    dependency('tinyc'),
]

app_sources = [
    'src/restyle.c',
]

executable(
    'restyle',
    c_args: c_args,
    dependencies: app_deps,
    sources: app_sources,
    install: true,
)

# install sys tools in /usr/local/bin/

systools_files = [
    'src/appinst',
    'src/apt-upgrade',
    'src/builddep',
    'src/cbuild',
    'src/colortest',
    'src/cstyle',
    'src/extract.sh',
    'src/ff',
    'src/getcss',
    'src/git-check',
    'src/git-init',
    'src/git-push',
    'src/git-status',
    'src/imgtool',
    'src/mdconv',
    'src/pdfcount',
    'src/pdfmode',
    'src/psmem',
    'src/qninja',
    'src/rpimg',
    'src/testdd',
    'src/wallset',
    'src/yfname',
]

install_data(
    systools_files,
    install_dir : 'bin',
)


