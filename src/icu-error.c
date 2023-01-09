/*
 * Copyright 2022 Nahuel Gomez https://nahuelwexd.com
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "icu-error.h"
#include "icu-error-private.h"

G_DEFINE_QUARK (icu-standard-error-quark, icu_standard_error)
G_DEFINE_QUARK (icu-fmt-parse-error-quark, icu_fmt_parse_error)

#define ERROR_OVERWRITTEN_WARNING "GError set over the top of a previous GError or uninitialized memory.\n" \
  "This indicates a bug in someone's code. You must ensure an error is NULL before it's set.\n"             \
  "The overwriting error message was: %s"

static gchar *
get_standard_error_message (UErrorCode ec)
{
  return g_strdup ("");
}

static gchar *
get_fmt_parse_error_message (UErrorCode ec)
{
  return g_strdup ("");
}

static GError *
get_error_from_u_error_code (UErrorCode ec)
{
  g_autofree gchar *error_message = NULL;

  if (ec > U_ZERO_ERROR && ec < U_STANDARD_ERROR_LIMIT)
    {
      error_message = get_standard_error_message (ec);
      return g_error_new_literal (ICU_STANDARD_ERROR, ec, error_message);
    }

  if (ec >= U_FMT_PARSE_ERROR_START && ec < U_FMT_PARSE_ERROR_LIMIT)
    {
      error_message = get_fmt_parse_error_message (ec);
      return g_error_new_literal (ICU_FMT_PARSE_ERROR, ec, error_message);
    }

  return NULL;
}

void
icu_set_error_from_u_error_code (GError     **error,
                                 UErrorCode   ec)
{
  GError *inner_error = NULL;

  if (error == NULL)
    return;

  inner_error = get_error_from_u_error_code (ec);

  if (inner_error == NULL)
    return;

  if (*error != NULL)
    {
      g_warning (ERROR_OVERWRITTEN_WARNING, inner_error->message);
      g_error_free (inner_error);
    }
  else
    {
      *error = inner_error;
    }
}
