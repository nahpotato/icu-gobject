/*
 * Copyright 2023 Nahuel Gomez https://nahuelwexd.com
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "icu-constrained-field-position.h"
#include "icu-constrained-field-position-private.h"

#include <unicode/uformattedvalue.h>
#include "icu-error-private.h"

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

IcuConstrainedFieldPosition *
icu_constrained_field_position_ref (IcuConstrainedFieldPosition *self)
{
  g_return_val_if_fail (self != NULL, NULL);
  g_return_val_if_fail (self->ref_count >= 1, NULL);

  g_atomic_int_inc (&self->ref_count);

  return self;
}

void
icu_constrained_field_position_unref (IcuConstrainedFieldPosition *self)
{
  g_return_if_fail (self != NULL);
  g_return_if_fail (self->ref_count >= 1);

  if (g_atomic_int_dec_and_test (&self->ref_count))
    icu_constrained_field_position_free (self);
}

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
 * @start: (out) (optional):
 * @limit: (out) (optional):
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
