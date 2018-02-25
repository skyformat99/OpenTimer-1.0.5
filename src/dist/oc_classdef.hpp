/******************************************************************************
 *                                                                            *
 * Copyright (c) 2016, Tsung-Wei Huang and Martin D. F. Wong,                 *
 * University of Illinois at Urbana-Champaign (UIUC), IL, USA.                *
 *                                                                            *
 * All Rights Reserved.                                                       *
 *                                                                            *
 * This program is free software. You can redistribute and/or modify          *
 * it in accordance with the terms of the accompanying license agreement.     *
 * See LICENSE in the top-level directory for details.                        *
 *                                                                            *
 ******************************************************************************/

#ifndef OC_CLASSDEF_HPP_
#define OC_CLASSDEF_HPP_

namespace OpenCluster {

// Forward declaration.

enum class LoggingSeverity;

class EventState;
class IOState;
class FileLogPolicy;
class LogPolicyInterface;
class StreamBuffer;
class Ticker;

template <typename DerivedT> class Singleton;
template <typename EventT> class BasicEvent;
template <typename EventT, typename LessT> class EventHeap;
template <typename DemuxT> class DemuxInterface;
template <typename EventT> class Select;
template <typename StreamBufferT> class StreamBufferInterface;
template <typename StreamBufferT> class IOStreamBase;
template <typename StreamBufferT> class InputStream;
template <typename StreamBufferT> class OutputStream;
template <typename EventT> class Reactor;
template <typename PolicyT> class Logger;


};  // End of namespace OpenCluster. --------------------------------------------------------------

#endif
