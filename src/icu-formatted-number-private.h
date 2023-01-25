/*
 * Copyright 2022 Nahuel Gomez https://nahuelwexd.com
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#pragma once

#include "icu-formatted-number.h"
#include <unicode/unumberformatter.h>

G_BEGIN_DECLS

G_GNUC_INTERNAL
IcuFormattedNumber *icu_formatted_number_new (UFormattedNumber *uresult);

G_END_DECLS
