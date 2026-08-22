project(
  'bricks',
  'cpp',
  version: configure_file(
    input: 'version.txt',
    output: 'VERSION',
    copy: true,
  ),
  default_options: ['warning_level=3', 'cpp_std=c++23'],
)

# Add include folder with public headers
subdir('include')

# Add test folder with unit and integration tests
subdir('tests')

pkg_mod = import('pkgconfig')
pkg_mod.generate(
  name: 'bricks',
  description: 'Small useful blocks of code, to build bigger things',
  filebase: 'bricks',
  subdirs: 'bricks',
)
