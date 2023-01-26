/*
 * Copyright 2023 Nahuel Gomez https://nahuelwexd.com
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#pragma once

#if !defined(_ICU_GOBJECT_INSIDE) && !defined(ICU_GOBJECT_COMPILATION)
#  error "Only <icu-gobject.h> can be included directly"
#endif

#include <glib-object.h>
#include "icu-version.h"

G_BEGIN_DECLS

typedef enum {
  ICU_NUM_INTEGER_FIELD,
  ICU_NUM_FRACTION_FIELD,
  ICU_NUM_DECIMAL_SEPARATOR_FIELD,
  ICU_NUM_EXPONENT_SYMBOL_FIELD,
  ICU_NUM_EXPONENT_SIGN_FIELD,
  ICU_NUM_EXPONENT_FIELD,
  ICU_NUM_GROUPING_SEPARATOR_FIELD,
  ICU_NUM_CURRENCY_FIELD,
  ICU_NUM_PERCENT_FIELD,
  ICU_NUM_PERMILL_FIELD,
  ICU_NUM_SIGN_FIELD,
  ICU_NUM_MEASURE_UNIT_FIELD,
  ICU_NUM_COMPACT_FIELD,
} IcuNumberFormatField;

G_END_DECLS
