project('deliceos bootloader', 'c')
src_asm = [
  'bootloader.s'
]
foreach src : src_asm
  output = './' + 'delice_boot.bin'
  run_command(
    'nasm',
    '-f bin',
    src,
    '-o',
    output
  )
endforeach

