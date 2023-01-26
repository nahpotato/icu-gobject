/*
 * Copyright 2023 Nahuel Gomez https://nahuelwexd.com
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "icu-constrained-field-position.h"
#include "icu-constrained-field-position-private.h"

#include <unicode/uformattedvalue.h>
#include "icu-error-private.h"

/**
 * IcuConstrainedFieldPosition:
 *
 * Represents a span of a string containing a given field.
 *
 * This class differs from [struct@FieldPosition] in the following
 * ways:
 *
 * 1. It has information on the field category.
 * 2. It allows you to set constraints to use when iterating over
 *    field positions.
 * 3. It is used for the newer [class@FormattedValue] APIs.
 */

struct _IcuConstrainedFieldPosition
{
  guint ref_count;
  UConstrainedFieldPosition *ucfpos;
};

G_DEFINE_BOXED_TYPE (IcuConstrainedFieldPosition, icu_constrained_field_position, icu_constrained_field_position_ref, icu_constrained_field_position_unref)

static void
icu_constrained_field_position_free (IcuConstrainedFieldPosition *self)
{
  g_assert_nonnull (self);
  g_assert_cmpuint (self->ref_count, ==, 0);

  g_clear_pointer (&self->ucfpos, ucfpos_close);

  g_slice_free (IcuConstrainedFieldPosition, self);
}

/**
 * icu_constrained_field_position_new:
 * @error: (out) (optional): The return location for a recoverable
 *   error.
 *
 * Creates a new [class@ConstrainedFieldPosition].
 *
 * By default, the [class@ConstrainedFieldPosition] has no iteration
 * constraints.
 *
 * Returns: (transfer full): A newly created
 *   [class@ConstrainedFieldPosition].
 */
IcuConstrainedFieldPosition *
icu_constrained_field_position_new (GError **error)
{
  g_autoptr (IcuConstrainedFieldPosition) self = NULL;
  UErrorCode ec = U_ZERO_ERROR;

  self = g_slice_new0 (IcuConstrainedFieldPosition);
  self->ref_count = 1;

  self->ucfpos = ucfpos_open (&ec);
  if (icu_has_failed (ec, error))
    return NULL;

  return g_steal_pointer (&self);
}

/**
 * icu_constrained_field_position_ref:
 * @self: A [class@ConstrainedFieldPosition].
 *
 * Increases the reference count of `self` by one.
 *
 * Returns: (transfer full): The passed in
 *   [class@ConstrainedFieldPosition].
 */
IcuConstrainedFieldPosition *
icu_constrained_field_position_ref (IcuConstrainedFieldPosition *self)
{
  g_return_val_if_fail (self != NULL, NULL);
  g_return_val_if_fail (self->ref_count >= 1, NULL);

  g_atomic_int_inc (&self->ref_count);

  return self;
}

/**
 * icu_constrained_field_position_unref:
 * @self: A [class@ConstrainedFieldPosition].
 *
 * Decreases the reference count of `self` by one.
 *
 * If the resulting reference count is zero, frees the memory of
 * `self`.
 */
void
icu_constrained_field_position_unref (IcuConstrainedFieldPosition *self)
{
  g_return_if_fail (self != NULL);
  g_return_if_fail (self->ref_count >= 1);

  if (g_atomic_int_dec_and_test (&self->ref_count))
    icu_constrained_field_position_free (self);
}

/**
 * icu_constrained_field_position_reset:
 * @self: A [class@ConstrainedFieldPosition].
 * @error: (out) (optional): The return location for a recoverable
 *   error.
 *
 * Resets `self` to its initial state, as if it were newly created.
 *
 * Removes any constraints that may have been set on the instance.
 */
void
icu_constrained_field_position_reset (IcuConstrainedFieldPosition  *self,
                                      GError                      **error)
{
  UErrorCode ec = U_ZERO_ERROR;

  g_return_if_fail (self != NULL);
  g_return_if_fail (self->ref_count >= 1);

  ucfpos_reset (self->ucfpos, &ec);
  if (icu_has_failed (ec, error))
    return;
}

/**
 * icu_constrained_field_position_constrain_category:
 * @self: A [class@ConstrainedFieldPosition].
 * @category: The field category to fix when iterating.
 * @error: (out) (optional): The return location for a recoverable
 *   error.
 *
 * Sets a constraint on the field category.
 *
 * When `self` is passed to [method@FormattedValue.next_position],
 * positions are skipped unless they have the given category.
 *
 * Any previously set constraints are cleared.
 *
 * For example, to loop over only the number-related fields:
 * ```c
 * g_autoptr (IcuConstrainedFieldPosition) position = NULL;
 *
 * position = icu_constrained_field_position_new (NULL);
 * icu_constrained_field_position_constrain_category (position,
 *                                                    ICU_FIELD_CATEGORY_NUMBER,
 *                                                    NULL);
 *
 * while (icu_formatted_value_next_position (value, position, NULL))
 *   {
 *     // handle the number-related field position
 *   }
 * ```
 *
 * Changing the constraint while in the middle of iterating over a
 * [class@FormattedValue] does not generally have well-defined
 * behavior.
 */
void
icu_constrained_field_position_constrain_category (IcuConstrainedFieldPosition  *self,
                                                   gint32                        category,
                                                   GError                      **error)
{
  UErrorCode ec = U_ZERO_ERROR;

  g_return_if_fail (self != NULL);
  g_return_if_fail (self->ref_count >= 1);

  ucfpos_constrainCategory (self->ucfpos, category, &ec);
  if (icu_has_failed (ec, error))
    return;
}

/**
 * icu_constrained_field_position_constrain_field:
 * @self: A [class@ConstrainedFieldPosition].
 * @category: The field category to fix when iterating.
 * @field: The field to fix when iterating.
 * @error: (out) (optional): The return location for a recoverable
 *   error.
 *
 * Sets a constraint on the category and field.
 *
 * When `self` is passed to [method@FormattedValue.next_position],
 * positions are skipped unless they have the given category and
 * field.
 *
 * Any previously set constraints are cleared.
 *
 * For example, to loop over all grouping separators:
 * ```c
 * g_autoptr (IcuConstrainedFieldPosition) position = NULL;
 *
 * position = icu_constrained_field_position_new (NULL);
 * icu_constrained_field_position_constrain_field (position,
 *                                                 ICU_FIELD_CATEGORY_NUMBER,
 *                                                 ICU_NUM_GROUPING_SEPARATOR_FIELD,
 *                                                 NULL);
 *
 * while (icu_formatted_value_next_position (value, position, NULL))
 *   {
 *     // handle the grouping separator position
 *   }
 * ```
 *
 * Changing the constraint while in the middle of iterating over a
 * [class@FormattedValue] does not generally have well-defined
 * behavior.
 */
void
icu_constrained_field_position_constrain_field (IcuConstrainedFieldPosition  *self,
                                                gint32                        category,
                                                gint32                        field,
                                                GError                      **error)
{
  UErrorCode ec = U_ZERO_ERROR;

  g_return_if_fail (self != NULL);
  g_return_if_fail (self->ref_count >= 1);

  ucfpos_constrainField (self->ucfpos, category, field, &ec);
  if (icu_has_failed (ec, error))
    return;
}

/**
 * icu_constrained_field_position_get_category:
 * @self: A [class@ConstrainedFieldPosition].
 * @error: (out) (optional): The return location for a recoverable
 *   error.
 *
 * Gets the field category for the current position.
 *
 * If a category or field constraint was set, this method returns
 * the constrained category. Otherwise, the return value is
 * well-defined only after [method@FormattedValue.next_position]
 * returns `TRUE`.
 *
 * Returns: The field category stored in `self`.
 */
gint32
icu_constrained_field_position_get_category (IcuConstrainedFieldPosition  *self,
                                             GError                      **error)
{
  gint32 category = 0;
  UErrorCode ec = U_ZERO_ERROR;

  g_return_val_if_fail (self != NULL, 0);
  g_return_val_if_fail (self->ref_count >= 1, 0);

  category = ucfpos_getCategory (self->ucfpos, &ec);
  if (icu_has_failed (ec, error))
    return 0;

  return category;
}

/**
 * icu_constrained_field_position_get_field:
 * @self: A [class@ConstrainedFieldPosition].
 * @error: (out) (optional): The return location for a recoverable
 *   error.
 *
 * Gets the field for the current position.
 *
 * If a field constraint was set, this function returns the
 * constrained field. Otherwise, the return value is well-defined
 * only after [method@FormattedValue.next_position] returns `TRUE`.
 *
 * Returns: The field stored in `self`.
 */
gint32
icu_constrained_field_position_get_field (IcuConstrainedFieldPosition  *self,
                                          GError                      **error)
{
  gint32 field = 0;
  UErrorCode ec = U_ZERO_ERROR;

  g_return_val_if_fail (self != NULL, 0);
  g_return_val_if_fail (self->ref_count >= 1, 0);

  field = ucfpos_getField (self->ucfpos, &ec);
  if (icu_has_failed (ec, error))
    return 0;

  return field;
}

/**
 * icu_constrained_field_position_get_indexes:
 * @self: A [class@ConstrainedFieldPosition].
 * @start: (out) (optional): Set to the start index stored in `self`.
 *   Ignored if `NULL`.
 * @limit: (out) (optional): Set to the end index stored in `self`.
 *   Ignored if `NULL`.
 * @error: (out) (optional): The return location for a recoverable
 *   error.
 *
 * Gets the **inclusive** start and **exclusive** end index stored
 * for the current position.
 *
 * The output values are well-defined only after
 * [method@FormattedValue.next_position] returns `TRUE`.
 */
void
icu_constrained_field_position_get_indexes (IcuConstrainedFieldPosition  *self,
                                            gint32                       *start,
                                            gint32                       *limit,
                                            GError                      **error)
{
  UErrorCode ec = U_ZERO_ERROR;

  g_return_if_fail (self != NULL);
  g_return_if_fail (self->ref_count >= 1);

  ucfpos_getIndexes (self->ucfpos, start, limit, &ec);
  if (icu_has_failed (ec, error))
    return;
}

/**
 * icu_constrained_field_position_get_int64_iteration_context:
 * @self: A [class@ConstrainedFieldPosition].
 * @error: (out) (optional): The return location for a recoverable
 *   error.
 *
 * Gets a `gint64` that [class@FormattedValue] implementations may
 * use for storage.
 *
 * The initial value is zero.
 *
 * Users of [class@FormattedValue] should not need to call this
 * method.
 *
 * Returns: The current iteration context from
 *   [method@ConstrainedFieldPosition.set_int64_iteration_context].
 */
gint64
icu_constrained_field_position_get_int64_iteration_context (IcuConstrainedFieldPosition  *self,
                                                            GError                      **error)
{
  gint64 context = 0;
  UErrorCode ec = U_ZERO_ERROR;

  g_return_val_if_fail (self != NULL, 0);
  g_return_val_if_fail (self->ref_count >= 1, 0);

  context = ucfpos_getInt64IterationContext (self->ucfpos, &ec);
  if (icu_has_failed (ec, error))
    return 0;

  return context;
}

/**
 * icu_constrained_field_position_set_int64_iteration_context:
 * @self: A [class@ConstrainedFieldPosition].
 * @context: The new iteration context.
 * @error: (out) (optional): The return location for a recoverable
 *   error.
 *
 * Sets a `gint64` that [class@FormattedValue] implementations may
 * use for storage.
 *
 * Intended to be used by [class@FormattedValue] implementations.
 */
void
icu_constrained_field_position_set_int64_iteration_context (IcuConstrainedFieldPosition  *self,
                                                            gint64                        context,
                                                            GError                      **error)
{
  UErrorCode ec = U_ZERO_ERROR;

  g_return_if_fail (self != NULL);
  g_return_if_fail (self->ref_count >= 1);

  ucfpos_setInt64IterationContext (self->ucfpos, context, &ec);
  if (icu_has_failed (ec, error))
    return;
}

/**
 * icu_constrained_field_position_matches_field:
 * @self: A [class@ConstrainedFieldPosition].
 * @category: The category to test.
 * @field: The field to test.
 * @error: (out) (optional): The return location for a recoverable
 *   error.
 *
 * Determines whether a given field should be included given the
 * constraints.
 *
 * Intended to be used by [class@FormattedValue] implementations.
 *
 * Returns: `TRUE` if the given field should be included, `FALSE`
 *   otherwise.
 */
gboolean
icu_constrained_field_position_matches_field (IcuConstrainedFieldPosition  *self,
                                              gint32                        category,
                                              gint32                        field,
                                              GError                      **error)
{
  gboolean result = FALSE;
  UErrorCode ec = U_ZERO_ERROR;

  g_return_val_if_fail (self != NULL, FALSE);
  g_return_val_if_fail (self->ref_count >= 1, FALSE);

  result = ucfpos_matchesField (self->ucfpos, category, field, &ec);
  if (icu_has_failed (ec, error))
    return FALSE;

  return result;
}

/**
 * icu_constrained_field_position_set_state:
 * @self: A [class@ConstrainedFieldPosition].
 * @category: The new field category.
 * @field: The new field.
 * @start: The new inclusive start index.
 * @limit: The new exclusive end index.
 * @error: (out) (optional): The return location for a recoverable
 *   error.
 *
 * Sets new values for the primary public getters.
 *
 * Intended to be used by [class@FormattedValue] implementations.
 *
 * It is up to the implementation to ensure that the user-requested
 * constraints are satisfied. This method does not check!
 */
void
icu_constrained_field_position_set_state (IcuConstrainedFieldPosition  *self,
                                          gint32                        category,
                                          gint32                        field,
                                          gint32                        start,
                                          gint32                        limit,
                                          GError                      **error)
{
  UErrorCode ec = U_ZERO_ERROR;

  g_return_if_fail (self != NULL);
  g_return_if_fail (self->ref_count >= 1);

  ucfpos_setState (self->ucfpos, category, field, start, limit, &ec);
  if (icu_has_failed (ec, error))
    return;
}

UConstrainedFieldPosition *
icu_constrained_field_position_get_ucfpos (IcuConstrainedFieldPosition *self)
{
  return self->ucfpos;
}
