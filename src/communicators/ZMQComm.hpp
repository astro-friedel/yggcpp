#pragma once

#include "CommBase.hpp"

#ifdef ZMQINSTALLED
#include <zmq.h>
#endif

#include <vector>
#include <memory>
namespace YggInterface {
namespace communicator {
  
YGG_THREAD_GLOBAL_VAR(char, _reply_msg, [100])
YGG_THREAD_GLOBAL_VAR(char, _purge_msg, [100])
YGG_THREAD_GLOBAL_VAR(int, _zmq_sleeptime, )

class ZMQSocket : public YggInterface::utils::LogBase {
private:
  ZMQSocket& operator=(const ZMQSocket& rhs) = delete;
public:
  ZMQSocket();
  ZMQSocket(const ZMQSocket& rhs);
  ZMQSocket(int type0, int linger = 0, int immediate = 1,
            int sndtimeo = -1);
  ZMQSocket(int type0, utils::Address& address,
	    int linger = 0, int immediate = 1,
	    int sndtimeo = -1);
  void init(int type0, const std::string& address,
	    int linger = 0, int immediate = 1,
	    int sndtimeo = -1);
#ifdef ZMQINSTALLED
  void init(int type0, utils::Address& address,
	    int linger = 0, int immediate = 1,
	    int sndtimeo = -1);
  int poll(int method, int tout);
  int send(const std::string& msg);
  template<typename T>
  int set(int member, const T& data);
  int recv(std::string& msg);
  void destroy();
#else
  void init(int, utils::Address&, int = 0, int = 1, int = -1) {
    UNINSTALLED_ERROR(ZMQ);
  }
  void destroy() {}
#endif
  ~ZMQSocket();
  std::string logClass() const override { return "ZMQSocket"; }
  std::string logInst() const override { return endpoint; }
  void *handle;                    //  The libzmq socket handle
  std::string endpoint;            //  Last bound endpoint, if any
  int type;                        //  Socket type
private:
  static int _last_port;
  static int _last_port_set;
public:
  // Test methods
  // friend class testing::ZMQSocket_tester;
  static void resetPort();
};

/*!
  @brief Struct to store info for reply.
*/
class ZMQReply : public YggInterface::utils::LogBase {
public:
  ZMQReply(DIRECTION dir);
#ifdef ZMQINSTALLED
  void clear();
  int create(std::string& endpoint);
  int find(std::string endpoint);
  int set(std::string endpoint = "");
  bool recv(std::string msg_send="", bool* closed=nullptr);
  bool recv_stage1(std::string msg_send="");
  bool recv_stage2(std::string msg_send="", bool* closed=nullptr);
  bool send();
  bool send_stage1(std::string& msg_data);
  bool send_stage2(const std::string msg_data);
#endif // ZMQINSTALLED
  std::string logClass() const override { return "ZMQReply"; }
  
  std::vector<ZMQSocket> sockets;
  int n_msg;
  int n_rep;
  DIRECTION direction;
  int last_idx;
  // Test methods
  static bool _test_return_val;
  static void set_test_return_val(bool new_val);
};

class YGG_API ZMQComm : public CommBase<ZMQSocket> {
public:
    /**
     * Constructor
     * @param name The name of the communicator
     * @param address The address to associate with the communicator, if address is nullptr
     *                then an address will be created.
     * @param direction Enumerated direction for communicator
     * @param flgs Bitwise flags describing the communicator
     */
    explicit ZMQComm(const std::string name,
                     const utils::Address& address,
                     const DIRECTION direction = NONE,
                     int flgs = 0, const COMM_TYPE type = ZMQ_COMM);
    ADD_CONSTRUCTORS(ZMQ)

#ifdef ZMQINSTALLED

    /*! \copydoc Comm_t::comm_nmsg */
    int comm_nmsg(DIRECTION dir=NONE) const override;
  
    using Comm_t::send;
    using Comm_t::recv;

protected:
    void init();
    int send_single(utils::Header& msg) override;
    long recv_single(utils::Header& msg) override;
    virtual bool do_reply_recv(const utils::Header& header);
    virtual bool do_reply_send(const utils::Header& header);
    bool create_header_send(utils::Header& header) override;
    WORKER_METHOD_DECS(ZMQComm);
    Comm_t* create_worker_send(utils::Header& head) override;
    Comm_t* create_worker_recv(utils::Header& head) override;
#else
    void init() { UNINSTALLED_ERROR(ZMQ); }
#endif

private:
    friend class ClientComm;
    friend class ServerComm;

    ZMQReply reply;

    // Test methods
 public:
#ifdef ZMQINSTALLED
    bool afterSendRecv(Comm_t* sComm, Comm_t* rComm) override;
    bool genMetadata(std::string& out) override;
#endif // ZMQINSTALLED
    ZMQReply& getReply() { return reply; }
    static void disable_handshake();
    static int _disable_handshake;
};

}
}