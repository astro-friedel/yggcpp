#include "../../unittest.hpp"
#include "utils/Address.hpp"
#include "utils/tools.hpp"
#include "communicators/ZMQComm.hpp"
#include "../../elf_hook.hpp"
#include "../../mock.hpp"
#include "commtest.hpp"

using namespace communication;
using namespace communication::communicator;
using namespace communication::mock;

// TODO:
// - ELF error in
//   - zmq_connect (return -1)
//   - zmq_socket (return NULL)
//   - zmq_bind (return -1 first time, 0 second),
//       zmq_errno set to EADDRINUSE the first time
//   - zmq_poll return -1
//   - zmq_msg_init_size return -1

#ifdef ZMQINSTALLED

namespace communication {
namespace testing {
class ZMQSocket_tester : public ZMQSocket {
public:
    ZMQSocket_tester(int type) : ZMQSocket(type) {}

    ZMQSocket_tester() = delete;

    // static bool getValid() { return ctx_valid;}

    // static size_t activeCount() {return activeSockets.size();}
};
}
}

class ZMQComm_tester : public ZMQComm {
public:
    TESTER_METHODS(ZMQComm)

    std::string getAdr() { return address->address();}

    void setReply() {
        // this->getReply()->addresses.push_back(new communication::utils::Address("ABCDE"));
      std::string adr;
      this->getReply().create(adr);
    }
};

COMM_SERI_TEST(ZMQComm)

// TODO: Socket tests?

// TEST(yggSockT, yggSockTest) {
//     ZMQSocket ygs(ZMQ_REQ);
//     EXPECT_EQ(communication::testing::ZMQSocket_tester::activeCount(), 1);
//     ygs.close();
//     EXPECT_EQ(communication::testing::ZMQSocket_tester::activeCount(), 0);
//     ZMQSocket::ctx_shutdown();
//     EXPECT_FALSE(communication::testing::ZMQSocket_tester::getValid());
//     ZMQSocket::ctx_shutdown();

//     ZMQSocket::get_context();
//     //int x = 2;
//     ZMQSocket ygs1(ZMQ_REQ);
//     ZMQSocket ygs2(ZMQ_REQ);
//     EXPECT_EQ(communication::testing::ZMQSocket_tester::activeCount(), 2);
//     ZMQSocket::ctx_shutdown();
//     EXPECT_EQ(communication::testing::ZMQSocket_tester::activeCount(), 0);

//     //communication::testing::ZMQSocket_tester ygs2(ZMQ_REQ);
//     //ygs2.setValid(false);


// }

TEST(ZMQComm, constructor) {
    std::string name = "";
    ZMQSocket::resetPort();
    ZMQComm_tester zmqc(name, nullptr, SEND);
    EXPECT_EQ(zmqc.comm_nmsg(), 0);
    auto *adrs = new utils::Address();
    ZMQComm_tester zmqr(name, adrs, RECV);
    EXPECT_EQ(zmqr.comm_nmsg(), 0);
#ifdef ELF_AVAILABLE
    name = "";
    ELF_BEGIN;
    // Failure when YGG_MODEL_INDEX not set
    {
      ZMQSocket::resetPort();
      std::string alt_name = "TestZMQComm";
      ELF_BEGIN_ALT_F(getenv);
      EXPECT_THROW(ZMQComm zmqc(alt_name, nullptr, SEND), std::runtime_error);
      ELF_END_F(getenv);
    }
    // Failure to create socket
    {
      ELF_BEGIN_F(zmq_socket);
      EXPECT_THROW(ZMQComm zmqc(name, nullptr, SEND), std::exception);
      ELF_END_F(zmq_socket);
    }
    // Failure to set socket options
    {
      ELF_BEGIN_F(zmq_setsockopt);
      EXPECT_THROW(ZMQComm zmqc(name, nullptr, SEND), std::runtime_error);
      ELF_END_F(zmq_setsockopt);
    }
    // Failure to connect
    {
      ELF_BEGIN_F(zmq_connect);
      EXPECT_THROW(ZMQComm zmqc("", new utils::Address("1.2.3.4"), RECV),
		   std::exception);
      ELF_END_F(zmq_connect);
    }
    {
      // Advance port on bind to existing, then fail
      RETVAL = EADDRINUSE;
      ELF_BEGIN_F(zmq_bind);
      ELF_BEGIN_F(zmq_errno);
      EXPECT_THROW(ZMQComm zmqc(name, nullptr, SEND), std::runtime_error);
      ELF_END_F(zmq_bind);
      ELF_END_F(zmq_errno);
    }
    {
      // Failure to get socket options
      ELF_BEGIN_F(zmq_getsockopt);
      RETMSG = "";
      EXPECT_THROW(ZMQComm zmqc(name, nullptr, SEND), std::runtime_error);
      // Failure to get endpoint
      RETMSG = "invalid";
      EXPECT_THROW(ZMQComm zmqc(name, nullptr, SEND), std::runtime_error);
      ELF_END_F(zmq_getsockopt);
    }
    ELF_END;
#endif // ELF_AVAILABLE
}

TEST(ZMQComm, exchange) {
  std::string name = "TestZMQ";
  ZMQComm sComm(name, nullptr, SEND);
  ZMQComm rComm(name, new utils::Address(sComm.getAddress()), RECV);
  std::string msg_send = "This is a test message";
  std::string msg_recv;
  EXPECT_GT(sComm.sendVar(msg_send), 0);
  EXPECT_GT(rComm.recvVar(msg_recv), 0);
  EXPECT_EQ(msg_send, msg_recv);
  EXPECT_TRUE(sComm.afterSendRecv(&sComm, &rComm));
  // Error in reply
  EXPECT_GT(sComm.sendVar(msg_send), 0);
  ZMQReply::set_return_val(false);
  EXPECT_EQ(rComm.recvVar(msg_recv), -1);
  EXPECT_EQ(sComm.sendVar(msg_send), -1);
  ZMQReply::set_return_val(true);
  EXPECT_GT(rComm.recvVar(msg_recv), 0);
  EXPECT_EQ(msg_send, msg_recv);
#ifdef ELF_AVAILABLE
  std::string msg_reply;
  ELF_BEGIN;
  // Failure in recv_stage1 send
  {
    ELF_SEND_T(ZMQ, -1);
    EXPECT_FALSE(sComm.afterSendRecv(&sComm, &rComm));
    ELF_SEND_REVERT_T(ZMQ);
  }
  // Failure in send_stage1 recv
  {
    ELF_RECV_T(ZMQ, -1);
    EXPECT_FALSE(sComm.afterSendRecv(&sComm, &rComm));
    ELF_RECV_REVERT_T(ZMQ);
  }
  // Failure in send_stage2 send
  {
    EXPECT_TRUE(sComm.getReply().send_stage1(msg_reply));
    ELF_SEND_T(ZMQ, -1);
    EXPECT_FALSE(sComm.getReply().send_stage2(msg_reply));
    ELF_SEND_REVERT_T(ZMQ);
  }
  // Failure in recv_stage2 recv
  {
    EXPECT_TRUE(sComm.getReply().send_stage2(msg_reply));
    ELF_RECV_T(ZMQ, -1);
    EXPECT_FALSE(rComm.getReply().recv_stage2());
    ELF_RECV_REVERT_T(ZMQ);
    EXPECT_TRUE(rComm.getReply().recv_stage2());
  }
  ELF_END;
#endif // ELF_AVAILABLE
}

TEST(ZMQComm, send) {
#ifdef ELF_AVAILABLE
    std::string name = "TestZMQSend";
    std::string mmsg = "This is a test message";
    ZMQComm_tester zmq(name, nullptr, SEND);
    zmq.setReply();
    ELF_BEGIN;
    // Failure to create message
    {
      ELF_BEGIN_ALT_F(zmq_msg_init_size);
      EXPECT_EQ(zmq.send(mmsg.c_str(), mmsg.size()), -1);
      ELF_END_F(zmq_msg_init_size);
    }
    {
      // Failure to send
      ELF_REPLACE_SEND_ZMQ;
      EXPECT_EQ(zmq.send(mmsg.c_str(), mmsg.size()), -1);
      // Successful send
      RETVAL = 0;
      EXPECT_GT(zmq.send(mmsg.c_str(), mmsg.size()), 0);
      std::string long_msg(YGG_MSG_MAX * 3 + 20, 'A');
      RETVAL = 10;
      EXPECT_GT(zmq.send(long_msg.c_str(), long_msg.size()), 0);
      ELF_RESTORE_SEND_ZMQ;
    }
    // These are only needed if the tester dosn't by pass do_reply
    // ELF_BEGIN_F(zmq_recvmsg);
    // ELF_BEGIN_F(zmq_poller_wait_all);
    // ELF_END_F(zmq_recvmsg);
    // ELF_END_F(zmq_poller_wait_all);
    ELF_END;
#endif // ELF_AVAILABLE
}

TEST(ZMQComm, recv) {
    std::string name = "TestZMQSend";
    ZMQComm_tester zmq_recv(name, nullptr, RECV);
    ZMQComm_tester zmq_send(name, nullptr, SEND);
#ifdef ELF_AVAILABLE
    ELF_BEGIN;
    char* data = NULL;
    size_t len = 0;
    // Replace recv
    ELF_RECV_T(ZMQ, 0);
    ELF_META_ZMQ(zmq_send);
    // Sucessful receive
    EXPECT_GE(zmq_recv.recv(data, len, true), 0);
    EXPECT_EQ(strcmp(data, RETMSG.c_str()), 0);
    // Fail on zmq_getsockopt to check if there is more to the message
    ELF_BEGIN_F(zmq_getsockopt);
    RETVAL = 0;
    RETVAL_INC_POLL = 0;
    RETVAL_INC_RECV = -1;
    RETMSG = "";
    EXPECT_GE(zmq_recv.recv(data, len, true), -1);
    ELF_END_F(zmq_getsockopt);
    // Fail receive on poll
    RETVAL = -1;
    RETVAL_INC_RECV = 0;
    EXPECT_EQ(zmq_recv.recv(data, len, true), -1);
    // Fail receive on receiving message
    RETVAL = 0;
    RETVAL_INC_POLL = -1;
    EXPECT_EQ(zmq_recv.recv(data, len, true), -1);
    // Fail receive on realloc
    RETVAL = 0;
    RETVAL_INC_POLL = 0;
    RETVAL_INC_RECV = 0;
    ELF_BEGIN_F(realloc);
    EXPECT_EQ(zmq_recv.recv(data, len, true), -1);
    ELF_END_F(realloc);
    // Fail on invalid header
    RETVAL = 0;
    RETVAL_INC_POLL = 0;
    RETVAL_INC_RECV = 0;
    RETMSG_META = "}";
    EXPECT_THROW(zmq_recv.recv(data, len, true), std::exception);
    // Cleanup
    free(data);
    ELF_RECV_REVERT_T(ZMQ);
    ELF_END;
#endif // ELF_AVAILABLE
}

TEST(ZMQComm, errors) {
  ZMQComm comm("", nullptr, SEND);
  std::string msg;
  comm.getReply().clear();
  EXPECT_FALSE(comm.getReply().recv_stage1(msg));
  EXPECT_FALSE(comm.getReply().recv_stage2(msg));
  EXPECT_FALSE(comm.getReply().send_stage1(msg));
  EXPECT_FALSE(comm.getReply().send_stage2(msg));
}

#else // ZMQINSTALLED

TEST(ZMQComm, constructor) {
    EXPECT_THROW(ZMQComm zmq, std::exception);
    std::string name = "";
    EXPECT_THROW(ZMQComm zmq2(name, nullptr, SEND), std::exception);
}

#endif // ZMQINSTALLED
