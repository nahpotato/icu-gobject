/*
 * Copyright 2023 Nahuel Gomez https://nahuelwexd.com
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#pragma once

#include "icu-formatted-value.h"
#include <unicode/uformattedvalue.h>

G_BEGIN_DECLS

G_GNUC_INTERNAL
IcuFormattedValue *icu_formatted_value_new  (const UFormattedValue *ufmtval);
G_GNUC_INTERNAL
void               icu_formatted_value_free (IcuFormattedValue *self);

G_DEFINE_AUTOPTR_CLEANUP_FUNC (IcuFormattedValue, icu_formatted_value_free)

G_END_DECLS
