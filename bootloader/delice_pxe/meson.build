project('deliceos first stage kernel', 'cpp')

compiler_args = ['-m64',
  '-fno-stack-protector',
  '-ffreestanding',
  '-fno-exceptions',
  '-fno-stack-protector',
  '-fno-asynchronous-unwind-tables',
  '-mno-red-zone',
  '-fno-pic',
  '-std=c++17']


incdir = include_directories('includes')
source_files = ['src/init.cc',
                'src/serial.cc',
                'src/printf.cc'
                ]

lnk_args = ['-static',
    '-m64',
    '-lgcc',
    '-nostdlib']
lnk_args+='-Wl,--build-id=none,-T'+ meson.current_source_dir() / 'linker.ld'



executable(
    'pxe_boot.bin',
    source_files,
    cpp_args: compiler_args,
    link_args: lnk_args,
    include_directories : incdir
)
