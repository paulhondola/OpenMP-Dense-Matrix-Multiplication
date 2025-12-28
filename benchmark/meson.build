project('OpenMPDenseMatrixMultiplication', 'c',
  version : '1.0.0',
  default_options : ['warning_level=3', 'c_std=c99', 'optimization=3', 'b_lto=true'])

cc = meson.get_compiler('c')
m_dep = cc.find_library('m', required : false)
omp_dep = dependency('openmp')

# Common Source Files
common_src = files(
  'src/benchmark/benchmark.c',
  'src/loop_permutations/parallel/mm_parallel.c',
  'src/loop_permutations/serial/mm_serial.c',
  'src/matrix/matrix.c',
  'src/tiled/parallel/mm_tiled_parallel.c',
  'src/tiled/serial/mm_tiled_serial.c',
  'src/utils/utils.c'
)

inc_dir = include_directories('src')

# Executables

# Serial Loop
serial_loop_exe = executable('serial_loop',
  files('src/main/serial_loop.c') + common_src,
  include_directories : inc_dir,
  dependencies : [omp_dep, m_dep],
  install : true
)

# Parallel Loop
parallel_loop_exe = executable('parallel_loop',
  files('src/main/parallel_loop.c') + common_src,
  include_directories : inc_dir,
  dependencies : [omp_dep, m_dep],
  install : true
)

# Serial Parallel Scaling
scaling_exe = executable('serial_parallel_scaling',
  files('src/main/serial-parallel-scaling.c') + common_src,
  include_directories : inc_dir,
  dependencies : [omp_dep, m_dep],
  install : true
)

# Tiled
tiled_exe = executable('tiled',
  files('src/main/tiled.c') + common_src,
  include_directories : inc_dir,
  dependencies : [omp_dep, m_dep],
  install : true
)

# Run Targets

run_target('run_serial',
  command : [serial_loop_exe, 'O3'],
)

run_target('run_parallel',
  command : [parallel_loop_exe, 'O3'],
)

run_target('run_scaling',
  command : [scaling_exe, 'O3'],
)

run_target('run_tiled',
  command : [tiled_exe, 'O3'],
)
