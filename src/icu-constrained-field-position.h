/*
 * Copyright 2023 Nahuel Gomez https://nahuelwexd.com
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#pragma once

#if !defined(_ICU_GOBJECT_INSIDE) && !defined(ICU_GOBJECT_COMPILATION)
#  error "Only <icu-gobject.h> can be included directly"
#endif

#include <glib-object.h>
#include "icu-version.h"

G_BEGIN_DECLS

#define ICU_TYPE_CONSTRAINED_FIELD_POSITION (icu_constrained_field_position_get_type())

typedef struct _IcuConstrainedFieldPosition IcuConstrainedFieldPosition;

ICU_AVAILABLE_IN_ALL
GType icu_constrained_field_position_get_type (void);

ICU_AVAILABLE_IN_ALL
IcuConstrainedFieldPosition *icu_constrained_field_position_new (GError **error);

ICU_AVAILABLE_IN_ALL
IcuConstrainedFieldPosition *icu_constrained_field_position_ref   (IcuConstrainedFieldPosition *self);
ICU_AVAILABLE_IN_ALL
void                         icu_constrained_field_position_unref (IcuConstrainedFieldPosition *self);

ICU_AVAILABLE_IN_ALL
void icu_constrained_field_position_reset (IcuConstrainedFieldPosition  *self,
                                           GError                      **error);

ICU_AVAILABLE_IN_ALL
void icu_constrained_field_position_constrain_category (IcuConstrainedFieldPosition  *self,
                                                        gint32                        category,
                                                        GError                      **error);
ICU_AVAILABLE_IN_ALL
void icu_constrained_field_position_constrain_field    (IcuConstrainedFieldPosition  *self,
                                                        gint32                        category,
                                                        gint32                        field,
                                                        GError                      **error);

ICU_AVAILABLE_IN_ALL
gint32 icu_constrained_field_position_get_category (IcuConstrainedFieldPosition  *self,
                                                    GError                      **error);
ICU_AVAILABLE_IN_ALL
gint32 icu_constrained_field_position_get_field    (IcuConstrainedFieldPosition  *self,
                                                    GError                      **error);

ICU_AVAILABLE_IN_ALL
void icu_constrained_field_position_get_indexes (IcuConstrainedFieldPosition  *self,
                                                 gint32                       *start,
                                                 gint32                       *limit,
                                                 GError                      **error);

ICU_AVAILABLE_IN_ALL
gint64 icu_constrained_field_position_get_int64_iteration_context (IcuConstrainedFieldPosition  *self,
                                                                   GError                      **error);
ICU_AVAILABLE_IN_ALL
void   icu_constrained_field_position_set_int64_iteration_context (IcuConstrainedFieldPosition  *self,
                                                                   gint64                        context,
                                                                   GError                      **error);

ICU_AVAILABLE_IN_ALL
gboolean icu_constrained_field_position_matches_field (IcuConstrainedFieldPosition  *self,
                                                       gint32                        category,
                                                       gint32                        field,
                                                       GError                      **error);

ICU_AVAILABLE_IN_ALL
void icu_constrained_field_position_set_state (IcuConstrainedFieldPosition  *self,
                                               gint32                        category,
                                               gint32                        field,
                                               gint32                        start,
                                               gint32                        limit,
                                               GError                      **error);

G_DEFINE_AUTOPTR_CLEANUP_FUNC (IcuConstrainedFieldPosition, icu_constrained_field_position_unref)

G_END_DECLS
