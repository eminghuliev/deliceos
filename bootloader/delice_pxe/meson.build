project('deliceos', 'c')

compiler_args = ['-m32',
  '-march=i386',
  '-fno-stack-protector',
  '-ffreestanding',
  '-fno-stack-protector',
  '-fno-asynchronous-unwind-tables',
  '-mno-red-zone',
  '-fno-pic']

lnk_args = ['-static',
    '-m32',
    '-s',
    '-nostdlib']
lnk_args+='-Wl,--build-id=none,-T'+ meson.current_source_dir() / 'linker.ld'

executable(
    'deliceos.elf',
    files('pxe_loader.c', 'test.c'),
    c_args: compiler_args,
    link_args: lnk_args,
)
