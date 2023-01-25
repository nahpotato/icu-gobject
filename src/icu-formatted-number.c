/*
 * Copyright 2022 Nahuel Gomez https://nahuelwexd.com
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "icu-formatted-number.h"
#include "icu-formatted-number-private.h"

#include <unicode/unumberformatter.h>
#include "icu-error-private.h"
#include "icu-formatted-value-private.h"
#include "icu-field-position-iterator-private.h"

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

/**
 * icu_formatted_number_as_value:
 * Returns: (transfer none):
 */
IcuFormattedValue *
icu_formatted_number_as_value (IcuFormattedNumber  *self,
                               GError             **error)
{
  const UFormattedValue *ufmtval = NULL;
  UErrorCode ec = U_ZERO_ERROR;

  g_return_val_if_fail (self != NULL, NULL);
  g_return_val_if_fail (self->ref_count >= 1, NULL);

  ufmtval = unumf_resultAsValue (self->uresult, &ec);
  if (icu_has_failed (ec, error))
    return NULL;

  return icu_formatted_value_new (ufmtval);
}

/**
 * icu_formatted_number_next_field_position:
 * @position: (inout):
 */
gboolean
icu_formatted_number_next_field_position (IcuFormattedNumber  *self,
                                          IcuFieldPosition    *position,
                                          GError             **error)
{
  gboolean result = FALSE;
  UFieldPosition ufpos = {0};
  UErrorCode ec = U_ZERO_ERROR;

  g_return_val_if_fail (self != NULL, FALSE);
  g_return_val_if_fail (self->ref_count >= 1, FALSE);
  g_return_val_if_fail (position != NULL, FALSE);

  ufpos = (UFieldPosition) {
    .field = (*position).field,
    .beginIndex = (*position).begin_index,
    .endIndex = (*position).end_index,
  };

  result = unumf_resultNextFieldPosition (self->uresult, &ufpos, &ec);
  if (icu_has_failed (ec, error))
    return FALSE;

  *position = (IcuFieldPosition) {
    .field = ufpos.field,
    .begin_index = ufpos.beginIndex,
    .end_index = ufpos.endIndex,
  };

  return result;
}

void
icu_formatted_number_get_all_field_positions (IcuFormattedNumber        *self,
                                              IcuFieldPositionIterator  *iterator,
                                              GError                   **error)
{
  UFieldPositionIterator *fpositer = NULL;
  UErrorCode ec = U_ZERO_ERROR;

  g_return_if_fail (self != NULL);
  g_return_if_fail (self->ref_count >= 1);
  g_return_if_fail (iterator != NULL);

  fpositer = icu_field_position_iterator_get_fpositer (iterator);

  unumf_resultGetAllFieldPositions (self->uresult, fpositer, &ec);
  if (icu_has_failed (ec, error))
    return;
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
  ec = U_ZERO_ERROR; // ignore U_BUFFER_OVERFLOW_ERROR

  ustring = g_new0 (UChar, length + 1);
  unumf_resultToString (self->uresult, ustring, length + 1, &ec);
  if (icu_has_failed (ec, error))
    return NULL;

  string = g_utf16_to_utf8 (ustring, length, NULL, NULL, error);

  return g_steal_pointer (&string);
}

gchar *
icu_formatted_number_to_decimal_number (IcuFormattedNumber  *self,
                                        GError             **error)
{
  gint32 length = 0;
  UErrorCode ec = U_ZERO_ERROR;
  g_autofree gchar *string = NULL;

  g_return_val_if_fail (self != NULL, NULL);
  g_return_val_if_fail (self->ref_count >= 1, NULL);

  length = unumf_resultToDecimalNumber (self->uresult, NULL, 0, &ec);
  ec = U_ZERO_ERROR; // ignore U_BUFFER_OVERFLOW_ERROR;

  string = g_new0 (gchar, length + 1);
  unumf_resultToDecimalNumber (self->uresult, string, length + 1, &ec);
  if (icu_has_failed (ec, error))
    return NULL;

  return g_steal_pointer (&string);
}
