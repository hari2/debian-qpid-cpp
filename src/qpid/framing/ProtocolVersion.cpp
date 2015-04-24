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
#include "qpid/framing/ProtocolVersion.h"
#include <sstream>

using namespace qpid::framing;

const std::string ProtocolVersion::toString() const
{
    std::stringstream ss;
<<<<<<< HEAD
    ss << major_ << "-" << minor_;
=======
    ss << unsigned(major_) << "-" << unsigned(minor_);
    if (major_ == 1) {
        if (protocol_ == SASL) ss << " (SASL)";
        else if (protocol_ == TLS) ss << " (TLS)";
    }
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    return ss.str();
}

ProtocolVersion& ProtocolVersion::operator=(ProtocolVersion p)
{
    major_ = p.major_;
    minor_ = p.minor_;
    return *this;
}

bool ProtocolVersion::operator==(ProtocolVersion p) const
{
    return major_ == p.major_ && minor_ == p.minor_;
}

<<<<<<< HEAD
=======
const uint8_t ProtocolVersion::AMQP(0);
const uint8_t ProtocolVersion::LEGACY_AMQP(1);
const uint8_t ProtocolVersion::TLS(2);
const uint8_t ProtocolVersion::SASL(3);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
