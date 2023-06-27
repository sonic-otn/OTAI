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
 * @file    otaistatus.h
 * @brief   This module defines OTAI Status codes OTAI
 */

#if !defined (__OTAISTATUS_H_)
#define __OTAISTATUS_H_

/**
 * @defgroup OTAISTATUS OTAI - Status codes definitions
 *
 * @{
 */

#ifdef _WIN32

#define OTAI_STATUS_CODE(_S_)    ((otai_status_t)(_S_))

#else

/**
 * @brief Any mapping schema from _S_ to error codes convenient on *nix can be used.
 */
#define OTAI_STATUS_CODE(_S_)    (-(_S_))

#endif

/**
 * @brief Status success
 */
#define OTAI_STATUS_SUCCESS                                          0x00000000L

/**
 * @brief General failure
 */
#define OTAI_STATUS_FAILURE                          OTAI_STATUS_CODE(0x00000001L)

/**
 * @brief The request is not supported
 */
#define OTAI_STATUS_NOT_SUPPORTED                    OTAI_STATUS_CODE(0x00000002L)

/**
 * @brief Not enough memory to complete the operation
 */
#define OTAI_STATUS_NO_MEMORY                        OTAI_STATUS_CODE(0x00000003L)

/**
 * @brief Insufficient system resources exist to complete the operation
 */
#define OTAI_STATUS_INSUFFICIENT_RESOURCES           OTAI_STATUS_CODE(0x00000004L)

/**
 * @brief An invalid parameter was passed to a function
 */
#define OTAI_STATUS_INVALID_PARAMETER                OTAI_STATUS_CODE(0x00000005L)

/**
 * @brief An item already exists. For example, create a route which already
 * exists.
 */
#define OTAI_STATUS_ITEM_ALREADY_EXISTS              OTAI_STATUS_CODE(0x00000006L)

/**
 * @brief An item was not found. For example, remove a route which does not
 * exist.
 */
#define OTAI_STATUS_ITEM_NOT_FOUND                   OTAI_STATUS_CODE(0x00000007L)

/**
 * @brief The data was too large to fit into the specified buffer.
 */
#define OTAI_STATUS_BUFFER_OVERFLOW                  OTAI_STATUS_CODE(0x00000008L)

/**
 * @brief Invalid port number
 */
#define OTAI_STATUS_INVALID_PORT_NUMBER              OTAI_STATUS_CODE(0x00000009L)

/**
 * @brief Invalid port member
 */
#define OTAI_STATUS_INVALID_PORT_MEMBER              OTAI_STATUS_CODE(0x0000000AL)

/**
 * @brief Object is uninitialized
 */
#define OTAI_STATUS_UNINITIALIZED                    OTAI_STATUS_CODE(0x0000000CL)

/**
 * @brief Table is full
 */
#define OTAI_STATUS_TABLE_FULL                       OTAI_STATUS_CODE(0x0000000DL)

/**
 * @brief Mandatory attribute is missing
 */
#define OTAI_STATUS_MANDATORY_ATTRIBUTE_MISSING      OTAI_STATUS_CODE(0x0000000EL)

/**
 * @brief Function is not implemented
 */
#define OTAI_STATUS_NOT_IMPLEMENTED                  OTAI_STATUS_CODE(0x0000000FL)

/**
 * @brief Address not found
 */
#define OTAI_STATUS_ADDR_NOT_FOUND                   OTAI_STATUS_CODE(0x00000010L)

/**
 * @brief OTAI Object is in use
 */
#define OTAI_STATUS_OBJECT_IN_USE                    OTAI_STATUS_CODE(0x00000011L)

/**
 * @brief Invalid OTAI Object type passed to a function.
 *
 * If the object id is passed as an attribute value in list, then
 * #OTAI_STATUS_INVALID_ATTR_VALUE_0 status code should be used.
 */
#define OTAI_STATUS_INVALID_OBJECT_TYPE              OTAI_STATUS_CODE(0x00000012L)

/**
 * @brief Invalid OTAI Object ID passed to a function.
 *
 * Return #OTAI_STATUS_INVALID_OBJECT_TYPE when the object type is invalid,
 * otherwise return this #OTAI_STATUS_INVALID_OBJECT_ID. This can happen if the
 * object id is already deleted.
 */
#define OTAI_STATUS_INVALID_OBJECT_ID                OTAI_STATUS_CODE(0x00000013L)

/**
 * @brief The NV storage used is either invalid or corrupt. (rv for
 *        initialize_module())
 */
#define OTAI_STATUS_INVALID_NV_STORAGE               OTAI_STATUS_CODE(0x00000014L)

/**
 * @brief The NV storage is full. (rv for shutdown_module())
 */
#define OTAI_STATUS_NV_STORAGE_FULL                  OTAI_STATUS_CODE(0x00000015L)

/**
 * @brief Version mismatch in case of Inservice upgrade (rv for
 *        initialize_module())
 */
#define OTAI_STATUS_SW_UPGRADE_VERSION_MISMATCH      OTAI_STATUS_CODE(0x00000016L)

/**
 * @brief Operation not executed
 */
#define OTAI_STATUS_NOT_EXECUTED                     OTAI_STATUS_CODE(0x00000017L)

/**
 * @brief When admin status is up, operation is forbidden.
 */
#define OTAI_STATUS_ADMIN_IS_UP                      OTAI_STATUS_CODE(0x00000018L)

/**
 * @brief The hardware state does not match expectations.
 */
#define OTAI_STATUS_HARDWARE_STATE_MISMATCH          OTAI_STATUS_CODE(0x00000019L)

/**
 * @brief OTAI Object is not ready.
 */
#define OTAI_STATUS_OBJECT_NOT_READY                 OTAI_STATUS_CODE(0x0000001AL)

/**
 * @brief The linecard type does not match expectation.
 */
#define OTAI_STATUS_LINECARD_TYPE_MISMATCH           OTAI_STATUS_CODE(0x0000001BL)

/**
 * @brief Attribute is invalid
 *
 * Range from 0x00010000L to 0x0001FFFFL.
 *
 * Return this when the property attached to the attribute does not match
 * the API call.
 *
 * For example:
 * - When a READ_ONLY attribute is passed set_attribute or create call
 * - When a CREATE_ONLY attribute is passed to set_attribute call
 *
 * The offset from #OTAI_STATUS_INVALID_ATTRIBUTE_0 is the array index of
 * the attribute list passed to the OTAI API call.
 */
#define OTAI_STATUS_INVALID_ATTRIBUTE_0              OTAI_STATUS_CODE(0x00010000L)

/**
 * @brief End of invalid attribute range
 */
#define OTAI_STATUS_INVALID_ATTRIBUTE_MAX            OTAI_STATUS_CODE(0x0001FFFFL)

/**
 * @brief Invalid attribute value
 *
 * Range from 0x00020000L to 0x0002FFFFL.
 */
#define OTAI_STATUS_INVALID_ATTR_VALUE_0             OTAI_STATUS_CODE(0x00020000L)

/**
 * @brief End of invalid attribute value range
 */
#define OTAI_STATUS_INVALID_ATTR_VALUE_MAX           OTAI_STATUS_CODE(0x0002FFFFL)

/**
 * @brief Attribute is not implemented
 *
 * Range from 0x00030000L to 0x003FFFFL.
 *
 * Return this when the attribute is supported but not implemented on
 * the platform.
 */
#define OTAI_STATUS_ATTR_NOT_IMPLEMENTED_0           OTAI_STATUS_CODE(0x00030000L)

/**
 * @brief End of not implemented range
 */
#define OTAI_STATUS_ATTR_NOT_IMPLEMENTED_MAX         OTAI_STATUS_CODE(0x0003FFFFL)

/**
 * @brief Attribute is unknown
 *
 * Range from 0x00040000L to 0x004FFFFL.
 *
 * Return this when the attribute is undefined, e.g., the attribute id
 * is out of range.
 */
#define OTAI_STATUS_UNKNOWN_ATTRIBUTE_0              OTAI_STATUS_CODE(0x00040000L)

/**
 * @brief End of unknown attribute range
 */
#define OTAI_STATUS_UNKNOWN_ATTRIBUTE_MAX            OTAI_STATUS_CODE(0x0004FFFFL)

/**
 * @brief Attribute is not supported
 *
 * Range from 0x00050000L to 0x005FFFFL.
 *
 * Return this when the platform does not have the capability to support
 * this attribute.
 */
#define OTAI_STATUS_ATTR_NOT_SUPPORTED_0             OTAI_STATUS_CODE(0x00050000L)

/**
 * @brief End of attribute not supported range
 */
#define OTAI_STATUS_ATTR_NOT_SUPPORTED_MAX           OTAI_STATUS_CODE(0x0005FFFFL)

/*
 * Macros to check if attribute related error is within the specified range
 */

/**
 * @brief Is invalid attribute helper
 */
#define OTAI_STATUS_IS_INVALID_ATTRIBUTE(x)      (((x) & (~0xFFFF)) == OTAI_STATUS_INVALID_ATTRIBUTE_0)

/**
 * @brief Is invalid attribute value helper
 */
#define OTAI_STATUS_IS_INVALID_ATTR_VALUE(x)     (((x) & (~0xFFFF)) == OTAI_STATUS_INVALID_ATTR_VALUE_0)

/**
 * @brief Is attribute not implemented helper
 */
#define OTAI_STATUS_IS_ATTR_NOT_IMPLEMENTED(x)   (((x) & (~0xFFFF)) == OTAI_STATUS_ATTR_NOT_IMPLEMENTED_0)

/**
 * @brief Is unknown attribute helper
 */
#define OTAI_STATUS_IS_UNKNOWN_ATTRIBUTE(x)      (((x) & (~0xFFFF)) == OTAI_STATUS_INVALID_ATTRIBUTE_0)

/**
 * @brief Is attribute not supported helper
 */
#define OTAI_STATUS_IS_ATTR_NOT_SUPPORTED(x)     (((x) & (~0xFFFF)) == OTAI_STATUS_ATTR_NOT_SUPPORTED_0)

/**
 * @}
 */
#endif /** __OTAISTATUS_H_ */
