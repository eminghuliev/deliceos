project('deliceos first stage kernel', 'cpp')

compiler_args = ['-m64',
  '-fno-stack-protector',
  '-ffreestanding',
  '-Os',
  '-s',
  '-fno-exceptions',
  '-fno-builtin-printf',
  '-fno-stack-protector',
  '-fno-asynchronous-unwind-tables',
  '-mno-red-zone',
  '-fno-pic',
  '-std=c++17']


incdir = include_directories('includes')
source_files = ['src/init.cc',
                'src/serial.cc',
                'src/apic.cc',
                'src/printf.cc',
                'src/interrupt.cc',
                'src/cpuid.cc',
                'src/paging.cc',
                'src/cpu.cc'
                ]

# Generate assembly
src_asm = [                                                                       
  'src/interrupt.s'                                                                  
]                                           
foreach src : src_asm                                                             
  output = meson.current_build_dir() + '/' + src.underscorify() + '.o'
  run_command(
    'nasm',
    '-f elf64',
    src,
    '-o',
    output
  )
endforeach

# Link arguments
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
    objects: output,
    include_directories : incdir
)
