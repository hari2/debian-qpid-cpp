/*
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownersip.  The ASF licenses this file
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
#include "qpid/broker/PriorityQueue.h"
<<<<<<< HEAD
#include "qpid/broker/Queue.h"
#include "qpid/broker/QueuedMessage.h"
#include "qpid/framing/reply_exceptions.h"
#include "qpid/log/Statement.h"
#include <cmath>

namespace qpid {
namespace broker {

PriorityQueue::PriorityQueue(int l) :
    levels(l),
    messages(levels, Deque()),
    frontLevel(0), haveFront(false), cached(false) {}

bool PriorityQueue::deleted(const QueuedMessage& qm) {
    bool deleted = fifo.deleted(qm);
    if (deleted) erase(qm);
    return deleted;
=======
#include "qpid/broker/Message.h"
#include "qpid/broker/Queue.h"
#include "qpid/broker/QueueCursor.h"
#include "qpid/framing/reply_exceptions.h"
#include "qpid/log/Statement.h"
#include <algorithm>
#include <cmath>
#include <boost/bind.hpp>

namespace qpid {
namespace broker {
namespace {
class PriorityContext : public CursorContext {
  public:
    std::vector<QueueCursor> position;
    PriorityContext(size_t levels, SubscriptionType type) : position(levels, QueueCursor(type)) {}
};
}


PriorityQueue::PriorityQueue(int l) :
    levels(l),
    messages(levels, Deque(boost::bind(&PriorityQueue::priorityPadding, this, _1))),
    counters(levels, framing::SequenceNumber()),
    fifo(boost::bind(&PriorityQueue::fifoPadding, this, _1)),
    frontLevel(0), haveFront(false), cached(false)
{
}

bool PriorityQueue::deleted(const QueueCursor& c)
{
    MessagePointer* ptr = fifo.find(c);
    if (ptr && ptr->holder) {
        //mark the message as deleted
        ptr->holder->message.setState(DELETED);
        //clean the deque for the relevant priority level
        boost::shared_ptr<PriorityContext> ctxt = boost::dynamic_pointer_cast<PriorityContext>(c.context);
        messages[ptr->holder->priority].clean();
        //stop referencing that message holder (it may now have been
        //deleted)
        ptr->holder = 0;
        //clean fifo index
        fifo.clean();
        return true;
    } else {
        return false;
    }
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

size_t PriorityQueue::size()
{
    return fifo.size();
}

<<<<<<< HEAD
namespace {
bool before(QueuedMessage* a, QueuedMessage* b) { return *a < *b; }
}

void PriorityQueue::release(const QueuedMessage& message)
{
    QueuedMessage* qm = fifo.releasePtr(message);
    if (qm) {
        uint p = getPriorityLevel(message);
        messages[p].insert(
            lower_bound(messages[p].begin(), messages[p].end(), qm, before), qm);
        clearCache();
    }
}


void PriorityQueue::erase(const QueuedMessage& qm) {
    size_t i = getPriorityLevel(qm);
    if (!messages[i].empty()) {
        long diff = qm.position.getValue() - messages[i].front()->position.getValue();
        if (diff < 0) return;
        long maxEnd = std::min(size_t(diff), messages[i].size());
        QueuedMessage mutableQm = qm; // need non-const qm for lower_bound
        Deque::iterator l =
            lower_bound(messages[i].begin(),messages[i].begin()+maxEnd, &mutableQm, before);
        if (l != messages[i].end() && (*l)->position == qm.position) {
            messages[i].erase(l);
            clearCache();
            return;
        }
    }
}

bool PriorityQueue::acquire(const framing::SequenceNumber& position, QueuedMessage& message)
{
    bool acquired = fifo.acquire(position, message);
    if (acquired) erase(message); // No longer available
    return acquired;
}

bool PriorityQueue::find(const framing::SequenceNumber& position, QueuedMessage& message)
{
    return fifo.find(position, message);
}

bool PriorityQueue::browse(
    const framing::SequenceNumber& position, QueuedMessage& message, bool unacquired)
{
    return fifo.browse(position, message, unacquired);
}

bool PriorityQueue::consume(QueuedMessage& message)
{
    if (checkFront()) {
        QueuedMessage* pm = messages[frontLevel].front();
        messages[frontLevel].pop_front();
        clearCache();
        pm->status = QueuedMessage::ACQUIRED; // Updates FIFO index
        message = *pm;
        return true;
    } else {
        return false;
    }
}

bool PriorityQueue::push(const QueuedMessage& added, QueuedMessage& /*not needed*/)
{
    QueuedMessage* qmp = fifo.pushPtr(added);
    messages[getPriorityLevel(added)].push_back(qmp);
    clearCache();
    return false; // Adding a message never causes one to be removed for deque
}

void PriorityQueue::updateAcquired(const QueuedMessage& acquired) {
    fifo.updateAcquired(acquired);
}

void PriorityQueue::setPosition(const framing::SequenceNumber& n) {
    fifo.setPosition(n);
}

=======
Message* PriorityQueue::next(QueueCursor& cursor)
{
    boost::shared_ptr<PriorityContext> ctxt = boost::dynamic_pointer_cast<PriorityContext>(cursor.context);
    if (!ctxt) {
        ctxt = boost::shared_ptr<PriorityContext>(new PriorityContext(levels, CONSUMER));
        cursor.context = ctxt;
    }
    if (cursor.type == REPLICATOR) {
        //browse in fifo order
        MessagePointer* ptr = fifo.next(cursor);
        return ptr ? &(ptr->holder->message) : 0;
    } else if (cursor.type == PURGE) {
        //iterate over message in reverse priority order (i.e. purge lowest priority message first)
        //ignore any fairshare configuration here as well
        for (int p = 0; p < levels; ++p) {
            MessageHolder* holder = messages[p].next(ctxt->position[p]);
            if (holder) {
                cursor.setPosition(holder->message.getSequence(), 0);
                return &(holder->message);
            }
        }
        return 0;
    } else {
        //check each level in turn, in priority order, for any more messages
        Priority p = firstLevel();
        do {
            MessageHolder* holder = messages[p.current].next(ctxt->position[p.current]);
            if (holder) {
                cursor.setPosition(holder->message.getSequence(), 0);
                return &(holder->message);
            }
        } while (nextLevel(p));
        return 0;
    }
}

Message* PriorityQueue::find(const QueueCursor& cursor)
{
    return find(cursor.position, 0);
}

Message* PriorityQueue::find(const framing::SequenceNumber& position, QueueCursor* cursor)
{
    MessagePointer* ptr = fifo.find(position, cursor);
    return ptr ? &(ptr->holder->message) : 0;
}

void PriorityQueue::publish(const Message& published)
{
    MessageHolder holder;
    holder.message = published;
    holder.priority = getPriorityLevel(published);
    holder.id = ++(counters[holder.priority]);
    MessagePointer pointer;
    pointer.holder = &(messages[holder.priority].publish(holder));
    pointer.id = published.getSequence();
    fifo.publish(pointer);
}

Message* PriorityQueue::release(const QueueCursor& cursor)
{
    MessagePointer* ptr = fifo.release(cursor);
    if (ptr) {
        messages[ptr->holder->priority].resetCursors();
        return &(ptr->holder->message);
    } else {
        return 0;
    }
}

>>>>>>> 3bbfc42... Imported Upstream version 0.32
void PriorityQueue::foreach(Functor f)
{
    fifo.foreach(f);
}

<<<<<<< HEAD
void PriorityQueue::removeIf(Predicate p)
{
    for (int priority = 0; priority < levels; ++priority) {
        for (Deque::iterator i = messages[priority].begin(); i != messages[priority].end();) {
            if (p(**i)) {
                (*i)->status = QueuedMessage::DELETED; // Updates fifo index
                i = messages[priority].erase(i);
                clearCache();
            } else {
                ++i;
            }
        }
    }
    fifo.clean();
}

uint PriorityQueue::getPriorityLevel(const QueuedMessage& m) const
{
    uint priority = m.payload->getPriority();
=======
uint PriorityQueue::getPriorityLevel(const Message& m) const
{
    uint priority = m.getPriority();
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    //Use AMQP 0-10 approach to mapping priorities to a fixed level
    //(see rule priority-level-implementation)
    const uint firstLevel = 5 - uint(std::min(5.0, std::ceil((double) levels/2.0)));
    if (priority <= firstLevel) return 0;
    return std::min(priority - firstLevel, (uint)levels-1);
}
<<<<<<< HEAD

void PriorityQueue::clearCache()
{
    cached = false;
}

bool PriorityQueue::findFrontLevel(uint& l, PriorityLevels& m)
{
    for (int p = levels-1; p >= 0; --p) {
        if (!m[p].empty()) {
            l = p;
            return true;
        }
    }
    return false;
}

bool PriorityQueue::checkFront()
{
    if (!cached) {
        haveFront = findFrontLevel(frontLevel, messages);
        cached = true;
    }
    return haveFront;
}

uint PriorityQueue::getPriority(const QueuedMessage& message)
{
    const PriorityQueue* queue = dynamic_cast<const PriorityQueue*>(&(message.queue->getMessages()));
    if (queue) return queue->getPriorityLevel(message);
    else return 0;
}

=======
PriorityQueue::MessagePointer PriorityQueue::fifoPadding(qpid::framing::SequenceNumber id)
{
    PriorityQueue::MessagePointer pointer;
    pointer.holder = 0;
    pointer.id = id;
    return pointer;
}

PriorityQueue::MessageHolder PriorityQueue::priorityPadding(qpid::framing::SequenceNumber id)
{
    PriorityQueue::MessageHolder holder;
    holder.id = id;
    holder.message.setState(DELETED);
    return holder;
}

PriorityQueue::Priority PriorityQueue::firstLevel()
{
    return Priority(levels - 1);
}
bool PriorityQueue::nextLevel(Priority& p)
{
    if (p.current > 0) {
        --(p.current);
        return true;
    } else {
        return false;
    }
}

framing::SequenceNumber PriorityQueue::MessageHolder::getSequence() const
{
    return id;
}
void PriorityQueue::MessageHolder::setState(MessageState s)
{
    message.setState(s);
}
MessageState PriorityQueue::MessageHolder::getState() const
{
    return message.getState();
}
PriorityQueue::MessageHolder::operator Message&()
{
    return message;
}
framing::SequenceNumber PriorityQueue::MessagePointer::getSequence() const
{
    if (holder) {
        return holder->message.getSequence();
    } else {
        //this is used when the instance is merely acting as padding
        return id;
    }
}
void PriorityQueue::MessagePointer::setState(MessageState s)
{
    if (holder) {
        holder->message.setState(s);
    }
}
MessageState PriorityQueue::MessagePointer::getState() const
{
    if (holder) {
        return holder->message.getState();
    } else {
        return DELETED;
    }
}
PriorityQueue::MessagePointer::operator Message&()
{
    assert(holder);
    return holder->message;
}
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}} // namespace qpid::broker
