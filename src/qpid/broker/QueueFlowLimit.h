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
#ifndef _QueueFlowLimit_
#define _QueueFlowLimit_

#include <list>
#include <set>
#include <iostream>
#include <memory>
#include "qpid/broker/BrokerImportExport.h"
<<<<<<< HEAD
#include "qpid/broker/QueuedMessage.h"
#include "qpid/broker/StatefulQueueObserver.h"
#include "qpid/framing/FieldTable.h"
#include "qpid/sys/AtomicValue.h"
#include "qpid/sys/Mutex.h"

namespace qmf {
namespace org {
namespace apache {
namespace qpid {
namespace broker {
    class Queue;
}}}}}
=======
#include "qpid/broker/QueueObserver.h"
#include "qpid/framing/FieldTable.h"
#include "qpid/framing/SequenceNumber.h"
#include "qpid/sys/AtomicValue.h"
#include "qpid/sys/Mutex.h"
#include "qmf/org/apache/qpid/broker/Queue.h"

#include <boost/enable_shared_from_this.hpp>

>>>>>>> 3bbfc42... Imported Upstream version 0.32
namespace _qmfBroker = qmf::org::apache::qpid::broker;

namespace qpid {
namespace broker {

class Broker;
<<<<<<< HEAD
=======
class Queue;
class Message;
struct QueueSettings;
>>>>>>> 3bbfc42... Imported Upstream version 0.32

/**
 * Producer flow control: when level is > flowStop*, flow control is ON.
 * then level is < flowResume*, flow control is OFF.  If == 0, flow control
 * is not used.  If both byte and msg count thresholds are set, then
 * passing _either_ level may turn flow control ON, but _both_ must be
 * below level before flow control will be turned OFF.
 */
<<<<<<< HEAD
 class QueueFlowLimit : public StatefulQueueObserver
=======
 class QueueFlowLimit : public QueueObserver, public boost::enable_shared_from_this<QueueFlowLimit>
>>>>>>> 3bbfc42... Imported Upstream version 0.32
{
    static uint64_t defaultMaxSize;
    static uint defaultFlowStopRatio;
    static uint defaultFlowResumeRatio;

    Queue *queue;
    std::string queueName;

    uint32_t flowStopCount;
    uint32_t flowResumeCount;
    uint64_t flowStopSize;
    uint64_t flowResumeSize;
    bool flowStopped;   // true = producers held in flow control

    // current queue utilization
    uint32_t count;
    uint64_t size;

  public:
    static QPID_BROKER_EXTERN const std::string flowStopCountKey;
    static QPID_BROKER_EXTERN const std::string flowResumeCountKey;
    static QPID_BROKER_EXTERN const std::string flowStopSizeKey;
    static QPID_BROKER_EXTERN const std::string flowResumeSizeKey;

    QPID_BROKER_EXTERN virtual ~QueueFlowLimit();

<<<<<<< HEAD
    /** the queue has added QueuedMessage.  Returns true if flow state changes */
    QPID_BROKER_EXTERN void enqueued(const QueuedMessage&);
    /** the queue has removed QueuedMessage.  Returns true if flow state changes */
    QPID_BROKER_EXTERN void dequeued(const QueuedMessage&);
    /** ignored */
    QPID_BROKER_EXTERN void acquired(const QueuedMessage&) {};
    QPID_BROKER_EXTERN void requeued(const QueuedMessage&) {};

    /** for clustering: */
    QPID_BROKER_EXTERN void getState(qpid::framing::FieldTable&) const;
    QPID_BROKER_EXTERN void setState(const qpid::framing::FieldTable&);
=======
    /** the queue has added QueuedMessage */
    QPID_BROKER_EXTERN void enqueued(const Message&);
    /** the queue has removed QueuedMessage */
    QPID_BROKER_EXTERN void dequeued(const Message&);
    /** ignored */
    QPID_BROKER_EXTERN void acquired(const Message&) {};
    QPID_BROKER_EXTERN void requeued(const Message&) {};
>>>>>>> 3bbfc42... Imported Upstream version 0.32

    uint32_t getFlowStopCount() const { return flowStopCount; }
    uint32_t getFlowResumeCount() const { return flowResumeCount; }
    uint64_t getFlowStopSize() const { return flowStopSize; }
    uint64_t getFlowResumeSize() const { return flowResumeSize; }

    uint32_t getFlowCount() const { return count; }
    uint64_t getFlowSize() const { return size; }
    bool isFlowControlActive() const { return flowStopped; }
    bool monitorFlowControl() const { return flowStopCount || flowStopSize; }

    void encode(framing::Buffer& buffer) const;
    void decode(framing::Buffer& buffer);
    uint32_t encodedSize() const;

<<<<<<< HEAD
    static QPID_BROKER_EXTERN void observe(Queue& queue, const qpid::framing::FieldTable& settings);
=======
    QPID_BROKER_EXTERN void observe(Queue& queue);
    static QPID_BROKER_EXTERN boost::shared_ptr<QueueFlowLimit> createLimit(const std::string& queueName, const QueueSettings& settings);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    static QPID_BROKER_EXTERN void setDefaults(uint64_t defaultMaxSize, uint defaultFlowStopRatio, uint defaultFlowResumeRatio);

    friend QPID_BROKER_EXTERN std::ostream& operator<<(std::ostream&, const QueueFlowLimit&);

 protected:
    // msgs waiting for flow to become available.
<<<<<<< HEAD
    std::map<framing::SequenceNumber, boost::intrusive_ptr<Message> > index;
    mutable qpid::sys::Mutex indexLock;

    _qmfBroker::Queue *queueMgmtObj;

    const Broker *broker;

    QPID_BROKER_EXTERN QueueFlowLimit(Queue *queue,
                   uint32_t flowStopCount, uint32_t flowResumeCount,
                   uint64_t flowStopSize,  uint64_t flowResumeSize);
    static QPID_BROKER_EXTERN QueueFlowLimit *createLimit(Queue *queue, const qpid::framing::FieldTable& settings);
=======
    std::map<framing::SequenceNumber, Message > index;
    mutable qpid::sys::Mutex indexLock;

    _qmfBroker::Queue::shared_ptr queueMgmtObj;

    const Broker *broker;

    QPID_BROKER_EXTERN QueueFlowLimit(const std::string& _queueName,
                   uint32_t _flowStopCount, uint32_t _flowResumeCount,
                   uint64_t _flowStopSize,  uint64_t _flowResumeSize);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
};

}}


#endif
