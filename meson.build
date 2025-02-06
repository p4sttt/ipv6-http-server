project('http_server', 'c')

incdir = include_directories('include')
args = ['-std=gnu89', '-pedantic']
src = [
    'src/main.c',
    'src/cmd_options.c',
    'src/server.c',
    'src/common.c'
]

executable('srv', src, include_directories: incdir, c_args: args)
