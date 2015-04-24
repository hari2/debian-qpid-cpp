#ifndef QPID_HA_BACKUPCONNECTIONEXCLUDER_H
#define QPID_HA_BACKUPCONNECTIONEXCLUDER_H

/*
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 */

<<<<<<< HEAD
=======
#include "LogPrefix.h"
>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include "qpid/broker/ConnectionObserver.h"
#include "qpid/broker/Connection.h"
#include "qpid/log/Statement.h"

namespace qpid {
namespace ha {

/**
 * Exclude connections to a backup broker.
 */
class BackupConnectionExcluder : public broker::ConnectionObserver
{
  public:
<<<<<<< HEAD
    void opened(broker::Connection& connection) {
        QPID_LOG(debug, "Backup: Rejected connection "+connection.getMgmtId());
=======
    BackupConnectionExcluder(const LogPrefix& lp) : logPrefix(lp) {}

    void opened(broker::Connection& connection) {
        QPID_LOG(trace, logPrefix << "Rejected connection "+connection.getMgmtId());
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        connection.abort();
    }

    void closed(broker::Connection&) {}
<<<<<<< HEAD
=======

  private:
    const LogPrefix& logPrefix;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
};

}} // namespace qpid::ha

#endif  /*!QPID_HA_BACKUPCONNECTIONEXCLUDER_H*/
