project(
    'systools',
    ['c'],
    default_options: ['c_std=c99'],
    version : '1.0',
    license : 'GPL-2.0',
)

c_args = [
    '-Wall',
    '-Wextra',
    '-O2',
    '-D_GNU_SOURCE',
]

# git-history -----------------------------------------------------------------

ghist_deps = [
    dependency('tinyc'),
]

ghist_sources = [
    'src/git-history.c',
]

executable(
    'git-history',
    c_args: c_args,
    dependencies: ghist_deps,
    sources: ghist_sources,
    install: true,
)

# restyle ---------------------------------------------------------------------

rest_deps = [
    #dependency('gdlib'),
    dependency('tinyc'),
]

rest_sources = [
    'src/restyle.c',
]

executable(
    'restyle',
    c_args: c_args,
    dependencies: rest_deps,
    sources: rest_sources,
    install: true,
)

# scripts ---------------------------------------------------------------------

systools_files = [
    'src/appinst',
    'src/apt-upgrade',
    'src/bak',
    'src/unbak',
    'src/cbuild',
    'src/cdeps',
    'src/colortest',
    'src/cstyle',
    'src/extract.sh',
    'src/ff',
    'src/getcss',
    'src/git-check',
    'src/git-clone',
    'src/git-init',
    'src/git-push',
    'src/git-status',
    'src/imgtool',
    'src/mdconv',
    'src/pdfcount',
    'src/pdfmode',
    'src/psmem.py',
    'src/qninja',
    'src/radio',
    'src/rpimg',
    'src/testdd',
    'src/wallset',
    'src/yfname',
]

install_data(
    systools_files,
    install_dir : 'bin',
)


