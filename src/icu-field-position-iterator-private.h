/*
 * Copyright 2023 Nahuel Gomez https://nahuelwexd.com
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#pragma once

#include "icu-field-position-iterator.h"
#include <unicode/ufieldpositer.h>

G_BEGIN_DECLS

G_GNUC_INTERNAL
UFieldPositionIterator *icu_field_position_iterator_get_fpositer (IcuFieldPositionIterator *self);

G_END_DECLS
