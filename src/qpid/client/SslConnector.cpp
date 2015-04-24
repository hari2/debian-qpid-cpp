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
#include "qpid/client/Connector.h"

#include "config.h"
#include "qpid/client/Bounds.h"
#include "qpid/client/ConnectionImpl.h"
#include "qpid/client/ConnectionSettings.h"
#include "qpid/Options.h"
#include "qpid/log/Statement.h"
#include "qpid/sys/Time.h"
#include "qpid/framing/AMQFrame.h"
#include "qpid/framing/InitiationHandler.h"
#include "qpid/sys/ssl/util.h"
<<<<<<< HEAD
#include "qpid/sys/ssl/SslIo.h"
#include "qpid/sys/ssl/SslSocket.h"
=======
#include "qpid/sys/AsynchIO.h"
#include "qpid/sys/ssl/SslSocket.h"
#include "qpid/sys/SocketAddress.h"
>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include "qpid/sys/Dispatcher.h"
#include "qpid/sys/Poller.h"
#include "qpid/sys/SecuritySettings.h"
#include "qpid/Msg.h"
<<<<<<< HEAD

#include <iostream>
#include <map>
=======
#include "qpid/types/Exception.h"

#include <iostream>
>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include <boost/bind.hpp>
#include <boost/format.hpp>

namespace qpid {
namespace client {

using namespace qpid::sys;
using namespace qpid::sys::ssl;
using namespace qpid::framing;
using boost::format;
using boost::str;


class SslConnector : public Connector
{
<<<<<<< HEAD
    struct Buff;

    /** Batch up frames for writing to aio. */
    class Writer : public framing::FrameHandler {
        typedef sys::ssl::SslIOBufferBase BufferBase;
        typedef std::vector<framing::AMQFrame> Frames;

        const uint16_t maxFrameSize;
        sys::Mutex lock;
        sys::ssl::SslIO* aio;
        BufferBase* buffer;
        Frames frames;
        size_t lastEof; // Position after last EOF in frames
        framing::Buffer encode;
        size_t framesEncoded;
        std::string identifier;
        Bounds* bounds;

        void writeOne();
        void newBuffer();

      public:

        Writer(uint16_t maxFrameSize, Bounds*);
        ~Writer();
        void init(std::string id, sys::ssl::SslIO*);
        void handle(framing::AMQFrame&);
        void write(sys::ssl::SslIO&);
    };

    const uint16_t maxFrameSize;
    framing::ProtocolVersion version;
    bool initiated;
    SecuritySettings securitySettings;

    sys::Mutex closedLock;
=======
    typedef std::deque<framing::AMQFrame> Frames;

    const uint16_t maxFrameSize;

    sys::Mutex lock;
    Frames frames;
    size_t lastEof; // Position after last EOF in frames
    uint64_t currentSize;
    Bounds* bounds;

    framing::ProtocolVersion version;
    bool initiated;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    bool closed;

    sys::ShutdownHandler* shutdownHandler;
    framing::InputHandler* input;

<<<<<<< HEAD
    Writer writer;

    sys::ssl::SslSocket socket;

    sys::ssl::SslIO* aio;
    Poller::shared_ptr poller;

    ~SslConnector();

    void readbuff(qpid::sys::ssl::SslIO&, qpid::sys::ssl::SslIOBufferBase*);
    void writebuff(qpid::sys::ssl::SslIO&);
    void writeDataBlock(const framing::AMQDataBlock& data);
    void eof(qpid::sys::ssl::SslIO&);
    void disconnected(qpid::sys::ssl::SslIO&);

    std::string identifier;

    void connect(const std::string& host, const std::string& port);
    void init();
    void close();
    void send(framing::AMQFrame& frame);
    void abort() {} // TODO: Need to fix for heartbeat timeouts to work

    void setInputHandler(framing::InputHandler* handler);
    void setShutdownHandler(sys::ShutdownHandler* handler);
    sys::ShutdownHandler* getShutdownHandler() const;
    framing::OutputHandler* getOutputHandler();
    const std::string& getIdentifier() const;
    const SecuritySettings* getSecuritySettings();
    void socketClosed(qpid::sys::ssl::SslIO&, const qpid::sys::ssl::SslSocket&);
=======
    sys::ssl::SslSocket socket;

    sys::AsynchConnector* connector;
    sys::AsynchIO* aio;
    std::string identifier;
    Poller::shared_ptr poller;
    SecuritySettings securitySettings;

    ~SslConnector();

    void readbuff(AsynchIO&, AsynchIOBufferBase*);
    void writebuff(AsynchIO&);
    void writeDataBlock(const framing::AMQDataBlock& data);
    void eof(AsynchIO&);
    void disconnected(AsynchIO&);

    void connect(const std::string& host, const std::string& port);
    void connected(const sys::Socket&);
    void connectFailed(const std::string& msg);

    void close();
    void handle(framing::AMQFrame& frame);
    void abort();
    void connectAborted();

    void setInputHandler(framing::InputHandler* handler);
    void setShutdownHandler(sys::ShutdownHandler* handler);
    const std::string& getIdentifier() const;
    const SecuritySettings* getSecuritySettings();
    void socketClosed(AsynchIO&, const Socket&);

    size_t decode(const char* buffer, size_t size);
    size_t encode(char* buffer, size_t size);
    bool canEncode();
>>>>>>> 3bbfc42... Imported Upstream version 0.32

public:
    SslConnector(Poller::shared_ptr p, framing::ProtocolVersion pVersion,
              const ConnectionSettings&,
              ConnectionImpl*);
};

<<<<<<< HEAD
struct SslConnector::Buff : public SslIO::BufferBase {
    Buff(size_t size) : SslIO::BufferBase(new char[size], size) {}
    ~Buff() { delete [] bytes;}
};

// Static constructor which registers connector here
namespace {
    Connector* create(Poller::shared_ptr p, framing::ProtocolVersion v, const ConnectionSettings& s, ConnectionImpl* c) {
        return new SslConnector(p, v, s, c);
    }

    struct StaticInit {
        StaticInit() {
            try {
                CommonOptions common("", "", QPIDC_CONF_FILE);
                SslOptions options;
                common.parse(0, 0, common.clientConfig, true);
                options.parse (0, 0, common.clientConfig, true);
                if (options.certDbPath.empty()) {
                    QPID_LOG(info, "SSL connector not enabled, you must set QPID_SSL_CERT_DB to enable it.");
                } else {
                    initNSS(options);
                    Connector::registerFactory("ssl", &create);
                }
            } catch (const std::exception& e) {
                QPID_LOG(error, "Failed to initialise SSL connector: " << e.what());
            }
        };

        ~StaticInit() { shutdownNSS(); }
    } init;
=======
// Static constructor which registers connector here
namespace {
    Connector* create(Poller::shared_ptr p, framing::ProtocolVersion v, const ConnectionSettings& s, ConnectionImpl* c);

    struct StaticInit {
        static bool initialised;

        StaticInit() {
            Connector::registerFactory("ssl", &create);
        };
        ~StaticInit() {
            if (initialised) shutdownNSS();
        }

        void checkInitialised() {
            static qpid::sys::Mutex lock;
            qpid::sys::Mutex::ScopedLock l(lock);
            if (!initialised) {
                CommonOptions common("", "", QPIDC_CONF_FILE);
                SslOptions options;
                try {
                    common.parse(0, 0, common.clientConfig, true);
                    options.parse (0, 0, common.clientConfig, true);
                } catch (const std::exception& e) {
                    throw qpid::types::Exception(QPID_MSG("Failed to parse options while initialising SSL connector: " << e.what()));
                }
                if (options.certDbPath.empty()) {
                    throw qpid::types::Exception(QPID_MSG("SSL connector not enabled, you must set QPID_SSL_CERT_DB to enable it."));
                } else {
                    try {
                        initNSS(options);
                        initialised = true;
                    } catch (const std::exception& e) {
                        throw qpid::types::Exception(QPID_MSG("Failed to initialise SSL: " << e.what()));
                    }
                }
            }
        }

    } init;
    bool StaticInit::initialised = false;

    Connector* create(Poller::shared_ptr p, framing::ProtocolVersion v, const ConnectionSettings& s, ConnectionImpl* c) {
        init.checkInitialised();
        return new SslConnector(p, v, s, c);
    }
}

void initialiseSSL()
{
    init.checkInitialised();
}

void shutdownSSL()
{
    if (StaticInit::initialised) shutdownNSS();
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

SslConnector::SslConnector(Poller::shared_ptr p,
                     ProtocolVersion ver,
                     const ConnectionSettings& settings,
                     ConnectionImpl* cimpl)
    : maxFrameSize(settings.maxFrameSize),
<<<<<<< HEAD
=======
      lastEof(0),
      currentSize(0),
      bounds(cimpl),
>>>>>>> 3bbfc42... Imported Upstream version 0.32
      version(ver),
      initiated(false),
      closed(true),
      shutdownHandler(0),
      input(0),
<<<<<<< HEAD
      writer(maxFrameSize, cimpl),
=======
>>>>>>> 3bbfc42... Imported Upstream version 0.32
      aio(0),
      poller(p)
{
    QPID_LOG(debug, "SslConnector created for " << version.toString());

    if (settings.sslCertName != "") {
        QPID_LOG(debug, "ssl-cert-name = " << settings.sslCertName);
        socket.setCertName(settings.sslCertName);
    }
<<<<<<< HEAD
=======
    if (settings.sslIgnoreHostnameVerificationFailure) {
        socket.ignoreHostnameVerificationFailure();
    }
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

SslConnector::~SslConnector() {
    close();
}

<<<<<<< HEAD
void SslConnector::connect(const std::string& host, const std::string& port){
    Mutex::ScopedLock l(closedLock);
    assert(closed);
    try {
        socket.connect(host, port);
    } catch (const std::exception& e) {
        socket.close();
        throw TransportFailure(e.what());
    }

    identifier = str(format("[%1% %2%]") % socket.getLocalPort() % socket.getPeerAddress());
    closed = false;
    aio = new SslIO(socket,
                       boost::bind(&SslConnector::readbuff, this, _1, _2),
                       boost::bind(&SslConnector::eof, this, _1),
                       boost::bind(&SslConnector::disconnected, this, _1),
                       boost::bind(&SslConnector::socketClosed, this, _1, _2),
                       0, // nobuffs
                       boost::bind(&SslConnector::writebuff, this, _1));
    writer.init(identifier, aio);
}

void SslConnector::init(){
    Mutex::ScopedLock l(closedLock);
    ProtocolInitiation init(version);
    writeDataBlock(init);
    for (int i = 0; i < 32; i++) {
        aio->queueReadBuffer(new Buff(maxFrameSize));
    }
    aio->start(poller);
}

void SslConnector::close() {
    Mutex::ScopedLock l(closedLock);
=======
void SslConnector::connect(const std::string& host, const std::string& port) {
    Mutex::ScopedLock l(lock);
    assert(closed);
    connector = AsynchConnector::create(
        socket,
        host, port,
        boost::bind(&SslConnector::connected, this, _1),
        boost::bind(&SslConnector::connectFailed, this, _3));
    closed = false;

    connector->start(poller);
}

void SslConnector::connected(const Socket&) {
    connector = 0;
    aio = AsynchIO::create(socket,
                           boost::bind(&SslConnector::readbuff, this, _1, _2),
                           boost::bind(&SslConnector::eof, this, _1),
                           boost::bind(&SslConnector::disconnected, this, _1),
                           boost::bind(&SslConnector::socketClosed, this, _1, _2),
                           0, // nobuffs
                           boost::bind(&SslConnector::writebuff, this, _1));

    aio->createBuffers(maxFrameSize);
    identifier = str(format("[%1%]") % socket.getFullAddress());
    ProtocolInitiation init(version);
    writeDataBlock(init);
    aio->start(poller);
}

void SslConnector::connectFailed(const std::string& msg) {
    connector = 0;
    QPID_LOG(warning, "Connect failed: " << msg);
    socket.close();
    if (!closed)
        closed = true;
    if (shutdownHandler)
        shutdownHandler->shutdown();
}

void SslConnector::close() {
    Mutex::ScopedLock l(lock);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    if (!closed) {
        closed = true;
        if (aio)
            aio->queueWriteClose();
    }
}

<<<<<<< HEAD
void SslConnector::socketClosed(SslIO&, const SslSocket&) {
=======
void SslConnector::socketClosed(AsynchIO&, const Socket&) {
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    if (aio)
        aio->queueForDeletion();
    if (shutdownHandler)
        shutdownHandler->shutdown();
}

<<<<<<< HEAD
void SslConnector::setInputHandler(InputHandler* handler){
    input = handler;
}

void SslConnector::setShutdownHandler(ShutdownHandler* handler){
    shutdownHandler = handler;
}

OutputHandler* SslConnector::getOutputHandler() {
    return this;
}

sys::ShutdownHandler* SslConnector::getShutdownHandler() const {
    return shutdownHandler;
=======
void SslConnector::connectAborted() {
    connector->stop();
    connectFailed("Connection timedout");
}

void SslConnector::abort() {
    // Can't abort a closed connection
    if (!closed) {
        if (aio) {
            // Established connection
            aio->requestCallback(boost::bind(&SslConnector::eof, this, _1));
        } else if (connector) {
            // We're still connecting
            connector->requestCallback(boost::bind(&SslConnector::connectAborted, this));
        }
    }
}

void SslConnector::setInputHandler(InputHandler* handler){
    input = handler;
}

void SslConnector::setShutdownHandler(ShutdownHandler* handler){
    shutdownHandler = handler;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

const std::string& SslConnector::getIdentifier() const {
    return identifier;
}

<<<<<<< HEAD
void SslConnector::send(AMQFrame& frame) {
    writer.handle(frame);
}

SslConnector::Writer::Writer(uint16_t s, Bounds* b) : maxFrameSize(s), aio(0), buffer(0), lastEof(0), bounds(b)
{
}

SslConnector::Writer::~Writer() { delete buffer; }

void SslConnector::Writer::init(std::string id, sys::ssl::SslIO* a) {
    Mutex::ScopedLock l(lock);
    identifier = id;
    aio = a;
    newBuffer();
}
void SslConnector::Writer::handle(framing::AMQFrame& frame) {
    Mutex::ScopedLock l(lock);
    frames.push_back(frame);
    if (frame.getEof() || (bounds && bounds->getCurrentSize() >= maxFrameSize)) {
        lastEof = frames.size();
        aio->notifyPendingWrite();
    }
    QPID_LOG(trace, "SENT [" << identifier << "]: " << frame);
}

void SslConnector::Writer::writeOne() {
    assert(buffer);
    framesEncoded = 0;

    buffer->dataStart = 0;
    buffer->dataCount = encode.getPosition();
    aio->queueWrite(buffer);
    newBuffer();
}

void SslConnector::Writer::newBuffer() {
    buffer = aio->getQueuedBuffer();
    if (!buffer) buffer = new Buff(maxFrameSize);
    encode = framing::Buffer(buffer->bytes, buffer->byteCount);
    framesEncoded = 0;
}

// Called in IO thread.
void SslConnector::Writer::write(sys::ssl::SslIO&) {
    Mutex::ScopedLock l(lock);
    assert(buffer);
    size_t bytesWritten(0);
    for (size_t i = 0; i < lastEof; ++i) {
        AMQFrame& frame = frames[i];
        uint32_t size = frame.encodedSize();
        if (size > encode.available()) writeOne();
        assert(size <= encode.available());
        frame.encode(encode);
        ++framesEncoded;
        bytesWritten += size;
    }
    frames.erase(frames.begin(), frames.begin()+lastEof);
    lastEof = 0;
    if (bounds) bounds->reduce(bytesWritten);
    if (encode.getPosition() > 0) writeOne();
}

void SslConnector::readbuff(SslIO& aio, SslIO::BufferBase* buff) {
    framing::Buffer in(buff->bytes+buff->dataStart, buff->dataCount);

    if (!initiated) {
        framing::ProtocolInitiation protocolInit;
        if (protocolInit.decode(in)) {
            //TODO: check the version is correct
            QPID_LOG(debug, "RECV [" << identifier << "]: INIT(" << protocolInit << ")");
        }
        initiated = true;
=======
void SslConnector::handle(AMQFrame& frame) {
    bool notifyWrite = false;
    {
    Mutex::ScopedLock l(lock);
    frames.push_back(frame);
    //only ask to write if this is the end of a frameset or if we
    //already have a buffers worth of data
    currentSize += frame.encodedSize();
    if (frame.getEof()) {
        lastEof = frames.size();
        notifyWrite = true;
    } else {
        notifyWrite = (currentSize >= maxFrameSize);
    }
    /*
      NOTE: Moving the following line into this mutex block
            is a workaround for BZ 570168, in which the test
            testConcurrentSenders causes a hang about 1.5%
            of the time.  ( To see the hang much more frequently
            leave this line out of the mutex block, and put a
            small usleep just before it.)

            TODO mgoulish - fix the underlying cause and then
                            move this call back outside the mutex.
    */
    if (notifyWrite && !closed) aio->notifyPendingWrite();
    }
}

void SslConnector::writebuff(AsynchIO& /*aio*/)
{
    // It's possible to be disconnected and be writable
    if (closed)
        return;

    if (!canEncode()) {
        return;
    }

    AsynchIOBufferBase* buffer = aio->getQueuedBuffer();
    if (buffer) {

        size_t encoded = encode(buffer->bytes, buffer->byteCount);

        buffer->dataStart = 0;
        buffer->dataCount = encoded;
        aio->queueWrite(buffer);
    }
}

// Called in IO thread.
bool SslConnector::canEncode()
{
    Mutex::ScopedLock l(lock);
    //have at least one full frameset or a whole buffers worth of data
    return lastEof || currentSize >= maxFrameSize;
}

// Called in IO thread.
size_t SslConnector::encode(char* buffer, size_t size)
{
    framing::Buffer out(buffer, size);
    size_t bytesWritten(0);
    {
        Mutex::ScopedLock l(lock);
        while (!frames.empty() && out.available() >= frames.front().encodedSize() ) {
            frames.front().encode(out);
            QPID_LOG(trace, "SENT [" << identifier << "]: " << frames.front());
            frames.pop_front();
            if (lastEof) --lastEof;
        }
        bytesWritten = size - out.available();
        currentSize -= bytesWritten;
    }
    if (bounds) bounds->reduce(bytesWritten);
    return bytesWritten;
}

void SslConnector::readbuff(AsynchIO& aio, AsynchIOBufferBase* buff)
{
    int32_t decoded = decode(buff->bytes+buff->dataStart, buff->dataCount);
    // TODO: unreading needs to go away, and when we can cope
    // with multiple sub-buffers in the general buffer scheme, it will
    if (decoded < buff->dataCount) {
        // Adjust buffer for used bytes and then "unread them"
        buff->dataStart += decoded;
        buff->dataCount -= decoded;
        aio.unread(buff);
    } else {
        // Give whole buffer back to aio subsystem
        aio.queueReadBuffer(buff);
    }
}

size_t SslConnector::decode(const char* buffer, size_t size)
{
    framing::Buffer in(const_cast<char*>(buffer), size);
    if (!initiated) {
        framing::ProtocolInitiation protocolInit;
        if (protocolInit.decode(in)) {
            QPID_LOG(debug, "RECV [" << identifier << "]: INIT(" << protocolInit << ")");
            if(!(protocolInit==version)){
                throw Exception(QPID_MSG("Unsupported version: " << protocolInit
                                         << " supported version " << version));
            }
            initiated = true;
        } else {
            return size - in.available();
        }
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    }
    AMQFrame frame;
    while(frame.decode(in)){
        QPID_LOG(trace, "RECV [" << identifier << "]: " << frame);
        input->received(frame);
    }
<<<<<<< HEAD
    // TODO: unreading needs to go away, and when we can cope
    // with multiple sub-buffers in the general buffer scheme, it will
    if (in.available() != 0) {
        // Adjust buffer for used bytes and then "unread them"
        buff->dataStart += buff->dataCount-in.available();
        buff->dataCount = in.available();
        aio.unread(buff);
    } else {
        // Give whole buffer back to aio subsystem
        aio.queueReadBuffer(buff);
    }
}

void SslConnector::writebuff(SslIO& aio_) {
    writer.write(aio_);
}

void SslConnector::writeDataBlock(const AMQDataBlock& data) {
    SslIO::BufferBase* buff = new Buff(maxFrameSize);
=======
    return size - in.available();
}

void SslConnector::writeDataBlock(const AMQDataBlock& data) {
    AsynchIOBufferBase* buff = aio->getQueuedBuffer();
    assert(buff);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    framing::Buffer out(buff->bytes, buff->byteCount);
    data.encode(out);
    buff->dataCount = data.encodedSize();
    aio->queueWrite(buff);
}

<<<<<<< HEAD
void SslConnector::eof(SslIO&) {
    close();
}

void SslConnector::disconnected(SslIO&) {
=======
void SslConnector::eof(AsynchIO&) {
    close();
}

void SslConnector::disconnected(AsynchIO&) {
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    close();
    socketClosed(*aio, socket);
}

const SecuritySettings* SslConnector::getSecuritySettings()
{
    securitySettings.ssf = socket.getKeyLen();
    securitySettings.authid = "dummy";//set to non-empty string to enable external authentication
    return &securitySettings;
}

}} // namespace qpid::client
