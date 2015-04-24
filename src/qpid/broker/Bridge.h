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
#ifndef _Bridge_
#define _Bridge_

#include "qpid/broker/PersistableConfig.h"
#include "qpid/framing/AMQP_ServerProxy.h"
#include "qpid/framing/ChannelHandler.h"
#include "qpid/framing/Buffer.h"
#include "qpid/framing/FrameHandler.h"
#include "qpid/framing/FieldTable.h"
#include "qpid/management/Manageable.h"
#include "qpid/broker/Exchange.h"
#include "qpid/broker/SessionHandler.h"
#include "qmf/org/apache/qpid/broker/ArgsLinkBridge.h"
#include "qmf/org/apache/qpid/broker/Bridge.h"

#include <boost/function.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <memory>

namespace qpid {
namespace broker {
<<<<<<< HEAD

class Connection;
class ConnectionState;
=======
namespace amqp_0_10 {
class Connection;
}
>>>>>>> 3bbfc42... Imported Upstream version 0.32
class Link;
class LinkRegistry;

class Bridge : public PersistableConfig,
               public management::Manageable,
               public Exchange::DynamicBridge,
               public SessionHandler::ErrorListener,
               public boost::enable_shared_from_this<Bridge>
{
  public:
    typedef boost::shared_ptr<Bridge> shared_ptr;
    typedef boost::function<void(Bridge*)> CancellationListener;
    typedef boost::function<void(Bridge&, SessionHandler&)> InitializeCallback;

    Bridge(const std::string& name, Link* link, framing::ChannelId id, CancellationListener l,
           const qmf::org::apache::qpid::broker::ArgsLinkBridge& args,
           InitializeCallback init, const std::string& queueName="",
           const std::string& altExchange=""
    );
    ~Bridge();

    QPID_BROKER_EXTERN void close();
    bool isDurable() { return args.i_durable; }
<<<<<<< HEAD
=======
    framing::ChannelId getChannel() const { return channel; }
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    Link *getLink() const { return link; }
    const std::string getSrc() const { return args.i_src; }
    const std::string getDest() const { return args.i_dest; }
    const std::string getKey() const { return args.i_key; }

    bool isDetached() const { return detached; }

<<<<<<< HEAD
    management::ManagementObject* GetManagementObject() const;
=======
    management::ManagementObject::shared_ptr GetManagementObject() const;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    management::Manageable::status_t ManagementMethod(uint32_t methodId,
                                                      management::Args& args,
                                                      std::string& text);

    // PersistableConfig:
    void     setPersistenceId(uint64_t id) const;
    uint64_t getPersistenceId() const { return persistenceId; }
    uint32_t encodedSize() const;
    void     encode(framing::Buffer& buffer) const;
    const std::string& getName() const { return name; }

    static const std::string ENCODED_IDENTIFIER;
    static const std::string ENCODED_IDENTIFIER_V1;
    static Bridge::shared_ptr decode(LinkRegistry& links, framing::Buffer& buffer);
    static bool isEncodedBridge(const std::string& key);

    // Exchange::DynamicBridge methods
    void propagateBinding(const std::string& key, const std::string& tagList, const std::string& op, const std::string& origin, qpid::framing::FieldTable* extra_args=0);
    void sendReorigin();
    void ioThreadPropagateBinding(const std::string& queue, const std::string& exchange, const std::string& key, framing::FieldTable args);
    bool containsLocalTag(const std::string& tagList) const;
    const std::string& getLocalTag() const;

    // Methods needed by initialization functions
    std::string getQueueName() const { return queueName; }
    const qmf::org::apache::qpid::broker::ArgsLinkBridge& getArgs() { return args; }

    /** create a name for a bridge (if none supplied by user config) */
    static std::string createName(const std::string& linkName,
                                  const std::string& src,
                                  const std::string& dest,
                                  const std::string& key);

    // SessionHandler::ErrorListener methods.
    void connectionException(framing::connection::CloseCode code, const std::string& msg);
    void channelException(framing::session::DetachCode, const std::string& msg);
    void executionException(framing::execution::ErrorCode, const std::string& msg);
<<<<<<< HEAD
=======
    void incomingExecutionException(framing::execution::ErrorCode, const std::string& msg);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    void detach();

    void setErrorListener(boost::shared_ptr<ErrorListener> e) { errorListener = e; }
  private:
    struct PushHandler : framing::FrameHandler {
<<<<<<< HEAD
        PushHandler(Connection* c) { conn = c; }
        void handle(framing::AMQFrame& frame);
        Connection* conn;
=======
        PushHandler(amqp_0_10::Connection* c) { conn = c; }
        void handle(framing::AMQFrame& frame);
        amqp_0_10::Connection* conn;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    };

    std::auto_ptr<PushHandler>                        pushHandler;
    std::auto_ptr<framing::ChannelHandler>            channelHandler;
    std::auto_ptr<framing::AMQP_ServerProxy::Session> session;
    std::auto_ptr<framing::AMQP_ServerProxy>          peer;

    Link* const link;
    const framing::ChannelId          channel;
    qmf::org::apache::qpid::broker::ArgsLinkBridge args;
<<<<<<< HEAD
    qmf::org::apache::qpid::broker::Bridge*        mgmtObject;
=======
    qmf::org::apache::qpid::broker::Bridge::shared_ptr mgmtObject;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    CancellationListener        listener;
    std::string name;
    std::string queueName;
    std::string altEx;
    mutable uint64_t  persistenceId;
<<<<<<< HEAD
    ConnectionState* connState;
    Connection* conn;
=======
    amqp_0_10::Connection* conn;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    InitializeCallback initialize;
    bool detached;              // Set when session is detached.
    bool resetProxy();

    // connection Management (called by owning Link)
<<<<<<< HEAD
    void create(Connection& c);
    void cancel(Connection& c);
=======
    void create(amqp_0_10::Connection& c);
    void cancel(amqp_0_10::Connection& c);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    void closed();
    friend class Link; // to call create, cancel, closed()
    boost::shared_ptr<ErrorListener> errorListener;

    const bool useExistingQueue;
    const std::string sessionName;
};


}}

#endif
