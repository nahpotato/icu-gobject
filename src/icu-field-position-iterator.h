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

#define ICU_TYPE_FIELD_POSITION_ITERATOR (icu_field_position_iterator_get_type())

typedef struct _IcuFieldPositionIterator IcuFieldPositionIterator;

ICU_AVAILABLE_IN_ALL
GType icu_field_position_iterator_get_type (void);

ICU_AVAILABLE_IN_ALL
IcuFieldPositionIterator *icu_field_position_iterator_new (GError **error);

ICU_AVAILABLE_IN_ALL
IcuFieldPositionIterator *icu_field_position_iterator_ref   (IcuFieldPositionIterator *self);
ICU_AVAILABLE_IN_ALL
void                      icu_field_position_iterator_unref (IcuFieldPositionIterator *self);

ICU_AVAILABLE_IN_ALL
gint32 icu_field_position_iterator_next (IcuFieldPositionIterator *self,
                                         gint32                   *begin_index,
                                         gint32                   *end_index);

G_DEFINE_AUTOPTR_CLEANUP_FUNC (IcuFieldPositionIterator, icu_field_position_iterator_unref)

G_END_DECLS
