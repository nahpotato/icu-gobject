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
#include "icu-formatted-number.h"

G_BEGIN_DECLS

#define ICU_TYPE_NUMBER_FORMATTER (icu_number_formatter_get_type())

typedef struct _IcuNumberFormatter IcuNumberFormatter;

ICU_AVAILABLE_IN_ALL
GType icu_number_formatter_get_type (void);

ICU_AVAILABLE_IN_ALL
IcuNumberFormatter *icu_number_formatter_new (const gchar  *skeleton,
                                              const gchar  *locale,
                                              GError      **error);

ICU_AVAILABLE_IN_ALL
IcuNumberFormatter *icu_number_formatter_ref   (IcuNumberFormatter *self);
ICU_AVAILABLE_IN_ALL
void                icu_number_formatter_unref (IcuNumberFormatter *self);

ICU_AVAILABLE_IN_ALL
IcuFormattedNumber *icu_number_formatter_format_int     (IcuNumberFormatter  *self,
                                                         gint64               value,
                                                         GError             **error);
ICU_AVAILABLE_IN_ALL
IcuFormattedNumber *icu_number_formatter_format_double  (IcuNumberFormatter  *self,
                                                         gdouble              value,
                                                         GError             **error);
ICU_AVAILABLE_IN_ALL
IcuFormattedNumber *icu_number_formatter_format_decimal (IcuNumberFormatter  *self,
                                                         const gchar         *value,
                                                         GError             **error);

G_DEFINE_AUTOPTR_CLEANUP_FUNC (IcuNumberFormatter, icu_number_formatter_unref)

G_END_DECLS
