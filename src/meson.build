icu_gobject_sources = [
  'icu-error.c',
  'icu-formatted-number.c',
  'icu-number-formatter.c',
  'icu-version.c',
]

icu_gobject_headers = [
  'icu-error.h',
  'icu-formatted-number.h',
  'icu-number-formatter.h',
]

icu_gobject_headers += configure_file(
  input         : 'icu-version.h.in',
  output        : 'icu-version.h',
  install       : true,
  install_dir   : includedir / package_api_name,
  configuration : {
    'VERSION'       : meson.project_version(),
    'MAJOR_VERSION' : icu_gobject_major_version,
    'MINOR_VERSION' : icu_gobject_minor_version,
    'MICRO_VERSION' : icu_gobject_micro_version,
  },
)

icu_gobject_sources += gnome.mkenums_simple(
  'icu-enum-types',

  decorator      : 'ICU_AVAILABLE_IN_ALL',
  header_prefix  : '#include "icu-version.h"',
  install_header : true,
  install_dir    : includedir / package_api_name,
  sources        : icu_gobject_headers,
)

icu_gobject_deps = [
  dependency('gio-2.0',  method : 'pkg-config'),
  dependency('icu-i18n', method : 'pkg-config'),
]

icu_gobject_lib = shared_library(
  package_api_name,
  icu_gobject_sources,

  dependencies : icu_gobject_deps,
  install      : true,
)

install_headers(['icu-gobject.h'],   subdir : package_api_name)
install_headers(icu_gobject_headers, subdir : package_api_name)

pkg.generate(
  description : 'A wrapper over the ICU4C library',
  filebase    : package_api_name,
  install_dir : libdir / 'pkgconfig',
  libraries   : icu_gobject_lib,
  name        : meson.project_name(),
  subdirs     : package_api_name,
  version     : meson.project_version(),
)

icu_gobject_gir = gnome.generate_gir(
  icu_gobject_lib,

  extra_args        : ['--warn-all'],
  sources           : icu_gobject_sources + icu_gobject_headers,
  nsversion         : api_version,
  namespace         : 'Icu',
  identifier_prefix : 'Icu',
  includes          : ['Gio-2.0'],
  header            : 'icu.h',
  install           : true,
  symbol_prefix     : 'icu',
)

icu_gobject_vapi = gnome.generate_vapi(
  package_api_name,

  install  : true,
  packages : ['gio-2.0'],
  sources  : icu_gobject_gir[0],
)