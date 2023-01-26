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

/**
 * icu_field_position_iterator_new:
 * @error: (out) (optional): The return location for a recoverable
 *   error.
 *
 * Creates a new [class@FieldPositionIterator].
 *
 * Returns: A newly created [class@FieldPositionIterator].
 */
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

/**
 * icu_field_position_iterator_ref:
 * @self: A [class@FieldPositionIterator].
 *
 * Increases the reference count of `self` by one.
 *
 * Returns: (transfer full): The passed in
 *   [class@FieldPositionIterator].
 */
IcuFieldPositionIterator *
icu_field_position_iterator_ref (IcuFieldPositionIterator *self)
{
  g_return_val_if_fail (self != NULL, NULL);
  g_return_val_if_fail (self->ref_count >= 1, NULL);

  g_atomic_int_inc (&self->ref_count);

  return self;
}

/**
 * icu_field_position_iterator_unref:
 * @self: A [class@FieldPositionIterator].
 *
 * Decreases the reference count of `self` by one.
 *
 * If the resulting reference count is zero, frees the memory of
 * `self`.
 */
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
 * @self: A [class@FieldPositionIterator].
 * @begin_index: (out) (optional): Set to the start offset of the
 *   field in the formatted string (undefined if the method returns
 *   a negative value). Ignored if `NULL`.
 * @end_index: (out) (optional): Set to the end offset of the field
 *   in the formatted string (undefined if the method returns a
 *   negative value). Ignored if `NULL`.
 *
 * Get information for the next field in the formatted string to
 * which `self` currently applies, or return a negative value if
 * there are no more fields.
 *
 * Returns: The field type (non-negative value), or a negative value
 *   if there are no more fields for which to provide information.
 *   If negative, then any values pointed by `begin_index` and
 *   `end_index` are undefined.
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
