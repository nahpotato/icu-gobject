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
#include "icu-formatted-value.h"
#include "icu-field-position.h"
#include "icu-field-position-iterator.h"

G_BEGIN_DECLS

#define ICU_TYPE_FORMATTED_NUMBER (icu_formatted_number_get_type())

typedef struct _IcuFormattedNumber IcuFormattedNumber;

ICU_AVAILABLE_IN_ALL
GType icu_formatted_number_get_type (void);

ICU_AVAILABLE_IN_ALL
IcuFormattedNumber *icu_formatted_number_ref   (IcuFormattedNumber *self);
ICU_AVAILABLE_IN_ALL
void                icu_formatted_number_unref (IcuFormattedNumber *self);

ICU_AVAILABLE_IN_ALL
IcuFormattedValue *icu_formatted_number_as_value (IcuFormattedNumber  *self,
                                                  GError             **error);

ICU_AVAILABLE_IN_ALL
gboolean icu_formatted_number_next_field_position (IcuFormattedNumber  *self,
                                                   IcuFieldPosition    *position,
                                                   GError             **error);

ICU_AVAILABLE_IN_ALL
void icu_formatted_number_get_all_field_positions (IcuFormattedNumber        *self,
                                                   IcuFieldPositionIterator  *iterator,
                                                   GError                   **error);

ICU_AVAILABLE_IN_ALL
gchar *icu_formatted_number_to_string         (IcuFormattedNumber  *self,
                                               GError             **error);
ICU_AVAILABLE_IN_ALL
gchar *icu_formatted_number_to_decimal_number (IcuFormattedNumber  *self,
                                               GError             **error);

G_DEFINE_AUTOPTR_CLEANUP_FUNC (IcuFormattedNumber, icu_formatted_number_unref)

G_END_DECLS
