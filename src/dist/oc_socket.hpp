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

#ifndef OC_SOCKET_HPP_
#define OC_SOCKET_HPP_

#include "oc_headerdef.hpp"

namespace OpenCluster {

//// Sockaddr
//struct sockaddr {
//    unsigned short    sa_family;    // address family, AF_xxx
//    char              sa_data[14];  // 14 bytes of protocol address
//};
//
//
//// IPv4 AF_INET sockets:
//struct sockaddr_in {
//    short            sin_family;   // e.g. AF_INET, AF_INET6
//    unsigned short   sin_port;     // e.g. htons(3490)
//    struct in_addr   sin_addr;     // see struct in_addr, below
//    char             sin_zero[8];  // zero this if you want to
//};
//
//struct in_addr {
//    unsigned long s_addr;          // load with inet_pton()
//};
//
//
//// IPv6 AF_INET6 sockets:
//
//struct sockaddr_in6 {
//    u_int16_t       sin6_family;   // address family, AF_INET6
//    u_int16_t       sin6_port;     // port number, Network Byte Order
//    u_int32_t       sin6_flowinfo; // IPv6 flow information
//    struct in6_addr sin6_addr;     // IPv6 address
//    u_int32_t       sin6_scope_id; // Scope ID
//};
//
//struct in6_addr {
//    unsigned char   s6_addr[16];   // load with inet_pton()
//};
//
//
//// General socket address holding structure, big enough to hold either
//// struct sockaddr_in or struct sockaddr_in6 data:
//
//struct sockaddr_storage {
//    sa_family_t  ss_family;     // address family
//
//    // all this is padding, implementation specific, ignore it:
//    char      __ss_pad1[_SS_PAD1SIZE];
//    int64_t   __ss_align;
//    char      __ss_pad2[_SS_PAD2SIZE];
//};
//
// struct addrinfo {
//   int             ai_flags;       // AI_PASSIVE, AI_CANONNAME, etc.
//   int             ai_family;      // AF_INET, AF_INET6, AF_UNSPEC
//   int             ai_socktype;    // SOCK_STREAM, SOCK_DGRAM
//   int             ai_protocol;    // use 0 for "any"
//   size_t          ai_addrlen;     // size of ai_addr in bytes
//   struct sockaddr *ai_addr;       // struct sockaddr_in or _in6
//   char            *ai_canonname;  // full canonical hostname
//   struct addrinfo *ai_next;       // linked list, next node
// }


// Class: BasicSocketAddress
class BasicSocketAddress {

};

//-------------------------------------------------------------------------------------------------

// Class: BasicSocket
class BasicSocket : public BasicSocketAddress {

  private: 

    int _fd {-1};

  public:

    BasicSocket();
    ~BasicSocket();

    OC_CODEGEN_ACCESSOR(fd);

    OC_CODEGEN_MUTATOR(fd);

};

//-------------------------------------------------------------------------------------------------

// Class: BasicSocketServer
class BasicSocketServer : public BasicSocket {
  
  private:

  public:

    BasicSocketServer();
    ~BasicSocketServer();

    void open(const ::std::string&);
    void close();

};

//-------------------------------------------------------------------------------------------------


// Class: BasicSocketClient
class BasicSocketclient : public BasicSocket {

  private:

  public:
    
    void open(const ::std::string&, const ::std::string&);
    void close();

};


};  // End of namespace OpenCluster. --------------------------------------------------------------



#endif



