#ifndef QPID_BROKER_MESSAGES_H
#define QPID_BROKER_MESSAGES_H

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
#include "qpid/framing/SequenceNumber.h"
#include <boost/function.hpp>

namespace qpid {
namespace framing {
class SequenceNumber;
}
namespace broker {
<<<<<<< HEAD
struct QueuedMessage;
=======
class Message;
class QueueCursor;
>>>>>>> 3bbfc42... Imported Upstream version 0.32

/**
 * This interface abstracts out the access to the messages held for
 * delivery by a Queue instance. Note the the assumption at present is
 * that all locking is done in the Queue itself.
 */
class Messages
{
  public:
<<<<<<< HEAD
    typedef boost::function1<void, QueuedMessage&> Functor;
    typedef boost::function1<bool, QueuedMessage&> Predicate;
=======
    typedef boost::function1<void, Message&> Functor;
>>>>>>> 3bbfc42... Imported Upstream version 0.32

    virtual ~Messages() {}
    /**
     * @return the number of messages available for delivery.
     */
    virtual size_t size() = 0;

    /**
     * Called when a message is deleted from the queue.
     */
<<<<<<< HEAD
    virtual bool deleted(const QueuedMessage&) = 0;
    /**
     * Releases an acquired message, making it available again.
     */
    virtual void release(const QueuedMessage&) = 0;
    /**
     * Acquire the message at the specified position, returning true
     * if found, false otherwise. The acquired message is passed back
     * via the second parameter.
     */
    virtual bool acquire(const framing::SequenceNumber&, QueuedMessage&) = 0;
    /**
     * Find the message at the specified position, returning true if
     * found, false otherwise. The matched message is passed back via
     * the second parameter.
     */
    virtual bool find(const framing::SequenceNumber&, QueuedMessage&) = 0;
    /**
     * Retrieve the next message to be given to a browsing
     * subscription that has reached the specified position. The next
     * message is passed back via the second parameter.
     *
     * @param unacquired, if true, will only browse unacquired messages
     *
     * @return true if there is another message, false otherwise.
     */
    virtual bool browse(const framing::SequenceNumber&, QueuedMessage&, bool unacquired) = 0;
    /**
     * Retrieve the next message available for a consuming
     * subscription.
     *
     * @return true if there is such a message, false otherwise.
     */
    virtual bool consume(QueuedMessage&) = 0;
    /**
     * Pushes a message to the back of the 'queue'. For some types of
     * queue this may cause another message to be removed; if that is
     * the case the method will return true and the removed message
     * will be passed out via the second parameter.
     */
    virtual bool push(const QueuedMessage& added, QueuedMessage& removed) = 0;

    /**
     * Add an already acquired message to the queue.
     * Used by a cluster updatee to replicate acquired messages from the updater.
     * Only need be implemented by subclasses that keep track of
     * acquired messages.
     */
    virtual void updateAcquired(const QueuedMessage&) { }

    /**
     * Set the position of the back of the queue. Next message enqueued will be n+1.
     *@pre Any messages with seq > n must already be dequeued.
     */
    virtual void setPosition(const framing::SequenceNumber& /*n*/) = 0;
=======
    virtual bool deleted(const QueueCursor&) = 0;
    /**
     * Makes a message available.
     */
    virtual void publish(const Message& added) = 0;
    /**
     * Retrieve the next message for the given cursor. A reference to
     * the message is passed back via the second parameter.
     *
     * @return a pointer to the message if there is one, in which case
     * the cursor that points to it is assigned to cursor; null
     * otherwise.
     */
    virtual Message* next(QueueCursor& cursor) = 0;

    /**
     * Release the message i.e. return it to the available state
     * unless it has already been deleted.
     *
     * @return a pointer to the Message if it is still in acquired state and
     * hence can be released; null if it has already been deleted
     */
    virtual Message* release(const QueueCursor& cursor) = 0;
    /**
     * Find the message with the specified sequence number, returning
     * a pointer if found, null otherwise. A cursor to the matched
     * message can be passed back via the second parameter, regardless
     * of whether the message is found, using this cursor to call
     * next() will give the next message greater than position if one
     * exists.
     */
    virtual Message* find(const framing::SequenceNumber&, QueueCursor*) = 0;

    /**
     * Find the message at the specified position, returning a pointer if
     * found, null otherwise.
     */
    virtual Message* find(const QueueCursor&) = 0;
>>>>>>> 3bbfc42... Imported Upstream version 0.32

    /**
     * Apply, the functor to each message held
     */
<<<<<<< HEAD

    virtual void foreach(Functor) = 0;
    /**
     * Remove every message held that for which the specified
     * predicate returns true
     */
    virtual void removeIf(Predicate) = 0;

=======
    virtual void foreach(Functor) = 0;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
  private:
};
}} // namespace qpid::broker

#endif  /*!QPID_BROKER_MESSAGES_H*/
