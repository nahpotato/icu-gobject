/*
 * Copyright 2022 Nahuel Gomez https://nahuelwexd.com
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#pragma once

#include "icu-error.h"
#include <unicode/utypes.h>

G_BEGIN_DECLS

G_GNUC_INTERNAL
gboolean icu_has_failed (UErrorCode ec, GError **error);

G_END_DECLS
