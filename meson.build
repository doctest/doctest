project('doctest', ['cpp'], version: '2.4.11')

doctest_dep = declare_dependency(include_directories: include_directories('doctest'))

if meson.version().version_compare('>=0.54.0')
    meson.override_dependency('doctest', doctest_dep)
endif
