
inc_dir = include_directories('.')

# Make this library usable from the system's
# package manager.
headers = [
    'bricks/algorithm.hpp',
    'bricks/charconv.hpp',
    'bricks/detail/contains.hpp',
    'bricks/detail/enumerate.hpp',
    'bricks/detail/filter.hpp',
    'bricks/detail/index_of.hpp',
    'bricks/detail/read_guard.hpp',
    'bricks/detail/reverse.hpp',
    'bricks/detail/write_guard.hpp',
    'bricks/detail/zip.hpp',
    'bricks/mutex.hpp',
    'bricks/ranges.hpp',
    'bricks/result.hpp',
    'bricks/rw_lock.hpp',
    'bricks/timer.hpp',
    'bricks/type_traits.hpp',
]

install_headers(headers, preserve_path: true)

# Make this library usable as a Meson subproject.
bricks_dep = declare_dependency(include_directories: inc_dir)
