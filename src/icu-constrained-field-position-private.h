/*
 * Copyright 2023 Nahuel Gomez https://nahuelwexd.com
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#pragma once

#include "icu-constrained-field-position.h"
#include <unicode/uformattedvalue.h>

G_BEGIN_DECLS

G_GNUC_INTERNAL
UConstrainedFieldPosition *icu_constrained_field_position_get_ucfpos (IcuConstrainedFieldPosition *self);

G_END_DECLS
