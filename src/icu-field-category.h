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
#include "icu-enum-types.h"

G_BEGIN_DECLS

typedef enum {
  ICU_FIELD_CATEGORY_UNDEFINED = 0,
  ICU_FIELD_CATEGORY_DATE,
  ICU_FIELD_CATEGORY_NUMBER,
  ICU_FIELD_CATEGORY_LIST,
  ICU_FIELD_CATEGORY_RELATIVE_DATETIME,
  ICU_FIELD_CATEGORY_DATE_INTERVAL,
  ICU_FIELD_CATEGORY_LIST_SPAN = 0x1000 + ICU_FIELD_CATEGORY_LIST,
  ICU_FIELD_CATEGORY_DATE_INTERVAL_SPAN = 0x1000 + ICU_FIELD_CATEGORY_DATE_INTERVAL,
  ICU_FIELD_CATEGORY_NUMBER_RANGE_SPAN = 0x1000 + ICU_FIELD_CATEGORY_NUMBER,
} IcuFieldCategory;

G_END_DECLS
