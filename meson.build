project('doctest', ['cpp'], version: '2.5.1')

doctest_dep = declare_dependency(
    include_directories: include_directories('.', 'doctest', is_system: true),
)

if meson.version().version_compare('>=0.54.0')
    meson.override_dependency('doctest', doctest_dep)
endif
