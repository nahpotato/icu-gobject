/*
 * Copyright 2023 Nahuel Gomez https://nahuelwexd.com
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "icu-field-position-iterator.h"
#include "icu-field-position-iterator-private.h"

#include <unicode/ufieldpositer.h>
#include "icu-error-private.h"

struct _IcuFieldPositionIterator
{
  guint ref_count;
  UFieldPositionIterator *fpositer;
};

G_DEFINE_BOXED_TYPE (IcuFieldPositionIterator, icu_field_position_iterator, icu_field_position_iterator_ref, icu_field_position_iterator_unref)

static void
icu_field_position_iterator_free (IcuFieldPositionIterator *self)
{
  g_assert_nonnull (self);
  g_assert_cmpuint (self->ref_count, ==, 0);

  g_clear_pointer (&self->fpositer, ufieldpositer_close);

  g_slice_free (IcuFieldPositionIterator, self);
}

IcuFieldPositionIterator *
icu_field_position_iterator_new (GError **error)
{
  g_autoptr (IcuFieldPositionIterator) self = NULL;
  UErrorCode ec = U_ZERO_ERROR;

  self = g_slice_new0 (IcuFieldPositionIterator);
  self->ref_count = 1;

  self->fpositer = ufieldpositer_open (&ec);
  if (icu_has_failed (ec, error))
    return NULL;

  return g_steal_pointer (&self);
}

IcuFieldPositionIterator *
icu_field_position_iterator_ref (IcuFieldPositionIterator *self)
{
  g_return_val_if_fail (self != NULL, NULL);
  g_return_val_if_fail (self->ref_count >= 1, NULL);

  g_atomic_int_inc (&self->ref_count);

  return self;
}

void
icu_field_position_iterator_unref (IcuFieldPositionIterator *self)
{
  g_return_if_fail (self != NULL);
  g_return_if_fail (self->ref_count >= 1);

  if (g_atomic_int_dec_and_test (&self->ref_count))
    icu_field_position_iterator_free (self);
}

/**
 * icu_field_position_iterator_next:
 * @begin_index: (out) (optional):
 * @end_index: (out) (optional):
 */
gint32
icu_field_position_iterator_next (IcuFieldPositionIterator *self,
                                  gint32                   *begin_index,
                                  gint32                   *end_index)
{
  g_return_val_if_fail (self != NULL, 0);
  g_return_val_if_fail (self->ref_count >= 1, 0);

  return ufieldpositer_next (self->fpositer, begin_index, end_index);
}

UFieldPositionIterator *
icu_field_position_iterator_get_fpositer (IcuFieldPositionIterator *self)
{
  return self->fpositer;
}
