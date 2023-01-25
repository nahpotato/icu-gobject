/*
 * Copyright 2023 Nahuel Gomez https://nahuelwexd.com
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "icu-formatted-value.h"
#include "icu-formatted-value-private.h"

#include "icu-error-private.h"
#include "icu-constrained-field-position-private.h"

struct _IcuFormattedValue
{
  const UFormattedValue *ufmtval;
  gchar *string;
};

G_DEFINE_POINTER_TYPE (IcuFormattedValue, icu_formatted_value)

IcuFormattedValue *
icu_formatted_value_new (const UFormattedValue *ufmtval)
{
  g_autoptr (IcuFormattedValue) self = NULL;

  self = g_slice_new0 (IcuFormattedValue);
  self->ufmtval = ufmtval;

  return g_steal_pointer (&self);
}

void
icu_formatted_value_free (IcuFormattedValue *self)
{
  g_assert_nonnull (self);

  g_clear_pointer (&self->string, g_free);

  g_slice_free (IcuFormattedValue, self);
}

const gchar *
icu_formatted_value_get_string (IcuFormattedValue  *self,
                                GError            **error)
{
  const UChar *ustring = NULL;
  gint32 length = 0;
  UErrorCode ec = U_ZERO_ERROR;

  g_return_val_if_fail (self != NULL, NULL);

  if (self->string != NULL)
    return self->string;

  ustring = ufmtval_getString (self->ufmtval, &length, &ec);
  if (icu_has_failed (ec, error))
    return NULL;

  self->string = g_utf16_to_utf8 (ustring, -1, NULL, NULL, error);

  return self->string;
}

gboolean
icu_formatted_value_next_position (IcuFormattedValue            *self,
                                   IcuConstrainedFieldPosition  *ucfpos,
                                   GError                      **error)
{
  UConstrainedFieldPosition *real_ucfpos = NULL;
  gboolean result = FALSE;
  UErrorCode ec = U_ZERO_ERROR;

  g_return_val_if_fail (self != NULL, FALSE);

  real_ucfpos = icu_constrained_field_position_get_ucfpos (ucfpos);

  result = ufmtval_nextPosition (self->ufmtval, real_ucfpos, &ec);
  if (icu_has_failed (ec, error))
    return FALSE;

  return result;
}
