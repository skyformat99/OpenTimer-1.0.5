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

#ifndef OC_LOGGER_HPP_
#define OC_LOGGER_HPP_

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <thread>
#include <mutex>

namespace OpenCluster {

// Enum: LoggingSeverity
enum class LoggingSeverity {
  DEBUG   = 0,
  INFO    = 1,
  WARNING = 2,
  ERROR   = 3,
};

//-------------------------------------------------------------------------------------------------

// Class: LogPolicyInterface
class LogPolicyInterface {

  protected:

    ::std::ostream* _os {&::std::cerr};

  public:

    inline bool is_console() const;
    
    virtual void open(const std::string& name) = 0;
    virtual void close() = 0;
    virtual void write(const std::string& msg) = 0;
};

// Function: is_console
bool LogPolicyInterface::is_console() const {
  return _os == &::std::cerr || _os == &::std::cout;
}

//-------------------------------------------------------------------------------------------------

// Class: FileLogPolicy
class FileLogPolicy : public LogPolicyInterface {

  private:

    ::std::ofstream _ofs;

  public:

    inline FileLogPolicy();
    inline ~FileLogPolicy();

    inline void open(const ::std::string&) override;
    inline void close() override;
    inline void write(const ::std::string&) override;
};

// Constructor
inline FileLogPolicy::FileLogPolicy() {
}

// Destructor
inline FileLogPolicy::~FileLogPolicy() {
}
  
// Procedure: write
inline void FileLogPolicy::write(const ::std::string& msg) {
  *(_os) << msg << ::std::endl;
}

// Procedure: open
inline void FileLogPolicy::open(const ::std::string& name) {

  close();
  _ofs.open(name, ::std::ios_base::binary | ::std::ios_base::out);

  // Redirect the log to stderr.
  if(_ofs.is_open()) {
    _os = &_ofs;
  }
}

// Procedure: close
inline void FileLogPolicy::close() {
  if(_ofs.is_open()) {
    _ofs.close();
    _os = &::std::cerr;
  }
}
  
//-------------------------------------------------------------------------------------------------

// Class: Logger
template< typename LogPolicyT >
class Logger {

  private:
  
    static constexpr const char* _set_red     {"\033[1;31m"};
    static constexpr const char* _set_green   {"\033[1;32m"};
    static constexpr const char* _set_yellow  {"\033[1;33m"};
    static constexpr const char* _set_cyan    {"\033[1;36m"};
    static constexpr const char* _set_magenta {"\033[1;35m"};
    static constexpr const char* _reset_color {"\033[0m"   };
    
    LogPolicyT _policy;

    ::std::ostringstream _osstream;
    ::std::mutex _write_mutex;

  public:

    Logger(const ::std::string& fpath = "");
    ~Logger();

    template <LoggingSeverity severity, typename...ArgsT>
    void write(const char*, const int, ArgsT&&...);
    
    template <typename...ArgsT>
    void debug(const char*, const int, ArgsT&&...);
    
    template <typename...ArgsT>
    void info(const char*, const int, ArgsT&&...);
    
    template <typename...ArgsT>
    void warning(const char*, const int, ArgsT&&...);
    
    template <typename...ArgsT>
    void error(const char*, const int, ArgsT&&...);

    void redir(const ::std::string&);
  
  private:
    
    template <LoggingSeverity severity> 
    void _write_impl(const char*, const int);

    template <LoggingSeverity severity, typename FirstT, typename...RestT>
    void _write_impl(const char*, const int, const FirstT&, RestT&&...);
    
    template <LoggingSeverity severity>
    ::std::string _get_header(const char*, const int);

    constexpr const char* _strend(const char*) const;
    constexpr const char* _basename(const char*, const char*) const;
    constexpr const char* _basename(const char*) const;
};

// Constructor.
template <typename LogPolicyT>
Logger<LogPolicyT>::Logger(const ::std::string& fpath) {
  _policy.open(fpath);
}

// Destructor.
template <typename LogPolicyT>
Logger<LogPolicyT>::~Logger() {
  _policy.close();
}
    
// Function: _strend
// Compile-time finding of the end of a string.
template <typename LogPolicyT>
constexpr const char* Logger<LogPolicyT>::_strend(const char* str) const {
  return *str ? _strend(str + 1) : str;
}

// Function: _basename
// Compile-time finding of a file name.
template <typename LogPolicyT>
constexpr const char* Logger<LogPolicyT>::_basename(const char* beg, const char* end) const {
  return (end >= beg && *end != '/') ? _basename(beg, end - 1) : (end + 1);
}

// Function: _basename
// Compile-time finding of a file name.
template <typename LogPolicyT>
constexpr const char* Logger<LogPolicyT>::_basename(const char* fpath) const {
  return _basename(fpath, _strend(fpath));
}

// Procedure: write
template< typename LogPolicyT >
template< LoggingSeverity severity , typename...ArgsT >
void Logger< LogPolicyT >::write(const char* fpath, const int line, ArgsT&&... args) {
  _write_mutex.lock();
  _write_impl<severity>(fpath, line, ::std::forward<ArgsT>(args)...);
  _write_mutex.unlock();
}

// Procedure: redir
template <typename LogPolicyT>
void Logger<LogPolicyT>::redir(const ::std::string& fpath) {
  _policy.open(fpath); 
}

// Function:_get_header
template <typename LogPolicyT>
template <LoggingSeverity severity>
::std::string Logger<LogPolicyT>::_get_header(const char* fpath, const int line) {

  ::std::ostringstream header;
  
  switch(severity) {
    
    // Info.
    case LoggingSeverity::INFO:
      header << "I ";
    break;
    
    // Debug.
    case LoggingSeverity::DEBUG:
      if(_policy.is_console()) {
        header << _set_cyan;
      }
      header << "D ";
    break;
    
    // Warning.
    case LoggingSeverity::WARNING:
      if(_policy.is_console()) {
        header << _set_yellow;
      }
      header << "W ";
    break;

    // Error.
    case LoggingSeverity::ERROR:
      if(_policy.is_console()) {
        header << _set_red;
      }
      header << "E ";
    break;

    default:
      throw ::std::runtime_error("Undefined severity level\n");
    break;
  };
  
  // Insert timeinfo
  auto rt = ::std::time(nullptr);
  auto lt = *::std::localtime(&rt);
  header << ::std::put_time(&lt, "%Y-%m-%d %T ");
  header << _basename(fpath) << ":" << line << "] ";

  return header.str();
}

// Procedure: _write_impl 
template <typename LogPolicyT>
template <LoggingSeverity severity>
void Logger< LogPolicyT >::_write_impl(const char* fpath, const int line) {

  _policy.write(
    _policy.is_console() ? 
    _get_header<severity>(fpath, line) + _osstream.str() + _reset_color:
    _get_header<severity>(fpath, line) + _osstream.str()
  );

  _osstream.str("");
}

// Procedure: _write_impl
template <typename LogPolicyT>
template <LoggingSeverity severity, typename FirstT, typename...RestT>
void Logger< LogPolicyT >::_write_impl(const char* fpath, const int line, const FirstT& parm1, RestT&&...parm) {
  _osstream << parm1;
  _write_impl<severity>(fpath, line, ::std::forward<RestT>(parm)...);  
}

// Procedure: debug
template <typename LogPolicyT>
template <typename... ArgsT>
void Logger<LogPolicyT>::debug(const char* fpath, const int line, ArgsT&&... args) {
  write<LoggingSeverity::DEBUG>(fpath, line, ::std::forward<ArgsT>(args)...);
}

// Procedure: info
template <typename LogPolicyT>
template <typename... ArgsT>
void Logger<LogPolicyT>::info(const char* fpath, const int line, ArgsT&&... args) {
  write<LoggingSeverity::INFO>(fpath, line, ::std::forward<ArgsT>(args)...);
}

// Procedure: warning
template <typename LogPolicyT>
template <typename... ArgsT>
void Logger<LogPolicyT>::warning(const char* fpath, const int line, ArgsT&&... args) {
  write<LoggingSeverity::WARNING>(fpath, line, ::std::forward<ArgsT>(args)...);
}

// Procedure: error
template <typename LogPolicyT>
template <typename... ArgsT>
void Logger<LogPolicyT>::error(const char* fpath, const int line, ArgsT&&... args) {
  write<LoggingSeverity::ERROR>(fpath, line, ::std::forward<ArgsT>(args)...);
}

//-------------------------------------------------------------------------------------------------

// Global declaration.
static Logger<FileLogPolicy> logger;

//-------------------------------------------------------------------------------------------------

// Logging convenience macros.
#define LOGD(...) logger.debug  (__FILE__, __LINE__, __VA_ARGS__)
#define LOGI(...) logger.info   (__FILE__, __LINE__, __VA_ARGS__)
#define LOGW(...) logger.warning(__FILE__, __LINE__, __VA_ARGS__)
#define LOGE(...) logger.error  (__FILE__, __LINE__, __VA_ARGS__)

};  // End of namespace OpenCluster. ---------------------------------------------------------------------


#endif



