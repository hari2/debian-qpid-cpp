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
#ifndef _RecoveredDequeue_
#define _RecoveredDequeue_

#include "qpid/broker/Deliverable.h"
#include "qpid/broker/Message.h"
#include "qpid/broker/MessageStore.h"
#include "qpid/broker/TxOp.h"

<<<<<<< HEAD
#include <boost/intrusive_ptr.hpp>

=======
>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include <algorithm>
#include <functional>
#include <list>

namespace qpid {
    namespace broker {
        class RecoveredDequeue : public TxOp{
            boost::shared_ptr<Queue> queue;
<<<<<<< HEAD
            boost::intrusive_ptr<Message> msg;

        public:
            RecoveredDequeue(boost::shared_ptr<Queue> queue, boost::intrusive_ptr<Message> msg);
            virtual bool prepare(TransactionContext* ctxt) throw();
            virtual void commit() throw();
            virtual void rollback() throw();
            virtual ~RecoveredDequeue(){}
            virtual void accept(TxOpConstVisitor& visitor) const { visitor(*this); }

            boost::shared_ptr<Queue> getQueue() const { return queue; }
            boost::intrusive_ptr<Message> getMessage() const { return msg; }
=======
            Message msg;

        public:
            RecoveredDequeue(boost::shared_ptr<Queue> queue, Message msg);
            virtual bool prepare(TransactionContext* ctxt) throw();
            virtual void commit() throw();
            virtual void rollback() throw();
            // TODO aconway 2013-07-08: revisit
            virtual void callObserver(const boost::shared_ptr<TransactionObserver>&) {}
            virtual ~RecoveredDequeue(){}

            boost::shared_ptr<Queue> getQueue() const { return queue; }
            Message getMessage() const { return msg; }
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        };
    }
}


#endif
