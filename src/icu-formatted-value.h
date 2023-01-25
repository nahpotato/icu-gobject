/*
 * Copyright 2022 Nahuel Gomez https://nahuelwexd.com
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#pragma once

#if !defined(_ICU_GOBJECT_INSIDE) && !defined(ICU_GOBJECT_COMPILATION)
#  error "Only <icu-gobject.h> can be included directly"
#endif

#include <glib-object.h>
#include "icu-version.h"
#include "icu-constrained-field-position.h"

G_BEGIN_DECLS

#define ICU_TYPE_FORMATTED_VALUE (icu_formatted_value_get_type())

typedef struct _IcuFormattedValue IcuFormattedValue;

ICU_AVAILABLE_IN_ALL
GType icu_formatted_value_get_type (void);

ICU_AVAILABLE_IN_ALL
const gchar *icu_formatted_value_get_string (IcuFormattedValue  *self,
                                             GError            **error);

ICU_AVAILABLE_IN_ALL
gboolean icu_formatted_value_next_position (IcuFormattedValue            *self,
                                            IcuConstrainedFieldPosition  *position,
                                            GError                      **error);

G_END_DECLS
