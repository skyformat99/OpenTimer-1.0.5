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

#ifndef OC_UTILITY_HPP_
#define OC_UTILITY_HPP_

#include "oc_headerdef.hpp"

namespace OpenCluster {


// Function: now
// Return the current clock tick (steady_clock).
inline static ::std::chrono::steady_clock::time_point now() {
  return ::std::chrono::steady_clock::now();
}

// Function: make_duration
template <typename T, typename Rep>
static constexpr T make_duration(Rep&& val) {
  return T(::std::forward<Rep>(val));
}

// Function: make_hour
template <typename Rep>
static constexpr ::std::chrono::hours make_hour(Rep&& val) {
  return ::std::chrono::hours(::std::forward<Rep>(val));
}

// Function: make_minute 
template <typename Rep>
static constexpr ::std::chrono::minutes make_minute(Rep&& val) {
  return ::std::chrono::minutes(::std::forward<Rep>(val));
}

// Function: make_seconds 
template <typename Rep>
static constexpr ::std::chrono::seconds make_seconds(Rep&& val) {
  return ::std::chrono::seconds(::std::forward<Rep>(val));
}

// Function: make_milliseconds 
template <typename Rep>
static constexpr ::std::chrono::milliseconds make_milliseconds(Rep&& val) {
  return ::std::chrono::milliseconds(::std::forward<Rep>(val));
}

// Function: make_microseconds 
template <typename Rep>
static constexpr ::std::chrono::microseconds make_microseconds(Rep&& val) {
  return ::std::chrono::microseconds(::std::forward<Rep>(val));
}

// Function: make_nanoseconds 
template <typename Rep>
static constexpr ::std::chrono::nanoseconds make_nanoseconds(Rep&& val) {
  return ::std::chrono::nanoseconds(::std::forward<Rep>(val));
}

// Function: duration_cast
// chrono -> timeval
template <typename T, typename Rep, typename Period>
auto duration_cast (const ::std::chrono::duration<Rep, Period>& d) 
-> ::std::enable_if_t<std::is_same<T, struct timeval>::value, struct timeval> {
  struct timeval tv;
  const ::std::chrono::seconds sec = ::std::chrono::duration_cast<::std::chrono::seconds>(d);
  tv.tv_sec = sec.count();
  tv.tv_usec = ::std::chrono::duration_cast<::std::chrono::microseconds>(d - sec).count();
  return tv;
}

// Function: duration_cast
// timeval -> chrono
template <typename D>
D duration_cast(const struct timeval& tv) {
  return ::std::chrono::duration_cast<D> (
    ::std::chrono::seconds(tv.tv_sec) + ::std::chrono::microseconds(tv.tv_usec)
  );
}

// Function: get_time_info
// Return the time information in proper format.
::std::string get_time_info() {
  time_t rawtime;
  struct tm* timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  char buffer[100];
  strftime(buffer, 100, "%Y-%m-%d %H:%M:%S", timeinfo);
  return buffer;
}

// Function: get_fd_flags
// Get the flags of a file descriptor. File descriptor flags are miscellaneous attributes of 
// a file descriptor. These flags are associated with particular file descriptors, so that if 
// you have created duplicate file descriptors from a single opening of a file, each descriptor 
// has its own set of flags.
auto get_fd_flags(const int fd) {
  auto flags = fcntl(fd, F_GETFD, nullptr);
  if(flags < 0) {
    LOGE("Failed to get fd flags (", strerror(errno), ")");
    return -1;
  }
  return flags;
}

// Function: get_fs_flags
// Get the flags of the file status. Each open file description has certain associated status 
// flags, initialized by open and possibly modified by fcntl. Duplicated file descriptors refer
// to the same open file description, and thus share the same file status flags.
auto get_fs_flags(const int fd) {
  auto flags = fcntl(fd, F_GETFL, nullptr);
  if(flags < 0) {
    LOGE("Failed to get fd status (", strerror(errno), ")");
    return -1;
  }
  return flags;
}

// Procedure: make_socketpair
// Create a un-named socket pair connection.
void make_socketpair(int fd[2]) {
  if(socketpair(AF_UNIX, SOCK_STREAM, 0, fd) == -1) {
    LOGE("Failed to make socketpair (", strerror(errno), ")");
  }
}

// Function: is_valid_fd
// Check whether a file descriptor is valid.
bool is_valid_fd(const int fd) {
  return fcntl(fd, F_GETFL) != -1 || errno != EBADF;
}

// Procedure: make_fd_nonblocking
// Make a file descriptor non-blocking. Read and write access to this file descriptor can be done
// through select or other demultiplexing protocols.
void make_fd_nonblocking(const int fd) {
  auto flags = get_fs_flags(fd);
  if(fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
    LOGE("Failed to make fd non-blocking (", strerror(errno), ")");
  }
}

// Procedure: make_fd_closeonexec
// It sets the close-on-exec flag for the file descriptor, which causes the file descriptor to 
// be automatically (and atomically) closed when any of the exec-family functions succeed.
void make_fd_closeonexec(const int fd) {
  auto flags = get_fd_flags(fd);
  if(fcntl(fd, F_SETFD, flags | FD_CLOEXEC) == -1) {
    LOGE("Failed to make fd closeonexec (", strerror(errno), ")");
  }
}

// Procedure: make_socket_reuseable
// Specifies that the rules used in validating addresses supplied to bind should allow reuse
// of local addresses, if this is supported by the protocol. This option takes an int value.
// This is a boolean option.
void make_socket_reuseable(const int sfd) {
  int one = 1;
  if(setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)) == -1) {
    LOGE("Failed to make socket reuseable (", strerror(errno), ")");
  }
}

// Procedure: make_socket_keepalive
// Keeps connection active by enabling the periodic transmission of messages, if this is
// supported by the protocol. If the connected socket fails to respond to these messages,
// the connection is broken and threads writing to that socket are notified with a SIGPIPE
// signal. This is a boolean option.
void make_socket_keepalive(const int sfd) {
  int one = 1;
  if(setsockopt(sfd, SOL_SOCKET, SO_KEEPALIVE, &one, sizeof(one)) == -1) {
    LOGE("Failed to make socket keepalive (", strerror(errno), ")");
  }
}

// Procedure: make_socket_nosigpipe
// When a connection closes, by default, the process receives a SIGPIPE signal. If the program 
// does not handle or ignore this signal, it will quit immediately. Therefore, the procedure
// tells the socket not to send the signal in the first place if the other side is not connected.
void make_socket_nosigpipe(const int sfd) {
  #ifdef SO_NOSIGPIPE
  int one = 1;
  if(setsockopt(sfd, SOL_SOCKET, SO_NOSIGPIPE, &one, sizeof(one)) == -1) {
    LOGE("Failed to make socket nosigpipe (", strerror(errno), ")");
  }
  #endif
}

// Procedure: set_default_socket_server_attribute
// Set the socket attribute to the default values defined by evio library.
void set_default_socket_server_attribute(const int sfd) {
  make_fd_nonblocking(sfd);
  make_fd_closeonexec(sfd);
  make_socket_reuseable(sfd);
  make_socket_keepalive(sfd);
}

// Procedure: set_default_socket_client_attribute
// Set the socket attribute to the default values defined by evio library.
void set_default_socket_client_attribute(const int sfd) {
  make_fd_nonblocking(sfd);
  make_fd_closeonexec(sfd);
  make_socket_reuseable(sfd);
  make_socket_keepalive(sfd);
}

// Function: accept_socket_client
// Accept a client connection from a given listener. The newly accepted client will be marked
// as non-blocking.
int accept_socket_client(const int listener) {

  struct sockaddr client_addr;
  socklen_t client_addrlen = sizeof(client_addr);
  auto newfd = accept(listener, &client_addr, &client_addrlen);

  if(newfd == -1) {
    LOGE("Failed to accept a client (", strerror(errno), ")");
  } 
  else {
    set_default_socket_client_attribute(newfd);
  }

  return newfd;
}

// Function: is_sockpeer_connected
// This function is typically called after select() has indicated that the given fd is 
// ready-for-write because fd's non-blocking TCP connection has either succeeded or failed.
// Returns true if the connection succeeded, false if the connection failed.
// If this returns true, fd can be continued to use as a normal connected non-blocking TCP socket.
// If this returns false, fd should be closed because the connection failed.
// Notice that the input socket should disable the termination of SIGPIPE.
bool is_sockpeer_connected(const int fd) {

  // Special case for FreeBSD7, for which send() does not generate SIGPIPE.
  //#if defined(__FreeBSD__) || defined(BSD)

  struct sockaddr junk;
  socklen_t length = sizeof(junk);
  memset(&junk, 0, sizeof(junk));
  return getpeername(fd, &junk, &length) == 0;

  // The following code is all we need for most platform.
  //#else

  //char junk;
  //return send(fd, &junk, 0, 0) == 0;

  //#endif
}

// TODO: Chun-Xun
// Function: make_shm_server
int make_shm_server(const ::std::string& fpath) {
  
  throw ::std::runtime_error("Function unsupported\n");

  // set_default_shm_attribute
  //  int fd = shm_open( fpath.c_str() , O_CREAT | O_RDWR | O_NONBLOCK, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH );  
  //  if( fd == -1 ){
  //    perror("Shared Memory Server cannot create\n");
  //    return -1;
  //  }
  //  // ptr =  mmap(nullptr, 10, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  //  return fd;
  return -1;
}

// TODO: Chun-Xun
// Function: make_shm_client
int make_shm_client(const ::std::string& fpath) {
  
  throw ::std::runtime_error("Function unsupported\n");
  // set_default_shm_attribute
  //  int fd = shm_open( fpath.c_str() , O_CREAT | O_RDWR | O_NONBLOCK, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH );  
  //  if( fd == -1 ){
  //    perror("Shared Memory Client cannot create\n");
  //    return -1;
  //  }
  //  // ptr =  mmap(nullptr, 10, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  //  return fd;
  return -1;
}

// Function: make_fifo_writer
//
// A FIFO special file (a named pipe) is similar to a pipe, except that it is accessed as part of 
// the filesystem. It can be opened by multiple processes for reading or writing. When processes 
// are exchanging data via the FIFO, the kernel passes all data internally without writing it to 
// the filesystem. Thus, the FIFO special file has no contents on the filesystem; the filesystem 
// entry merely serves as a reference point so that processes can access the pipe using a name in 
// the filesystem.
//
// The kernel maintains exactly one pipe object for each FIFO special file that is opened by at 
// least one process. The FIFO must be opened on both ends (reading and writing) before data can 
// be passed. Normally, opening the FIFO blocks until the other end is opened also.
//
// A process can open a FIFO in non-blocking mode. In this case, opening for read only will succeed
// even if noone has opened on the write side yet; opening for write only will fail with ENXIO 
// (no such device or address) unless the other end has already been opened.
//
// Under Linux, opening a FIFO for read and write will succeed both in blocking and non-blocking 
// mode. POSIX leaves this behaviour undefined. This can be used to open a FIFO for writing while 
// there are no readers available. A process that uses both ends of the connection in order to 
// communicate with itself should be very careful to avoid deadlocks.  
//
int make_fifo_writer(const ::std::string& fpath) {

  // Create a fifo at fpath & the file shouldn't exist before creating (EEXIST)
  // By default, the permission is set to 664 (with umask 002). That is, owner and group has w/r
  // and others can read only.
  if((mkfifo(fpath.c_str(), 0666) < 0) && errno != EEXIST) {   
    LOGE("Failed to make an FIFO writer file (", strerror(errno), ")");
    return -1;
  }   

  // Set to Write/Read & Non-Blocking. Notice here reading data from this FIFO is undefined.
  int fd = open(fpath.c_str(), O_WRONLY | O_NONBLOCK | O_CLOEXEC); 

  if(fd == -1) {   
    LOGE("Failed to open an FIFO writer file (", strerror(errno), ")");
  }   

  return fd;
}

// Function: make_fifo_reader
//
// A FIFO special file (a named pipe) is similar to a pipe, except that it is accessed as part of 
// the filesystem. It can be opened by multiple processes for reading or writing. When processes 
// are exchanging data via the FIFO, the kernel passes all data internally without writing it to 
// the filesystem. Thus, the FIFO special file has no contents on the filesystem; the filesystem 
// entry merely serves as a reference point so that processes can access the pipe using a name in 
// the filesystem.
//
// The kernel maintains exactly one pipe object for each FIFO special file that is opened by at 
// least one process. The FIFO must be opened on both ends (reading and writing) before data can 
// be passed. Normally, opening the FIFO blocks until the other end is opened also.
//
// A process can open a FIFO in non-blocking mode. In this case, opening for read only will succeed
// even if noone has opened on the write side yet; opening for write only will fail with ENXIO 
// (no such device or address) unless the other end has already been opened.
//
// Under Linux, opening a FIFO for read and write will succeed both in blocking and non-blocking 
// mode. POSIX leaves this behaviour undefined. This can be used to open a FIFO for writing while 
// there are no readers available. A process that uses both ends of the connection in order to 
// communicate with itself should be very careful to avoid deadlocks.  
//
int make_fifo_reader(const ::std::string& fpath) {

  // Create a fifo at fpath & the file shouldn't exist before creating (EEXIST)
  if((mkfifo(fpath.c_str(), 0666) < 0) && errno != EEXIST) {   
    LOGE("Failed to make an FIFO reader file (", strerror(errno), ")");
    return -1;
  }   

  // Set to Read Only & Non-Blocking
  int fd = open(fpath.c_str(), O_RDONLY | O_NONBLOCK | O_CLOEXEC); 
  if(fd == -1){   
    LOGE("Failed to open an FIFO reader file (", strerror(errno), ")");
  }   

  return fd;
}

// Function: make_socket_client
// Create a TCP client socket and make it non-blocking. We following the following steps to create
// an asynchronous connect:
// 
// 1. create socket with socket(..., SOCK_NONBLOCK, ...)
// 2. start connection with connect(fd, ...)
// 3. if return value is neither 0 nor EINPROGRESS, then abort with error
// 4. wait until fd is signalled as ready for output
// 5. check status of socket with getsockopt(fd, SOL_SOCKET, SO_ERROR, ...)
// done
//
int make_socket_client(const ::std::string& hostname, const ::std::string& port) {
  
  // Memo:
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
  // 
  // struct sockaddr {
  //   unsigned short si_family;       // address family, AF_XXX
  //   char           sa_data[14];     // 14 bytes of protocol address.
  // }

  struct addrinfo hints;
  struct addrinfo* res {nullptr};
  struct addrinfo* ptr {nullptr};

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  
  int rv;
  int connector;

  if((rv = getaddrinfo(hostname.c_str(), port.c_str(), &hints, &res)) != 0) {
    LOGE(gai_strerror(rv));
    return -1;
  }
  
  // Try to get the first available client connection.
  // TODO (the first might not be the one matching the server)
  for(ptr = res; ptr != nullptr; ptr = ptr->ai_next) {
    
    // Ignore undefined ip type.
    if(ptr->ai_family != AF_INET && ptr->ai_family != AF_INET6) {
      continue;
    }

    // Create a listener socket and bind it to the localhost as the server.
    if((connector = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol)) == -1){
      continue;
    }

    set_default_socket_client_attribute(connector);
    
    // The socket is nonblocking and the connection cannot be completed immediately.  It is 
    // possible to select(2) or poll(2) for completion by selecting the socket for writing.  
    // After select(2) indicates writability, use getsockopt(2) to read the SO_ERROR option at 
    // level SOL_SOCKET to determine whether connect() completed successfully (SO_ERROR is zero) or
    // unsuccessfully (SO_ERROR is one of the usual error codes listed here, explaining the 
    // reason for the failure).
    if(::connect(connector, (struct sockaddr*)ptr->ai_addr, ptr->ai_addrlen) < 0) {
      // This is what we expect.
      if(errno == EINPROGRESS) {
        break;
      }
      // This may happen right here, on localhost for example (immediate connection refused).
      // It happens on FreeBSD but not on Solaris.
      else {
        close(connector);
        continue;
      }
    }
    else {
      // This may happen on localhost.
      break;
    }
  }
  
  freeaddrinfo(res);

  if(ptr == nullptr) {
    LOGE("Failed to make a socket client");
    return -1;
  }
  
  return connector;  
}

// Function: make_socket_server
// Create a TCP socket, bind it to the localhost, and listen to a given port. By default the socket
// is reuseable on close and non-blocking.
int make_socket_server(const ::std::string& port) {
  
  // Memo:
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
  // 
  // struct sockaddr {
  //   unsigned short si_family;       // address family, AF_XXX
  //   char           sa_data[14];     // 14 bytes of protocol address.
  // }

  struct addrinfo hints;
  struct addrinfo* res {nullptr};
  struct addrinfo* ptr {nullptr};

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE;        // let it fill the host for us.

  int rv;
  int listener{-1};

  if((rv = getaddrinfo(nullptr, port.c_str(), &hints, &res)) != 0) {
    LOGE(gai_strerror(rv));
    return -1;
  }

  // Try to connect to the first one that is available.
  // TODO: are we really using the first address?
  for(ptr = res; ptr != nullptr; ptr = ptr->ai_next) {
    
    // Undefined ip type.
    if(ptr->ai_family != AF_INET && ptr->ai_family != AF_INET6) {
      continue;
    }
    
    // Create a listener socket and bind it to the localhost as the server.
    if((listener = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol)) == -1){
      continue;
    }

    set_default_socket_server_attribute(listener);

    if(bind(listener, ptr->ai_addr, ptr->ai_addrlen) < 0) {
      close(listener);
      continue;
    }
    
    break;
  }
  
  freeaddrinfo(res);

  if(ptr == nullptr) {
    LOGE("Failed to make a socket server");
    return -1;
  }

  if(listen(listener, 128) == -1) {
    LOGE("Failed to make a socket listener");
    return -1;
  }

  return listener;
}


};  // End of namespace OpenCluster. ---------------------------------------------------------------------


#endif



