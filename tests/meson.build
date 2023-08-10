doctest_dep = dependency('doctest')
threads_dep = dependency('threads')

sources = [
    'algorithm_test.cpp',
    'charconv_test.cpp',
    'contains_test.cpp',
    'enumerate_test.cpp',
    'filter_test.cpp',
    'handle_test.cpp',
    'index_of_test.cpp',
    'main.cpp',
    'mutex_test.cpp',
    'result_test.cpp',
    'reverse_test.cpp',
    'rw_lock_test.cpp',
    'timer_test.cpp',
    'type_traits_test.cpp',
    'zip_test.cpp',
]

test_exe = executable(
    'bricks_test',
    sources,
    dependencies: [bricks_dep, doctest_dep, threads_dep],
)

test('bricks_test', test_exe)
