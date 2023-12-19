#pragma once

#include "utils/tools.hpp"
#include "utils/logging.hpp"
// #if defined(ZMQINSTALLED) && defined(_MSC_VER)
// #define YGG_ZMQ_CATCH_ERROR_POST_UNLOAD 1
// #endif
#ifdef YGG_ZMQ_PRELOAD
#include <windows.h>
#endif // YGG_ZMQ_PRELOAD
#ifdef YGG_ZMQ_CATCH_ERROR_POST_UNLOAD
#include <windows.h> // for EXCEPTION_ACCESS_VIOLATION
#include <excpt.h>
#endif // YGG_ZMQ_CATCH_ERROR_POST_UNLOAD
#if defined(_WIN32) && !(defined(YGG_ZMQ_PRELOAD) || defined(YGG_ZMQ_CATCH_ERROR_POST_UNLOAD))
#define YGG_ZMQ_PRESERVE_CONTEXT 1
#endif

namespace YggInterface {
  namespace communicator {

    // Forward declaration
    class Comm_t;

    /**
     * Class for handling Yggdrasil communicator contexts including
     * cleanup on exit that is intended to be used as a singleton.
     */
    class YGG_API CommContext : public YggInterface::utils::LogBase {
    private:
      CommContext(const CommContext&) = delete;
      CommContext& operator=(const CommContext&) = delete;
    public:
      CommContext(bool for_testing=false);
      ~CommContext();
      std::string logClass() const override { return "CommContext"; }
      // std::string logInst() const override { return logInst_; }

      // Class members
      std::vector<Comm_t*> registry_;
      std::string thread_id;
      bool for_testing_;
      CLEANUP_MODE cleanup_mode_;
      void* zmq_ctx;
#ifdef YGG_ZMQ_PRELOAD
      HINSTANCE hzmqDLL;
#endif // YGG_ZMQ_PRELOAD
#ifdef THREADSINSTALLED
#define YGG_THREAD_MUTEX(name)			\
      std::mutex name ## _mutex;
      YGG_THREAD_MUTEX(init)
      YGG_THREAD_MUTEX(clean)
      YGG_THREAD_MUTEX(client)
      YGG_THREAD_MUTEX(comms)
      YGG_THREAD_MUTEX(IPCComm)
      YGG_THREAD_MUTEX(zmq)
      YGG_THREAD_MUTEX(zmqport)
#undef YGG_THREAD_MUTEX
#endif // THREADSINSTALLED

      int init(bool for_testing=false);
      void cleanup(CLEANUP_MODE mode=CLEANUP_DEFAULT);
      void register_comm(Comm_t* x);
      Comm_t* find_registered_comm(const std::string& name,
				   const DIRECTION dir,
				   const COMM_TYPE type);
#ifdef YGG_ZMQ_CATCH_ERROR_POST_UNLOAD
    protected:
      DWORD _HandleWSAStartupError(unsigned int code,
				   struct _EXCEPTION_POINTERS *ep);
#endif
    };

    YGG_THREAD_GLOBAL_VAR(int, global_scope_comm, )
    extern std::shared_ptr<CommContext> global_context;

    int get_global_scope_comm();
    void set_global_scope_comm(int new_value);
    void global_scope_comm_on();
    void global_scope_comm_off();
    int ygg_init(bool for_testing=false);
    void ygg_cleanup(CLEANUP_MODE mode=CLEANUP_DEFAULT);
    void ygg_exit();

  }
}
