/*
 * Copyright 2022 Nahuel Gomez https://nahuelwexd.com
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "icu-number-formatter.h"

#include <unicode/unumberformatter.h>
#include "icu-error-private.h"
#include "icu-formatted-number-private.h"

struct _IcuNumberFormatter
{
  guint ref_count;
  UNumberFormatter *uformatter;
};

G_DEFINE_BOXED_TYPE (IcuNumberFormatter, icu_number_formatter, icu_number_formatter_ref, icu_number_formatter_unref)

// Enable automatic pointers for UFormattedNumber
G_DEFINE_AUTOPTR_CLEANUP_FUNC (UFormattedNumber, unumf_closeResult)

static void
icu_number_formatter_free (IcuNumberFormatter *self)
{
  g_assert_nonnull (self);
  g_assert_cmpuint (self->ref_count, ==, 0);

  g_clear_pointer (&self->uformatter, unumf_close);

  g_slice_free (IcuNumberFormatter, self);
}

IcuNumberFormatter *
icu_number_formatter_new (const gchar  *skeleton,
                          const gchar  *locale,
                          GError      **error)
{
  g_autoptr (IcuNumberFormatter) self = NULL;
  g_autofree UChar *uskeleton = NULL;
  UErrorCode ec = U_ZERO_ERROR;

  self = g_slice_new0 (IcuNumberFormatter);
  self->ref_count = 1;

  uskeleton = g_new0 (UChar, strlen (skeleton) + 1);
  u_uastrcpy (uskeleton, skeleton);

  self->uformatter = unumf_openForSkeletonAndLocale (uskeleton, -1, locale, &ec);
  icu_handle_u_error_code (error, ec);

  return g_steal_pointer (&self);
}

IcuNumberFormatter *
icu_number_formatter_ref (IcuNumberFormatter *self)
{
  g_return_val_if_fail (self != NULL, NULL);
  g_return_val_if_fail (self->ref_count >= 1, NULL);

  g_atomic_int_inc (&self->ref_count);

  return self;
}

void
icu_number_formatter_unref (IcuNumberFormatter *self)
{
  g_return_if_fail (self != NULL);
  g_return_if_fail (self->ref_count >= 1);

  if (g_atomic_int_dec_and_test (&self->ref_count))
    icu_number_formatter_free (self);
}

IcuFormattedNumber *
icu_number_formatter_format_int (IcuNumberFormatter  *self,
                                 gint64               value,
                                 GError             **error)
{
  g_autoptr (UFormattedNumber) uresult = NULL;
  UErrorCode ec = U_ZERO_ERROR;

  g_return_val_if_fail (self != NULL, NULL);
  g_return_val_if_fail (self->ref_count >= 1, NULL);

  uresult = unumf_openResult (&ec);
  icu_handle_u_error_code (error, ec);

  unumf_formatInt (self->uformatter, value, uresult, &ec);
  icu_handle_u_error_code (error, ec);

  return icu_formatted_number_new (g_steal_pointer (&uresult));
}

IcuFormattedNumber *
icu_number_formatter_format_double (IcuNumberFormatter  *self,
                                    gdouble              value,
                                    GError             **error)
{
  g_autoptr (UFormattedNumber) uresult = NULL;
  UErrorCode ec = U_ZERO_ERROR;

  g_return_val_if_fail (self != NULL, NULL);
  g_return_val_if_fail (self->ref_count >= 1, NULL);

  uresult = unumf_openResult (&ec);
  icu_handle_u_error_code (error, ec);

  unumf_formatDouble (self->uformatter, value, uresult, &ec);
  icu_handle_u_error_code (error, ec);

  return icu_formatted_number_new (g_steal_pointer (&uresult));
}

IcuFormattedNumber *
icu_number_formatter_format_decimal (IcuNumberFormatter  *self,
                                     const gchar         *value,
                                     GError             **error)
{
  g_autoptr (UFormattedNumber) uresult = NULL;
  UErrorCode ec = U_ZERO_ERROR;

  g_return_val_if_fail (self != NULL, NULL);
  g_return_val_if_fail (self->ref_count >= 1, NULL);
  g_return_val_if_fail (value != NULL, NULL);

  uresult = unumf_openResult (&ec);
  icu_handle_u_error_code (error, ec);

  unumf_formatDecimal (self->uformatter, value, -1, uresult, &ec);
  icu_handle_u_error_code (error, ec);

  return icu_formatted_number_new (g_steal_pointer (&uresult));
}
