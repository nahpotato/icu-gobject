/*
 * Copyright 2023 Nahuel Gomez https://nahuelwexd.com
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "icu-field-position.h"

G_DEFINE_BOXED_TYPE (IcuFieldPosition, icu_field_position, icu_field_position_copy, icu_field_position_free)

IcuFieldPosition *
icu_field_position_copy (const IcuFieldPosition *self)
{
  IcuFieldPosition *copy = NULL;

  g_return_val_if_fail (self != NULL, NULL);

  copy = g_new0 (IcuFieldPosition, 1);
  memcpy (copy, self, sizeof (IcuFieldPosition));

  return copy;
}

void
icu_field_position_free (IcuFieldPosition *self)
{
  g_return_if_fail (self != NULL);
  g_free (self);
}
