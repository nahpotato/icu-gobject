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

static const gchar *
get_standard_error_message (UErrorCode ec)
{
  switch (ec)
    {
    case U_ILLEGAL_ARGUMENT_ERROR:
      return "illegal-argument-error";

    case U_MISSING_RESOURCE_ERROR:
      return "missing-resource-error";

    case U_INVALID_FORMAT_ERROR:
      return "invalid-format-error";

    case U_FILE_ACCESS_ERROR:
      return "file-access-error";

    case U_INTERNAL_PROGRAM_ERROR:
      return "internal-program-error";

    case U_MESSAGE_PARSE_ERROR:
      return "message-parse-error";

    case U_MEMORY_ALLOCATION_ERROR:
      return "memory-allocation-error";

    case U_INDEX_OUTOFBOUNDS_ERROR:
      return "index-outofbounds-error";

    case U_PARSE_ERROR:
      return "parse-error";

    case U_INVALID_CHAR_FOUND:
      return "invalid-char-found";

    case U_TRUNCATED_CHAR_FOUND:
      return "truncated-char-found";

    case U_ILLEGAL_CHAR_FOUND:
      return "illegal-char-found";

    case U_INVALID_TABLE_FORMAT:
      return "invalid-table-format";

    case U_INVALID_TABLE_FILE:
      return "invalid-table-file";

    case U_BUFFER_OVERFLOW_ERROR:
      return "buffer-overflow-error";

    case U_UNSUPPORTED_ERROR:
      return "unsupported-error";

    case U_RESOURCE_TYPE_MISMATCH:
      return "resource-type-mismatch";

    case U_ILLEGAL_ESCAPE_SEQUENCE:
      return "illegal-escape-sequence";

    case U_UNSUPPORTED_ESCAPE_SEQUENCE:
      return "unsupported-escape-sequence";

    case U_NO_SPACE_AVAILABLE:
      return "no-space-available";

    case U_CE_NOT_FOUND_ERROR:
      return "ce-not-found-error";

    case U_PRIMARY_TOO_LONG_ERROR:
      return "primary-too-long-error";

    case U_STATE_TOO_OLD_ERROR:
      return "state-too-old-error";

    case U_TOO_MANY_ALIASES_ERROR:
      return "too-many-aliases-error";

    case U_ENUM_OUT_OF_SYNC_ERROR:
      return "enum-out-of-sync-error";

    case U_INVARIANT_CONVERSION_ERROR:
      return "invariant-conversion-error";

    case U_INVALID_STATE_ERROR:
      return "invalid-state-error";

    case U_COLLATOR_VERSION_MISMATCH:
      return "collator-version-mismatch";

    case U_USELESS_COLLATOR_ERROR:
      return "useless-collator-error";

    case U_NO_WRITE_PERMISSION:
      return "no-write-permission";

    case U_INPUT_TOO_LONG_ERROR:
      return "input-too-long-error";

    default:
      return "";
    }
}

static const gchar *
get_fmt_parse_error_message (UErrorCode ec)
{
  switch (ec)
    {
    case U_UNEXPECTED_TOKEN:
      return "unexpected-token";

    case U_MULTIPLE_DECIMAL_SEPARATORS:
      return "multiple-decimal-separators";

    case U_MULTIPLE_EXPONENTIAL_SYMBOLS:
      return "multiple-exponential-symbols";

    case U_MALFORMED_EXPONENTIAL_PATTERN:
      return "malformed-exponential-pattern";

    case U_MULTIPLE_PERCENT_SYMBOLS:
      return "multiple-percent-symbols";

    case U_MULTIPLE_PERMILL_SYMBOLS:
      return "multiple-permill-symbols";

    case U_MULTIPLE_PAD_SPECIFIERS:
      return "multiple-pad-specifiers";

    case U_PATTERN_SYNTAX_ERROR:
      return "pattern-syntax-error";

    case U_ILLEGAL_PAD_POSITION:
      return "illegal-pad-position";

    case U_UNMATCHED_BRACES:
      return "unmatched-braces";

    case U_UNSUPPORTED_PROPERTY:
      return "unsupported-property";

    case U_UNSUPPORTED_ATTRIBUTE:
      return "unsupported-attribute";

    case U_ARGUMENT_TYPE_MISMATCH:
      return "argument-type-mismatch";

    case U_DUPLICATE_KEYWORD:
      return "duplicate-keyword";

    case U_UNDEFINED_KEYWORD:
      return "undefined-keyword";

    case U_DEFAULT_KEYWORD_MISSING:
      return "default-keyword-missing";

    case U_DECIMAL_NUMBER_SYNTAX_ERROR:
      return "decimal-number-syntax-error";

    case U_FORMAT_INEXACT_ERROR:
      return "format-inexact-error";

    case U_NUMBER_ARG_OUTOFBOUNDS_ERROR:
      return "number-arg-outofbounds-error";

    case U_NUMBER_SKELETON_SYNTAX_ERROR:
      return "number-skeleton-syntax-error";

    default:
      return "";
    }
}

static GError *
get_error_from_u_error_code (UErrorCode ec)
{
  const gchar *error_message = NULL;

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

gboolean
icu_has_failed (UErrorCode ec, GError **error)
{
  g_autoptr (GError) inner_error = NULL;

  if (U_SUCCESS (ec))
    return FALSE;

  if (error == NULL)
    return TRUE;

  inner_error = get_error_from_u_error_code (ec);
  if (inner_error == NULL)
    return TRUE;

  if (*error != NULL)
    g_warning (ERROR_OVERWRITTEN_WARNING, inner_error->message);
  else
    *error = g_steal_pointer (&inner_error);

  return TRUE;
}
