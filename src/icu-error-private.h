/*
 * Copyright 2022 Nahuel Gomez https://nahuelwexd.com
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#pragma once

#include <glib.h>
#include <unicode/utypes.h>

#define icu_handle_u_error_code(error, ec)               \
  G_STMT_START                                           \
  {                                                      \
    if (U_FAILURE ((ec)))                                \
      {                                                  \
        icu_set_error_from_u_error_code ((error), (ec)); \
        return NULL;                                     \
      }                                                  \
  }                                                      \
  G_STMT_END

G_GNUC_INTERNAL
void icu_set_error_from_u_error_code (GError **error, UErrorCode ec);
