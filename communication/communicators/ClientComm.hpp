#pragma once

#include "RPCComm.hpp"
#ifdef COMM_BASE
namespace communication {
namespace communicator {

/**
 * Client communicator class, associated with a ServerComm. Actual communicator type
 * is determined at compile time based on available packages. It will be either
 * an IPCComm or ZMQComm
 */
class ClientComm : public RPCComm {
public:
    /**
     * Constructor
     * @param name The name of the communicator
     * @param address The address to associate with the communicator, if address is nullptr
     *                then an address will be created.
     * @param flgs Bitwise flags describing the communicator
     */
    explicit ClientComm(const std::string name = "",
			utils::Address *address = nullptr,
			int flgs = 0, const COMM_TYPE type = CLIENT_COMM);
    ADD_CONSTRUCTORS_RPC(ClientComm, CLIENT_COMM)

    /**
     * Destructor
     */
    ~ClientComm() override = default;
    void set_timeout_recv(int new_timeout) override;
    int get_timeout_recv() override;
    int wait_for_recv(const int& tout) override;

    using RPCComm::send;
    using RPCComm::recv;
    using RPCComm::comm_nmsg;

#ifndef YGG_TEST
protected:
#endif
    void init();
  virtual bool signon(const utils::Header& header, bool in_async=false);
    Comm_t* create_worker_send(utils::Header& head) override;
    Comm_t* create_worker_recv(utils::Header& head) override;
    bool create_header_send(utils::Header& header, const char* data, const size_t &len) override;
    bool create_header_recv(utils::Header& header, char*& data, const size_t &len,
			    size_t msg_len, int allow_realloc,
			    int temp) override;
    int send_single(const char *data, const size_t &len,
		    const utils::Header& header) override;
    long recv_single(char*& data, const size_t &len,
		     bool allow_realloc) override;

#ifndef YGG_TEST
private:
#endif
    static unsigned _client_rand_seeded;
};

}
} // communication

#endif
