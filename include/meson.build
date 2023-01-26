
inc_dir = include_directories('.')

# Make this library usable from the system's
# package manager.
headers = [
    'bricks/algorithm.hpp',
    'bricks/charconv.hpp',
    'bricks/concurrent.hpp',
    'bricks/detail/contains.hpp',
    'bricks/detail/index_of.hpp',
    'bricks/enumerate.hpp',
    'bricks/result.hpp',
    'bricks/reverse.hpp',
    'bricks/timer.hpp',
    'bricks/type_traits.hpp',
    'bricks/zip.hpp',
]

install_headers(headers, preserve_path: true)

# Make this library usable as a Meson subproject.
bricks_dep = declare_dependency(include_directories: inc_dir)
