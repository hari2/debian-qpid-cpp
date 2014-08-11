/*
 *
 * Copyright (c) 2006 The Apache Software Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

/*
 * This file is automatically generated and will be overwritten by the
 * next CMake invocation.
 */

#ifndef QPID_CONFIG_H
#define QPID_CONFIG_H

// PACKAGE_NAME and PACKAGE_VERSION are carry-overs from the autoconf world.
// They tend to cause confusion and problems when mixing headers from multiple
// autoconf-configured packages, so it's best to remove these in favor of
// Qpid-specific names as soon as the autoconf stuff is removed.
#define PACKAGE_NAME "${CMAKE_PROJECT_NAME}"
#define PACKAGE_VERSION "${qpidc_version}"

#cmakedefine QPIDC_CONF_FILE "${QPIDC_CONF_FILE}"
#cmakedefine QPIDD_CONF_FILE "${QPIDD_CONF_FILE}"

#cmakedefine QPIDC_MODULE_DIR "${QPIDC_MODULE_DIR}"
#cmakedefine QPIDD_MODULE_DIR "${QPIDD_MODULE_DIR}"

#define QPID_SHLIB_PREFIX "${CMAKE_SHARED_LIBRARY_PREFIX}"
#define QPID_MODULE_PREFIX
#cmakedefine QPID_DEBUG_POSTFIX "${QPID_DEBUG_POSTFIX}"
#if defined(QPID_DEBUG_POSTFIX) && defined (_DEBUG)
#  define QPID_SHLIB_POSTFIX QPID_DEBUG_POSTFIX
#  define QPID_MODULE_POSTFIX QPID_DEBUG_POSTFIX
#else
#  define QPID_SHLIB_POSTFIX
#  define QPID_MODULE_POSTFIX
#endif
#define QPID_SHLIB_SUFFIX "${CMAKE_SHARED_LIBRARY_SUFFIX}"
#define QPID_MODULE_SUFFIX "${CMAKE_SHARED_LIBRARY_SUFFIX}"

#define BROKER_SASL_NAME "${QPID_BROKER_SASL_NAME}"
#cmakedefine HAVE_SASL ${HAVE_SASL}

#cmakedefine HAVE_SYS_SDT_H ${HAVE_SYS_SDT_H}
#cmakedefine HAVE_LOG_AUTHPRIV
#cmakedefine HAVE_LOG_FTP
#cmakedefine HAVE_PROTON_TRACER

#endif /* QPID_CONFIG_H */
