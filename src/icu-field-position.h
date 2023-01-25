/*
 * Copyright 2023 Nahuel Gomez https://nahuelwexd.com
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#pragma once

#include <glib-object.h>
#include "icu-version.h"

G_BEGIN_DECLS

#define ICU_TYPE_FIELD_POSITION (icu_field_position_get_type())

typedef struct _IcuFieldPosition
{
  gint32 field;
  gint32 begin_index;
  gint32 end_index;
} IcuFieldPosition;

ICU_AVAILABLE_IN_ALL
GType icu_field_position_get_type (void);

ICU_AVAILABLE_IN_ALL
IcuFieldPosition *icu_field_position_copy (const IcuFieldPosition *self);
ICU_AVAILABLE_IN_ALL
void              icu_field_position_free (IcuFieldPosition *self);

G_END_DECLS
