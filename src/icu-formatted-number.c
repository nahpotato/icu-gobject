/*
 * Copyright 2022 Nahuel Gomez https://nahuelwexd.com
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "icu-formatted-number.h"
#include "icu-formatted-number-private.h"

#include <unicode/unumberformatter.h>
#include "icu-error-private.h"

struct _IcuFormattedNumber
{
  guint ref_count;
  UFormattedNumber *uresult;
};

G_DEFINE_BOXED_TYPE (IcuFormattedNumber, icu_formatted_number, icu_formatted_number_ref, icu_formatted_number_unref)

static void
icu_formatted_number_free (IcuFormattedNumber *self)
{
  g_assert_nonnull (self);
  g_assert_cmpuint (self->ref_count, ==, 0);

  g_clear_pointer (&self->uresult, unumf_closeResult);

  g_slice_free (IcuFormattedNumber, self);
}

IcuFormattedNumber *
icu_formatted_number_new (UFormattedNumber *uresult)
{
  g_autoptr (IcuFormattedNumber) self = NULL;

  self = g_slice_new0 (IcuFormattedNumber);
  self->ref_count = 1;
  self->uresult = uresult;

  return g_steal_pointer (&self);
}

IcuFormattedNumber *
icu_formatted_number_ref (IcuFormattedNumber *self)
{
  g_return_val_if_fail (self != NULL, NULL);
  g_return_val_if_fail (self->ref_count >= 1, NULL);

  g_atomic_int_inc (&self->ref_count);

  return self;
}

void
icu_formatted_number_unref (IcuFormattedNumber *self)
{
  g_return_if_fail (self != NULL);
  g_return_if_fail (self->ref_count >= 1);

  if (g_atomic_int_dec_and_test (&self->ref_count))
    icu_formatted_number_free (self);
}

gchar *
icu_formatted_number_to_string (IcuFormattedNumber  *self,
                                GError             **error)
{
  gint32 length = 0;
  UErrorCode ec = U_ZERO_ERROR;
  g_autofree UChar *ustring = NULL;
  g_autofree gchar *string = NULL;

  g_return_val_if_fail (self != NULL, NULL);
  g_return_val_if_fail (self->ref_count >= 1, NULL);

  length = unumf_resultToString (self->uresult, NULL, 0, &ec);

  // ignore U_BUFFER_OVERFLOW_ERROR
  if (ec == U_BUFFER_OVERFLOW_ERROR)
    ec = U_ZERO_ERROR;
  else
    icu_handle_u_error_code (error, ec);

  ustring = g_new0 (UChar, length + 1);
  unumf_resultToString (self->uresult, ustring, length + 1, &ec);
  icu_handle_u_error_code (error, ec);

  string = g_new0 (gchar, length + 1);
  u_austrcpy (string, ustring);

  return g_steal_pointer (&string);
}
