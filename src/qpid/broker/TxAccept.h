/*
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
<<<<<<< HEAD
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
=======
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
>>>>>>> 3bbfc42... Imported Upstream version 0.32
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 */
#ifndef _TxAccept_
#define _TxAccept_

<<<<<<< HEAD
#include <algorithm>
#include <functional>
#include <list>
#include "qpid/framing/SequenceSet.h"
#include "qpid/broker/DeliveryRecord.h"
#include "qpid/broker/TxOp.h"

namespace qpid {
    namespace broker {
        /**
         * Defines the transactional behaviour for accepts received by
         * a transactional channel.
         */
        class TxAccept : public TxOp {
            struct RangeOp
            {
                AckRange range;
    
                RangeOp(const AckRange& r);
                void prepare(TransactionContext* ctxt);
                void commit();
            };

            struct RangeOps
            {
                std::vector<RangeOp> ranges;
                DeliveryRecords& unacked;
    
                RangeOps(DeliveryRecords& u);

                void operator()(framing::SequenceNumber start, framing::SequenceNumber end);
                void prepare(TransactionContext* ctxt);
                void commit();    
            };

            framing::SequenceSet acked;
            DeliveryRecords& unacked;
            RangeOps ops;

        public:
            /**
             * @param acked a representation of the accumulation of
             * acks received
             * @param unacked the record of delivered messages
             */
            TxAccept(const framing::SequenceSet& acked, DeliveryRecords& unacked);
            virtual bool prepare(TransactionContext* ctxt) throw();
            virtual void commit() throw();
            virtual void rollback() throw();
            virtual ~TxAccept(){}
            virtual void accept(TxOpConstVisitor& visitor) const { visitor(*this); }

            // Used by cluster replication.
            const framing::SequenceSet& getAcked() const { return acked; }
        };
    }
=======
#include "qpid/framing/SequenceSet.h"
#include "qpid/broker/BrokerImportExport.h"
#include "qpid/broker/DeliveryRecord.h"
#include "qpid/broker/TxOp.h"
#include <boost/function.hpp>
#include <algorithm>
#include <functional>
#include <list>

namespace qpid {
namespace broker {
/**
 * Defines the transactional behaviour for accepts received by
 * a transactional channel.
 */
class TxAccept : public TxOp {
    typedef boost::shared_ptr<TransactionObserver> ObserverPtr;

    void each(boost::function<void(DeliveryRecord&)>);

    framing::SequenceSet acked;
    DeliveryRecords& unacked;

  public:
    /**
     * @param acked a representation of the accumulation of
     * acks received
     * @param unacked the record of delivered messages
     */
    QPID_BROKER_EXTERN TxAccept(const framing::SequenceSet& acked, DeliveryRecords& unacked);
    virtual bool prepare(TransactionContext* ctxt) throw();
    virtual void commit() throw();
    virtual void rollback() throw();
    virtual void callObserver(const ObserverPtr&);
    virtual ~TxAccept(){}
};
}
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}


#endif
