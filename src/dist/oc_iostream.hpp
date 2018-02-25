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

#ifndef OC_IOSTREAM_HPP_
#define OC_IOSTREAM_HPP_

#include "oc_headerdef.hpp"

namespace OpenCluster {

// TODO:
// (1) Write a read seek function that can wrap the _r_ptr and _in_avail together.
// (2) Write a write seek function that can wrap the _w_ptr and _in_avail together.


// Class: StreamBufferInterface
// The interface class for the streambuffer.
template <typename StreamBufferT>
class StreamBufferInterface {

  public:

    // Interface API.
    inline StreamBufferT* streambuffer();
    inline const StreamBufferT* streambuffer() const;
        
    inline void write(const void*, const size_t);
    inline void read(void*, const size_t);
    inline void copy(void*, const size_t);

    inline int sbumpc();
    inline int sgetc();

    inline ssize_t flush(const int);
    inline ssize_t sync(const int);

    inline size_t in_avail() const;

    // Default implementation.
    inline void write_impl(const void*, const size_t);
    inline void read_impl(void*, const size_t);
    inline void copy_impl(void*, const size_t);
    inline ssize_t flush_impl(const int);
    inline ssize_t sync_impl(const int);
    inline size_t in_avail_impl() const;
    inline int sbumpc_impl();
    inline int sgetc_impl();
};

// Function: StreamBuffer
template <typename StreamBufferT>
inline StreamBufferT* StreamBufferInterface<StreamBufferT>::streambuffer() {
  return static_cast <StreamBufferT*> (this);
}

// Function: StreamBuffer
template <typename StreamBufferT>
inline const StreamBufferT* StreamBufferInterface<StreamBufferT>::streambuffer() const {
  return static_cast <const StreamBufferT*> (this);
}

// Procedure: write
template <typename StreamBufferT>
inline void StreamBufferInterface<StreamBufferT>::write(const void* data, const size_t size) {
  streambuffer()->write_impl(data, size);
}

// Procedure: write_impl
template <typename StreamBufferT>
inline void StreamBufferInterface<StreamBufferT>::write_impl(const void* data, const size_t size) {
  LOGW("write method is not implemented in StreamBuffer class");
}

// Procedure: read
template <typename StreamBufferT>
inline void StreamBufferInterface<StreamBufferT>::read(void* data, const size_t size) {
  streambuffer()->read_impl(data, size);
}

// Procedure: read_impl
template <typename StreamBufferT>
inline void StreamBufferInterface<StreamBufferT>::read_impl(void* data, const size_t size) {
  LOGW("read method is not implemented in StreamBuffer class");
}

// Procedure: copy
template <typename StreamBufferT>
inline void StreamBufferInterface<StreamBufferT>::copy(void* data, const size_t size) {
  streambuffer()->copy_impl(data, size);
}

// Procedure: copy_impl
template <typename StreamBufferT>
inline void StreamBufferInterface<StreamBufferT>::copy_impl(void* data, const size_t size) {
  LOGW("copy method is not implemented in StreamBuffer class");
}

// Function: in_avail
template <typename StreamBufferT>
inline size_t StreamBufferInterface<StreamBufferT>::in_avail() const {
  return streambuffer()->in_avail_impl();
}

// Function: in_avail_impl
template <typename StreamBufferT>
inline size_t StreamBufferInterface<StreamBufferT>::in_avail_impl() const {
  LOGW("in_avail method is not implemented in StreamBuffer class");
  return 0;
}

// Function: flush
template <typename StreamBufferT>
inline ssize_t StreamBufferInterface<StreamBufferT>::flush(const int fd) {
  return streambuffer()->flush_impl(fd);
}

// Function: flush_impl
template <typename StreamBufferT>
inline ssize_t StreamBufferInterface<StreamBufferT>::flush_impl(const int fd) {
  LOGW("flush method is not implemented in StreamBuffer class");
  return -1;
}

// Function: sync
template <typename StreamBufferT>
inline ssize_t StreamBufferInterface<StreamBufferT>::sync(const int fd) {
  return streambuffer()->sync_impl(fd);
}

// Function: sync_impl
template <typename StreamBufferT>
inline ssize_t StreamBufferInterface<StreamBufferT>::sync_impl(const int fd) {
  LOGW("sync method is not implemented in StreamBuffer class");
  return -1;
}

// Function: sbumpc
template <typename StreamBufferT>
inline int StreamBufferInterface<StreamBufferT>::sbumpc() {
  return streambuffer()->sbumpc_impl();
}

// Function: sbumpc_impl
template <typename StreamBufferT>
inline int StreamBufferInterface<StreamBufferT>::sbumpc_impl() {
  LOGW("sbumpc method is not implemented in StreamBuffer class");
  return EOF;
}

// Function: sgetc
template <typename StreamBufferT>
inline int StreamBufferInterface<StreamBufferT>::sgetc() {
  return streambuffer()->sgetc_impl();
}

// Function: sgetc_impl
template <typename StreamBufferT>
inline int StreamBufferInterface<StreamBufferT>::sgetc_impl() {
  LOGW("sgetc method is not implemented in StreamBuffer class");
  return EOF;
}

//-------------------------------------------------------------------------------------------------

// Class: StreamBuffer
// The linked-list structured stream buffer.
class StreamBuffer : public StreamBufferInterface<StreamBuffer> {

  // Struct: DataBlock
  // The basic buffer block for stream buffer.
  struct DataBlock {
  
    static constexpr size_t size {1024};
  
    static_assert(
      size >= sizeof(size_t), 
      "Failed to compile DataBlock (size should be at least size_t)"
    );
    
    // Members.
    int idx {-1};
    unsigned char buffer[size];
    DataBlock* next {nullptr};
  
    // Positioning.
    inline size_t in_avail(const unsigned char* ptr) const { return size - ptrdiff_t(ptr - buffer); }
    inline bool exhausted(const unsigned char* ptr) const { return ptr == (buffer + size); }
  };

  private:

    ArrayList <DataBlock, SingularAllocator<DataBlock>> _blocklist;
    
    DataBlock* _head {nullptr};
    DataBlock* _tail {nullptr};

    size_t _in_avail {0};

    unsigned char* _w_ptr {nullptr};
    unsigned char* _r_ptr {nullptr};

    struct iovec _wsegs[MAX_NUM_WRITE_IOVECS];

  public:

    StreamBuffer();
    ~StreamBuffer();
    
    void write_impl(const void*, const size_t);
    void read_impl(void*, const size_t);
    void copy_impl(void*, const size_t);

    ssize_t flush_impl(const int);
    ssize_t sync_impl(const int);

    int sgetc_impl();
    int sbumpc_impl();

    inline size_t in_avail_impl() const;
    inline size_t num_blocks() const;

  private:
    
    template <typename... ArgsT>
    DataBlock* _append_block(ArgsT&&...);

    void _throw_block();
};

// Constructor
StreamBuffer::StreamBuffer() {

}

// Destructor
StreamBuffer::~StreamBuffer() {
  //free(_iosegs);
}

// Function: in_avail_impl
inline size_t StreamBuffer::in_avail_impl() const {
  return _in_avail;
}

// Function: sgetc_impl
int StreamBuffer::sgetc_impl() {
  return in_avail() ? *_r_ptr : EOF;
}

// Function: sbumpc_impl
int StreamBuffer::sbumpc_impl() {
  if(!in_avail()) return EOF;
  auto ret = *_r_ptr;
  ++_r_ptr;
  --_in_avail;
  if(_head->exhausted(_r_ptr)) _throw_block();
  return ret;
}

// Function: num_blocks
inline size_t StreamBuffer::num_blocks() const {
  return _blocklist.size();
}

// Procedure: copy_impl
void StreamBuffer::copy_impl(void* in_data, const size_t size) {

  unsigned char* data = static_cast<unsigned char*>(in_data);
  size_t remain = ::std::min(in_avail(), size);

  auto cur_r_ptr = _r_ptr;
  auto cur_head = _head;

  while(remain) {
    
    size_t num = (cur_head == _tail) ? ::std::min(remain, size_t(ptrdiff_t(_w_ptr - cur_r_ptr))) :
                                       ::std::min(remain, cur_head->in_avail(cur_r_ptr)) ;
    
    memcpy(data, cur_r_ptr, num);
    
    cur_r_ptr += num;
    data      += num;
    remain    -= num;

    if(cur_head->exhausted(cur_r_ptr)) {
      cur_head = cur_head->next;
      if(cur_head == nullptr) {
        cur_r_ptr = nullptr;
      }
      else {
        cur_r_ptr = cur_head->buffer;
      }
    }
  }
}

// Procedure: read_impl
void StreamBuffer::read_impl(void* in_data, const size_t size) {

  unsigned char* data = static_cast<unsigned char*>(in_data);
  size_t remain = ::std::min(in_avail(), size);

  while(remain) {

    size_t num = (_head == _tail) ? ::std::min(remain, size_t(ptrdiff_t(_w_ptr - _r_ptr))) :
                                    ::std::min(remain, _head->in_avail(_r_ptr)) ;
    memcpy(data, _r_ptr, num);

    _r_ptr    += num;
    data      += num;
    remain    -= num;
    _in_avail -= num;
    
    if(_head->exhausted(_r_ptr)) _throw_block();
  }
}

// Function: flush_impl
// The function flushes as much as possible bytes from the in_avail region to the given device
// specified by a file descriptor. 
ssize_t StreamBuffer::flush_impl(const int fd) {

	if(in_avail() == 0) return 0;

	size_t origin_total_bytes = in_avail();
	size_t total_byte_send {0};
	size_t num_blks {0};
	size_t num_written_blks {0};

	while( origin_total_bytes > total_byte_send ){
		// Refill _wsegs ONLY WHEN _wsegs is empty
    // TODO: 
		if(num_written_blks == num_blks && _head != nullptr){
			num_written_blks = 0;
			num_blks = 0;
			for(auto ptr=_head; ptr && num_blks < OpenCluster::MAX_NUM_WRITE_IOVECS; ptr=ptr->next, ++num_blks){
				// Case 1: Only one block
				if( ptr == _head && ptr == _tail ){
					_wsegs[num_blks].iov_base = _r_ptr;
					_wsegs[num_blks].iov_len = ptrdiff_t( _w_ptr - _r_ptr);
				}
				// Case 2: Head block
				else if(ptr == _head){
					_wsegs[num_blks].iov_base = _r_ptr;
					_wsegs[num_blks].iov_len = ptr->in_avail(_r_ptr);
				}
				// Case 3: Tail block.
				else if(ptr == _tail ){
					_wsegs[num_blks].iov_base = ptr->buffer;
					_wsegs[num_blks].iov_len = ptrdiff_t(_w_ptr - ptr->buffer);
				}
				// Case 4: Middle block.
				else{
					_wsegs[num_blks].iov_base = ptr->buffer;
					_wsegs[num_blks].iov_len = ptr->size;
				}
			}
		}


		// Write data to fd
    issue_writev:
		auto ret = ::writev(fd, &(_wsegs[ num_written_blks ]) ,  num_blks - num_written_blks );

		if(ret < 0){
			if(errno == EINTR) {
				goto issue_writev;
			}
			else if(errno == EAGAIN) {
				break; 
			}
			else {
				// TODO:
        LOGE("Failed to flush streambuffer (", strerror(errno), ")");
				return -1;
			}
		}
		else if(ret == 0) {
			break;
		}
		else{
			// ret is positive, some data has been written and needs to adjust num_blks
			total_byte_send += ret;
			_in_avail -= ret;
			while(ret > 0){
				if( ret >= static_cast<ssize_t>(_wsegs[ num_written_blks ].iov_len) ){
					ret -= static_cast<ssize_t>(_wsegs[ num_written_blks ].iov_len);
					_throw_block();
					++ num_written_blks;
				}
				else{
					 _wsegs[num_written_blks].iov_base = static_cast<unsigned char*>(_wsegs[num_written_blks].iov_base) + ret;
					 _r_ptr = static_cast<unsigned char*>(_wsegs[num_written_blks].iov_base);
					_wsegs[ num_written_blks ].iov_len -= ret;
					ret = 0;
				}
			}
			
		}
	}
	return total_byte_send;
}

// Procedure: write_impl
void StreamBuffer::write_impl(const void* in_data, const size_t size) {

  const unsigned char* data = static_cast<const unsigned char*>(in_data);
  size_t remain = size;

  while(remain) {
    
    auto block = _tail;

    if(block == nullptr || block->in_avail(_w_ptr) == 0) {
      block = _append_block();
    }
    size_t num = ::std::min(block->in_avail(_w_ptr), remain);

    memcpy(_w_ptr, data, num);
    data      += num;
    _w_ptr    += num;
    _in_avail += num;
    remain    -= num;
  }
}

// Procedure: sync_impl
// Synchronize the stream buffer with a given file descriptor. Notice that the given file 
// descriptor should be non-blocking.
ssize_t StreamBuffer::sync_impl(const int fd) {

  size_t total {0};

  while(1) {

    auto block = _tail;

    if(block == nullptr || block->in_avail(_w_ptr) == 0) {
      block = _append_block();
    }

    issue_read:
    auto ret = ::read(fd, _w_ptr, block->in_avail(_w_ptr));

    if(ret == -1) {
      if(errno == EINTR) {
        goto issue_read;
      }
      else if(errno == EAGAIN) {
        break; 
      }
      else {
        // TODO:
        LOGE("Failed to sync streambuf (", strerror(errno), ")");
        return -1;
      }
    }
    else if(ret == 0) {
      break;
    }

    total     += ret;
    _w_ptr    += ret;
    _in_avail += ret;
  };

  return total;
}

// Procedure: _throw_block
void StreamBuffer::_throw_block() {

  if(_head == nullptr) return;

  size_t idx = _head->idx;

  _head = _head->next;

  if(_head == nullptr) {
    _tail = nullptr;
    _w_ptr = nullptr;
    _r_ptr = nullptr;
  }
  else {
    _r_ptr = _head->buffer;
  }

  _blocklist.remove(idx);
}

// Function: _append_block
template <typename... ArgsT>
typename StreamBuffer::DataBlock* StreamBuffer::_append_block(ArgsT&&... args) {

  auto idx = _blocklist.insert(::std::forward<ArgsT>(args)...);
  auto blk = _blocklist[idx];
  blk->idx = idx;

  if(_head == nullptr) {
    _head = blk;
    _tail = blk;
    _r_ptr = blk->buffer;
  }
  else {
    _tail->next = blk;
    _tail = blk;
  }
  _w_ptr = blk->buffer;

  return blk;
}

//-------------------------------------------------------------------------------------------------

// Class: IOState
// A class that emulates the std::iostate with the following member functions.
//
// good        Check whether state of stream is good  (public member function )
// eof         Check whether eofbit is set            (public member function )
// fail        Check whether failbit or badbit is set (public member function )
// bad         Check whether badbit is set            (public member function )
// rdstate     Get error state flags                  (public member function )
// set_state   Set error state flag                   (public member function )
// clear_state Set error state flags                  (public member function )
//
class IOState {

  private:

    ::std::ios_base::iostate _iostate {::std::ios_base::goodbit};

  public:

    inline bool good() const;
    inline bool fail() const;
    inline bool eof() const;
    inline bool bad() const;
    inline bool has_iostate(const ::std::ios_base::iostate&) const;

    inline ::std::ios_base::iostate iostate() const;
    inline void set_iostate(const ::std::ios_base::iostate&);
    inline void clear_iostate();

    inline void insert_iostates(const ::std::ios_base::iostate&);
    inline void remove_iostates(const ::std::ios_base::iostate&);

    template <typename... ArgsT>
    inline void insert_iostates(const ::std::ios_base::iostate&, ArgsT&&...);

    template <typename... ArgsT>
    inline void remove_iostates(const ::std::ios_base::iostate&, ArgsT&&...);
};

// Fuction: good
inline bool IOState::good() const {
  return _iostate == ::std::ios_base::goodbit;
}

// Fuction: fail
inline bool IOState::fail() const {
  return (_iostate & (::std::ios_base::failbit | ::std::ios_base::badbit));
}

// Fuction: eof
inline bool IOState::eof() const {
  return (_iostate & ::std::ios_base::eofbit);
}

// Fuction: bad
inline bool IOState::bad() const {
  return (_iostate & ::std::ios_base::badbit);
}

// Function: has_iostate
inline bool IOState::has_iostate(const ::std::ios_base::iostate& s) const {
  return _iostate & s;
}

// Function: rdstate
inline ::std::ios_base::iostate IOState::iostate() const {
  return _iostate;
}

// Function: clear_iostate
inline void IOState::clear_iostate() {
  _iostate = ::std::ios_base::goodbit;
}

// Function: set_iostate
inline void IOState::set_iostate(const ::std::ios_base::iostate& new_state){
  _iostate = new_state;
}

// Procedure: insert_iostates
inline void IOState::insert_iostates(const ::std::ios_base::iostate& s) {
  _iostate |= s;
}

// Procedure: insert_iostates
template <typename... ArgsT>
inline void IOState::insert_iostates(const ::std::ios_base::iostate& s, ArgsT&&... args) {
  _iostate |= s;
  insert_iostates(::std::forward<ArgsT>(args)...);
}

// Procedure: remove_iostates
inline void IOState::remove_iostates(const ::std::ios_base::iostate& s) {
  _iostate &= ~s;
}

// Procedure: remove_iostates
template <typename... ArgsT>
inline void IOState::remove_iostates(const ::std::ios_base::iostate& s, ArgsT&&... args) {
  _iostate &= ~s;
  remove_iostates(::std::forward<ArgsT>(args)...);
}

//-------------------------------------------------------------------------------------------------

// Class: IOStreamBase
template <typename StreamBufferT>
class IOStreamBase : public IOState {

  private:

    ::std::shared_ptr<StreamBufferT> _rdbuf_sptr;
  
  public:  
    
    IOStreamBase (const ::std::shared_ptr<StreamBufferT>&);
    
    inline StreamBufferT* rdbuf();
    inline const StreamBufferT* rdbuf() const;
};

// Constructor.
template <typename StreamBufferT>
IOStreamBase<StreamBufferT>::IOStreamBase(const ::std::shared_ptr<StreamBufferT>& tgt) {
  _rdbuf_sptr = (tgt == nullptr) ? ::std::make_shared<StreamBufferT>() : tgt;
}

// Function: rdbuf
template <typename StreamBufferT>
StreamBufferT* IOStreamBase<StreamBufferT>::rdbuf() {
  return _rdbuf_sptr.get();
}

// Function: rdbuf
template <typename StreamBufferT>
const StreamBufferT* IOStreamBase<StreamBufferT>::rdbuf() const {
  return _rdbuf_sptr.get();
}

//-------------------------------------------------------------------------------------------------

// Class: OutputStream
//
// By default, the insertion operation is provided for arithmetic types:
// - integral types bool
//   char, char16_t, char32_t, wchar_t, signed char,
//   short int, int, long int, long long int
//   unsigned char, unsigned short int, unsigned int, unsigned long int, unsigned long long int
// - floating point types  
//   float, double, long double
//
template <typename StreamBufferT>
class OutputStream : public IOStreamBase<StreamBufferT> {
  
  public:
    
    OutputStream(const ::std::shared_ptr<StreamBufferT>& = nullptr);

    template <typename T, typename = ::std::enable_if_t<::std::is_arithmetic<T>::value>>
    inline OutputStream& operator << (const T&);

    inline OutputStream& operator << (const ::std::string&);
    inline OutputStream& operator << (const char*);
    inline OutputStream& operator << (const signed char*);
    inline OutputStream& operator << (const unsigned char*);

    inline ssize_t flush(const int);
};

// Constructor.
template <typename StreamBufferT>
OutputStream<StreamBufferT>::OutputStream(const ::std::shared_ptr<StreamBufferT>& tgt) :
  IOStreamBase<StreamBufferT>::IOStreamBase(tgt) {
}

// Operator: <<
// Arithmetic data types.
template <typename StreamBufferT>
template <typename T, typename>
inline OutputStream<StreamBufferT>& OutputStream<StreamBufferT>::operator << (const T& rhs) {
  this->clear_iostate();
  this->rdbuf()->write(&rhs, sizeof(T));
  return *this;
} 

// Operator: <<
// String insertion.
template <typename StreamBufferT>
inline OutputStream<StreamBufferT>& OutputStream<StreamBufferT>::operator << (const ::std::string& rhs) {
  return *this << rhs.c_str();
}

// Operator: <<
// String insertion.
template <typename StreamBufferT>
inline OutputStream<StreamBufferT>& OutputStream<StreamBufferT>::operator << (const char* rhs) {
  this->clear_iostate();
  if(rhs == nullptr) {
    this->insert_iostates(::std::ios_base::badbit);
  }
  else {
    this->rdbuf()->write(rhs, ::std::char_traits<char>::length(rhs) + 1);
  }
  return *this;
}

// Operator: <<
// String insertion.
template <typename StreamBufferT>
inline OutputStream<StreamBufferT>& OutputStream<StreamBufferT>::operator << (const signed char* rhs) {
  return *this << reinterpret_cast<const char*>(rhs);
}

// Operator: <<
// String insertion.
template <typename StreamBufferT>
inline OutputStream<StreamBufferT>& OutputStream<StreamBufferT>::operator << (const unsigned char* rhs) {
  return *this << reinterpret_cast<const char*>(rhs);
}

// Procedure: flush
template <typename StreamBufferT>
inline ssize_t OutputStream<StreamBufferT>::flush(const int fd) {

  this->clear_iostate();

  auto ret = this->rdbuf()->flush(fd);

  if(ret < 0) {
    this->insert_iostates(::std::ios_base::failbit);
  }
  else if(ret == 0) {
    this->insert_iostates(::std::ios_base::badbit);
  }

  return ret;
}

//-------------------------------------------------------------------------------------------------

// Class: InputStream
//
// By default, the insertion operation is provided for arithmetic types:
// - integral types bool
//   char, char16_t, char32_t, wchar_t, signed char,
//   short int, int, long int, long long int
//   unsigned char, unsigned short int, unsigned int, unsigned long int, unsigned long long int
// - floating point types  
//   float, double, long double
//
template <typename StreamBufferT>
class InputStream : public IOStreamBase <StreamBufferT> {
  
  public:
    
    InputStream(const ::std::shared_ptr<StreamBufferT>& = nullptr);

    template <typename T, typename = ::std::enable_if_t<::std::is_arithmetic<T>::value>>
    inline InputStream& operator >> (T&);

    inline InputStream& operator >> (::std::string&);

    inline ssize_t sync(const int);
};

// Constructor.
template <typename StreamBufferT>
InputStream<StreamBufferT>::InputStream(const ::std::shared_ptr<StreamBufferT>& tgt) :
  IOStreamBase<StreamBufferT>::IOStreamBase(tgt) {
}

// Operator: >>
// Arithmetic data types.
template <typename StreamBufferT>
template <typename T, typename>
inline InputStream<StreamBufferT>& InputStream<StreamBufferT>::operator >> (T& rhs) {
  this->clear_iostate();
  if(this->rdbuf()->in_avail() >= sizeof(T)) {
    this->rdbuf()->read(&rhs, sizeof(T));
  }
  else {
    this->insert_iostates(::std::ios_base::failbit); 
  }
  return *this;
}

// Operator: >>
// String data type.
template <typename StreamBufferT>
inline InputStream<StreamBufferT>& InputStream<StreamBufferT>::operator >> (::std::string& rhs) {

  rhs.clear(); 
  this->clear_iostate();

  while(this->rdbuf()->sgetc() != '\0' && this->rdbuf()->sgetc() != EOF) {
    rhs.push_back(this->rdbuf()->sbumpc());
  }
  
  // Successfully extract a string.
  if(this->rdbuf()->sgetc() == '\0') {
    this->rdbuf()->sbumpc();
  }
  else {
    this->insert_iostates(::std::ios_base::badbit);
  }

  return *this;
}

// Function: sync
template <typename StreamBufferT>
inline ssize_t InputStream<StreamBufferT>::sync(const int fd) {

  this->clear_iostate();

  auto ret = this->rdbuf()->sync(fd);

  if(ret < 0) {
    this->insert_iostates(::std::ios_base::failbit);
  }
  else if(ret == 0) {
    this->insert_iostates(::std::ios_base::badbit | ::std::ios_base::eofbit);
  }

  return ret;
}


//-------------------------------------------------------------------------------------------------


};  // End of namespace OpenCluster. --------------------------------------------------------------

#endif












