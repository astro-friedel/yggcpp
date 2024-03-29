#pragma once


#include "utils/enums.hpp"
#include "utils/Address.hpp"
#include "utils/logging.hpp"
//#include "datatypes/datatypes.hpp"
#include "datatypes/dtype_t.hpp"
#include "utils/logging.hpp"
//#include "datatypes/YggPly.hpp"
//#include "datatypes/YggObj.hpp"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"

/*! @brief Bit flags. */
#define COMM_FLAG_VALID   0x00000001  //!< Set if the comm is initialized
#define COMM_FLAG_GLOBAL  0x00000002  //!< Set if the comm is global
#define COMM_FLAG_FILE    0x00000004  //!< Set if the comm connects to a file
#define COMM_FLAG_WORKER  0x00000008  //!< Set if the comm is a work comm
#define COMM_FLAG_CLIENT  0x00000010  //!< Set if the comm is a client
#define COMM_FLAG_SERVER  0x00000020  //!< Set if the comm is a server
#define COMM_FLAG_CLIENT_RESPONSE 0x00000040 //!< Set if the comm is a client response comm
#define COMM_ALWAYS_SEND_HEADER   0x00000080 //!< Set if the comm should always include a header in messages
#define COMM_ALLOW_MULTIPLE_COMMS 0x00000100 //!< Set if the comm should connect in a way that allow multiple connections

/*! @brief Bit flags that can be set for const comm */
#define COMM_FLAGS_USED   0x00000001  //!< Set if the comm has been used
#define COMM_EOF_SENT     0x00000002  //!< Set if EOF has been sent
#define COMM_EOF_RECV     0x00000004  //!< Set if EOF has been received

/*! @brief Set if the comm is the receiving comm for a client/server request connection */
#define COMM_FLAG_RPC     COMM_FLAG_SERVER | COMM_FLAG_CLIENT
#define COMM_NAME_SIZE 100
#define COMM_DIR_SIZE 100


namespace communication {

namespace communicator {
class ServerComm;

class ClientComm;

class Comm_t {
public:
    virtual ~Comm_t();

    int send(const dtype_t* dtype);
    long recv(dtype_t* dtype);

    virtual int comm_nmsg() const = 0;

    COMM_TYPE getType() const { return type; }
    bool valid() {return flags & COMM_FLAG_VALID;}

#ifdef YGG_TEST
    std::string getName() {return name;}
#endif
protected:
    friend ServerComm;
    friend ClientComm;

    int send(const std::string message) {
        return send(message.c_str(), message.size());
    }
    virtual int send(const char *data, const size_t &len) = 0;
    virtual long recv(char *data, const size_t &len, bool allow_realloc) = 0;

    //Comm_t(const Comm_t* comm, COMM_TYPE type);
    Comm_t(utils::Address *address, DIRECTION direction, const COMM_TYPE &t, int flgs = 0);

    explicit Comm_t(const std::string &name, DIRECTION direction = NONE, const COMM_TYPE &t = NULL_COMM);

    bool check_size(const size_t &len) const;

    virtual void init() = 0;

    virtual void reset() = 0;

    COMM_TYPE type; //!< Comm type.
    //void *other; //!< Pointer to additional information for the comm.
    std::string name; //!< Comm name.
    utils::Address *address; //!< Comm address.
    DIRECTION direction; //!< send or recv for direction messages will go.
    int flags; //!< Flags describing the status of the comm.
    int const_flags;  //!< Flags describing the status of the comm that can be est for const.
    ServerComm *info; //!< Pointer to any extra info comm requires.
    size_t maxMsgSize; //!< The maximum message size.
    size_t msgBufSize; //!< The size that should be reserved in messages.
    int index_in_register; //!< Index of the comm in the comm register.
    time_t *last_send; //!< Clock output at time of last send.
    int thread_id; //!< ID for the thread that created the comm.
};

Comm_t* new_Comm_t(const DIRECTION dir, const COMM_TYPE type, const std::string &name="", char* address=nullptr);
/*!
      @brief Communication structure.
     */
template<typename H, typename R>
class CommBase : public Comm_t {
public:
    CommBase() = delete;

    int comm_nmsg() const override {
        utils::ygglog_throw_error("Comm_nmsg of base class called, must be overridden");
        return -1;
    }
    using Comm_t::send;
    using Comm_t::recv;

protected:
    int send(const char *data, const size_t &len) override {
        utils::ygglog_throw_error("Send of base class called, must be overridden");
        return -1;
    }

    long recv(char *data, const size_t &len, bool allow_realloc) override {
        utils::ygglog_throw_error("Recv of base class called, must be overridden");
        return -1;
    }

    CommBase(utils::Address *address, DIRECTION direction, const COMM_TYPE &t);

    //CommBase(Comm_t* comm, const COMM_TYPE &type);
    explicit CommBase(const std::string &name, DIRECTION direction = NONE, const COMM_TYPE &t = NULL_COMM);

    void init() override {
        utils::ygglog_throw_error("init of base class called, must be overridden");
    }

    void reset() override;

    ~CommBase() override;

    H *handle; //!< Pointer to handle for comm.
    R *reply; //!< Reply information.
};

template<typename H, typename R>
CommBase<H, R>::CommBase(utils::Address *address, DIRECTION direction, const COMM_TYPE &t) :
        Comm_t(address, direction, t, 0) {
    //other = NULL_ptr;
    handle = nullptr;
    reply = nullptr;
}

template<typename H, typename R>
CommBase<H, R>::CommBase(const std::string &name, DIRECTION direction, const COMM_TYPE &t) :
        Comm_t(name, direction, t) {
}

template<typename H, typename R>
void CommBase<H, R>::reset() {
    if (handle != nullptr)
        delete handle;
    if (reply != nullptr)
        delete reply;
}

template<typename H, typename R>
CommBase<H, R>::~CommBase() {
    ygglog_debug << "~CommBase: Started";
    if (handle != nullptr)
        delete handle;
    if (reply != nullptr)
        delete reply;
    ygglog_debug << "~CommBase: Finished";
}


}
}
