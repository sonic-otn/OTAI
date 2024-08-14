/**
 * Copyright (c) 2021 Alibaba Group.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License"); you may
 *    not use this file except in compliance with the License. You may obtain
 *    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 *    THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR
 *    CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 *    LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
 *    FOR A PARTICULAR PURPOSE, MERCHANTABILITY OR NON-INFRINGEMENT.
 *
 *    See the Apache Version 2.0 License for specific language governing
 *    permissions and limitations under the License.
 *
 * @file    otaiobject.h
 *
 * @brief   This module defines OTAI APIs for bulk retrieval for each object-type
 */

#if !defined (__OTAIOBJECT_H_)
#define __OTAIOBJECT_H_

#include <otaitypes.h>

/**
 * @defgroup OTAIOBJECT OTAI - Object API definitions.
 *
 * @{
 */

/**
 * @extraparam otai_object_type_t object_type
 */
typedef union _otai_object_key_entry_t
{
    /**
     * @brief Key is object ID.
     *
     * @validonly otai_metadata_is_object_type_oid(object_type) == true
     */
    otai_object_id_t           object_id;

} otai_object_key_entry_t;

/**
 * @brief Structure for bulk retrieval of object ids, attribute and values for
 * each object-type. Key will be used in case of object-types not having
 * object-ids.
 *
 * @extraparam otai_object_type_t object_type
 */
typedef struct _otai_object_key_t
{
    /** @passparam object_type */
    otai_object_key_entry_t key;

} otai_object_key_t;

/**
 * @}
 */
#endif /** __OTAIOBJECT_H_ */
