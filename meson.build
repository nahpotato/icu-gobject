project(
  'icu-gobject',
  'c',

  version         : '0.1.0',
  meson_version   : '>=0.58.0',
  default_options : ['warning_level=2', 'werror=false', 'c_std=gnu11'],
)

version_split = meson.project_version().split('-')[0].split('.')
icu_gobject_major_version = version_split[0].to_int()
icu_gobject_minor_version = version_split[1].to_int()
icu_gobject_micro_version = version_split[2].to_int()

api_version      = f'@icu_gobject_major_version@.0'
package_api_name = meson.project_name() + '-' + api_version

gnome = import('gnome')
pkg   = import('pkgconfig')

prefix     = get_option('prefix')
includedir = prefix / get_option('includedir')
libdir     = prefix / get_option('libdir')

c_args = [
  '-Wcast-align',
  '-Wdeclaration-after-statement',
  '-Werror=address',
  '-Wcast-align',
  '-Wdeclaration-after-statement',
  '-Werror=address',
  '-Werror=array-bounds',
  '-Werror=empty-body',
  '-Werror=implicit',
  '-Werror=implicit-function-declaration',
  '-Werror=incompatible-pointer-types',
  '-Werror=init-self',
  '-Werror=int-conversion',
  '-Werror=int-to-pointer-cast',
  '-Werror=main',
  '-Werror=misleading-indentation',
  '-Werror=missing-braces',
  '-Werror=missing-include-dirs',
  '-Werror=nonnull',
  '-Werror=overflow',
  '-Werror=parenthesis',
  '-Werror=pointer-arith',
  '-Werror=pointer-to-int-cast',
  '-Werror=redundant-decls',
  '-Werror=return-type',
  '-Werror=sequence-point',
  '-Werror=shadow',
  '-Werror=strict-prototypes',
  '-Werror=trigraphs',
  '-Werror=undef',
  '-Werror=write-strings',
  '-Wformat-nonliteral',
  '-Wignored-qualifiers',
  '-Wimplicit-function-declaration',
  '-Wlogical-op',
  '-Wmissing-declarations',
  '-Wmissing-format-attribute',
  '-Wmissing-include-dirs',
  '-Wmissing-noreturn',
  '-Wnested-externs',
  '-Wno-cast-function-type',
  '-Wno-dangling-pointer',
  '-Wno-missing-field-initializers',
  '-Wno-sign-compare',
  '-Wno-unused-parameter',
  '-Wold-style-definition',
  '-Wpointer-arith',
  '-Wredundant-decls',
  '-Wstrict-prototypes',
  '-Wswitch-default',
  '-Wswitch-enum',
  '-Wundef',
  '-Wuninitialized',
  '-Wunused',
  '-fno-strict-aliasing',
  '-Werror=format-security',
  '-Werror=format=2',
]

if get_option('buildtype') != 'plain'
  c_args += '-fstack-protector-strong'
endif

cc = meson.get_compiler('c')

add_project_arguments(
  '-DICU_GOBJECT_COMPILATION',
  cc.get_supported_arguments(c_args),

  language : 'c',
)

subdir('src')
