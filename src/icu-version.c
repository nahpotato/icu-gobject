/*
 * Copyright 2022 Nahuel Gomez https://nahuelwexd.com
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "icu-version.h"

/**
 * icu_get_major_version:
 *
 * Returns the major version number of the icu-gobject library.
 */
guint
icu_get_major_version (void)
{
  return ICU_MAJOR_VERSION;
}

/**
 * icu_get_minor_version:
 *
 * Returns the minor version number of the icu-gobject library.
 */
guint
icu_get_minor_version (void)
{
  return ICU_MINOR_VERSION;
}

/**
 * icu_get_micro_version:
 *
 * Returns the micro version number of the icu-gobject library.
 */
guint
icu_get_micro_version (void)
{
  return ICU_MICRO_VERSION;
}
