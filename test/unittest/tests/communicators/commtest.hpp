#include <thread>
#include <cstdlib>

#define DO_SEND_RECV_EXCHANGE(init_data, comp_data, send_method, send_args, recv_method, recv_args) \
  init_data;								\
  EXPECT_GE(sComm.send_method send_args, 0);				\
  EXPECT_GE(rComm.recv_method recv_args, 0);				\
  EXPECT_TRUE(sComm.afterSendRecv(&sComm, &rComm));			\
  comp_data
#define DO_SEND_RECV_EOF(recv_method, recv_args)	\
  EXPECT_GE(sComm.send_eof(), 0);					\
  EXPECT_GE(rComm.recv_method recv_args, -2);				\
  EXPECT_TRUE(sComm.afterSendRecv(&sComm, &rComm))

#define DO_SEND_RECV_BASE(init_data, comp_data, send_method, recv_method, send_args, recv_args) \
  DO_SEND_RECV_EXCHANGE(init_data, comp_data, send_method, send_args, recv_method, recv_args); \
  DO_SEND_RECV_EOF(recv_method, recv_args)

#define DO_SEND_RECV_REQUEST(init_data, comp_data, send_method, send_args, recv_method, recv_args) \
  DO_SEND_RECV_EXCHANGE(init_data, comp_data, send_method, send_args, recv_method, recv_args)
  
#define DO_SEND_RECV_RESPONSE(init_data, comp_data, send_method, send_args, recv_method, recv_args) \
  init_data;								\
  EXPECT_GE(rComm.send_method send_args, 0);				\
  EXPECT_GE(sComm.recv_method recv_args, 0);				\
  EXPECT_TRUE(rComm.afterSendRecv(&rComm, &sComm));			\
  comp_data;								\
  EXPECT_GE(sComm.send_eof(), 0);					\
  EXPECT_GE(rComm.recv_method recv_args, -2);				\
  EXPECT_TRUE(sComm.afterSendRecv(&sComm, &rComm))

#define DO_RPC_SIGNON							\
  {									\
    std::string msg_cli = YGG_CLIENT_SIGNON;				\
    Header header;							\
    EXPECT_TRUE(sComm.create_header_send(header, msg_cli.c_str(), msg_cli.size())); \
    size_t len = header.format(msg_cli.c_str(), msg_cli.size(), 0);	\
    msg_cli.assign(header.data[0], len);				\
    EXPECT_GE(rComm.requests.addRequestServer(header), 0);		\
    std::string msg_srv = YGG_SERVER_SIGNON;				\
    EXPECT_GE(rComm.send(msg_srv.c_str(), msg_srv.size()), 0);		\
  }


#define INIT_DATA_SINGLE(type, value)		\
  type data_send = value;			\
  type data_recv

#define INIT_DATA_CHAR				\
  const char data_send[10] = "hello";		\
  char data_recv[10];				\
  size_t n_send = 5;				\
  size_t n_recv = 10

#define INIT_DATA_DOUBLE			\
  int a_send = 1;				\
  int a_recv;					\
  double b_send = 5.5;				\
  double b_recv

#define INIT_DATA_TRIPLE			\
  INIT_DATA_DOUBLE;				\
  const char c_send[10] = "hello";		\
  char c_recv[10];				\
  size_t nc_send = 5;				\
  size_t nc_recv = 10

#define INIT_DATA_TRIPLE_REALLOC		\
  INIT_DATA_DOUBLE;				\
  const char* c_send = "hello";			\
  char* c_recv = NULL;				\
  size_t nc_send = 5;				\
  size_t nc_recv = 0

#define INIT_DATA_TABLE_SEND				\
  size_t n_send = 3;					\
  int a_send[3];					\
  a_send[0] = 0;					\
  a_send[1] = 1;					\
  a_send[2] = 2;					\
  double b_send[3];					\
  b_send[0] = 0.0;					\
  b_send[1] = 1.0;					\
  b_send[2] = 2.0;					\
  char c_send[30];					\
  memset(c_send, 0, 30);				\
  strcpy(&(c_send[0]), "one");				\
  strcpy(&(c_send[10]), "two");				\
  strcpy(&(c_send[20]), "three");			\
  size_t nc_send = 10
#define INIT_DATA_TABLE				\
  INIT_DATA_TABLE_SEND;				\
  size_t n_recv = 3;				\
  int a_recv[3];				\
  double b_recv[3];				\
  char c_recv[30]
#define INIT_DATA_TABLE_REALLOC			\
  INIT_DATA_TABLE_SEND;				\
  size_t n_recv = 0;				\
  int* a_recv = NULL;				\
  double* b_recv = NULL;			\
  char* c_recv = NULL

#define INIT_DATA_PLY						\
  rapidjson::Document sd;					\
  rapidjson::Document result;					\
  sd.Parse("{\"type\": \"ply\"}");				\
  rapidjson::SchemaDocument s(sd);				\
  rapidjson::SchemaValidator validator(s);			\
  validator.GenerateData(result);				\
  rapidjson::Ply data_send = result.GetPly();			\
  rapidjson::Ply data_recv
#define INIT_DATA_OBJ						\
  rapidjson::Document sd;					\
  rapidjson::Document result;					\
  sd.Parse("{\"type\": \"obj\"}");				\
  rapidjson::SchemaDocument s(sd);				\
  rapidjson::SchemaValidator validator(s);			\
  validator.GenerateData(result);				\
  rapidjson::ObjWavefront data_send = result.GetObjWavefront();	\
  rapidjson::ObjWavefront data_recv

#define INIT_DATA_SCHEMA(schema)				\
  rapidjson::Document sd;					\
  rapidjson::Document data_send;				\
  sd.Parse(schema);						\
  rapidjson::SchemaDocument s(sd);				\
  rapidjson::SchemaValidator validator(s);			\
  validator.GenerateData(data_send);				\
  rapidjson::Document data_recv

#define INIT_DATA_SCHEMA_C(schema)					\
  rapidjson::Document sd;						\
  rapidjson::Document* data_send_doc = new rapidjson::Document();	\
  rapidjson::Document* data_recv_doc = new rapidjson::Document();	\
  sd.Parse(schema);							\
  rapidjson::SchemaDocument s(sd);					\
  rapidjson::SchemaValidator validator(s);				\
  validator.GenerateData(*data_send_doc);				\
  generic_t data_send = init_generic();					\
  data_send.obj = (void*)data_send_doc;					\
  generic_t data_recv = init_generic();					\
  data_recv.obj = (void*)data_recv_doc

#define COMP_DATA_SINGLE			\
  EXPECT_EQ(data_send, data_recv)

#define COMP_DATA_CHAR				\
  n_recv = n_send;				\
  EXPECT_EQ(n_send, n_recv);			\
  EXPECT_EQ(strcmp(data_send, data_recv), 0)

#define COMP_DATA_TRIPLE			\
  EXPECT_EQ(a_send, a_recv);			\
  EXPECT_EQ(b_send, b_recv);			\
  EXPECT_EQ(strcmp(c_send, c_recv), 0)

#define COMP_DATA_TABLE_BASE(cmp_c)		\
  EXPECT_EQ(n_send, n_recv);			\
  if (n_send == n_recv) {			\
    for (size_t i = 0; i < n_send; i++) {	\
      EXPECT_EQ(a_send[i], a_recv[i]);		\
      EXPECT_EQ(b_send[i], b_recv[i]);		\
      cmp_c					\
    }						\
  }
#define COMP_TABLE_C				\
  EXPECT_EQ(strncmp(&(c_send[i * nc_send]), &(c_recv[i * nc_send]), nc_send), 0);
#define COMP_TABLE_C_REALLOC						\
  if (c_recv != NULL) {							\
    EXPECT_EQ(strncmp(&(c_send[i * nc_send]), c_recv + (i * nc_send), nc_send), 0); \
  }
#define COMP_DATA_TABLE				\
  COMP_DATA_TABLE_BASE(COMP_TABLE_C)
#define COMP_DATA_TABLE_REALLOC			\
  COMP_DATA_TABLE_BASE(COMP_TABLE_C_REALLOC);	\
  free(a_recv);					\
  free(b_recv);					\
  free(c_recv)

#define SEND_NARGS_TRIPLE			\
  (4, a_send, b_send, c_send, nc_send)
#define RECV_NARGS_TRIPLE			\
  (4, &a_recv, &b_recv, &c_recv, &nc_recv)
#define RECV_NARGS_TRIPLE_REALLOC		\
  (4, &a_recv, &b_recv, &c_recv, &nc_recv)

#define SEND_NARGS_TABLE			\
  (4, n_send, a_send, b_send, c_send)
#define RECV_NARGS_TABLE			\
  (4, &n_recv, &a_recv, &b_recv, &c_recv)
#define RECV_NARGS_TABLE_REALLOC			\
  (4, &n_recv, &a_recv, &b_recv, &c_recv)

#define DO_SEND_RECV(send_method, recv_method, type, value)		\
  DO_SEND_RECV_BASE(INIT_DATA_SINGLE(type, value), COMP_DATA_SINGLE,	\
		    send_method, recv_method, (data_send), (data_recv))

#define TEST_SEND_RECV(group, name, init_cls, init_data, comp_data, send_method, send_args, recv_method, recv_args) \
  TEST(group, name) {							\
    init_cls;								\
    DO_SEND_RECV_BASE(init_data, comp_data, send_method, recv_method,	\
		      send_args, recv_args);				\
  }
#define TEST_SEND_RECV_RPC(group, name, init_cls, init_data, comp_data, send_method, send_args, recv_method, recv_args)	\
  TEST(group, name) {							\
    init_cls;								\
    DO_RPC_SIGNON;							\
    {									\
      DO_SEND_RECV_REQUEST(init_data, comp_data, send_method, send_args, recv_method, recv_args); \
    }									\
    {									\
      DO_SEND_RECV_RESPONSE(init_data, comp_data, send_method, send_args, recv_method, recv_args); \
    }									\
  }

#define TESTER_METHODS(cls)						\
  cls ## _tester(const std::string name = "",				\
		 utils::Address *address = new utils::Address(),	\
		 const DIRECTION direction = NONE) :			\
  cls(name, address, direction) {}					\
  cls ## _tester(DIRECTION dir) :					\
  cls("", nullptr, dir) {}						\
  cls ## _tester(const std::string name, DIRECTION dir) :		\
  cls(name, dir) {}

#define COMM_SERI_TEST_TYPE(cls, type, value, schema)			\
  TEST(cls, type) {							\
    cls ## _tester sComm(SEND);						\
    sComm.addSchema(schema);						\
    std::string name = "test_name";					\
    std::string key_env = name + "_IN";					\
    std::string val_env = sComm.getAddress();				\
    setenv(key_env.c_str(), val_env.c_str(), 1);			\
    cls ## _tester rComm(name, RECV);					\
    DO_SEND_RECV(sendVar, recvVar, type, value);			\
    rComm.close();							\
    EXPECT_EQ(rComm.recvVar(data_recv), -1);				\
  }
#define COMM_SERI_TEST(cls)						\
  COMM_SERI_TEST_TYPE(cls, double, 1.5, "{\"type\": \"number\"}")	\
  COMM_SERI_TEST_TYPE(cls, int, 32, "{\"type\": \"integer\"}")		\
  COMM_SERI_TEST_TYPE(cls, uint8_t, 3u,					\
		      "{\"type\": \"scalar\","				\
		      " \"subtype\": \"uint\","				\
		      " \"precision\": 1}")				\
  COMM_SERI_TEST_TYPE(cls, bool, true, "{\"type\": \"boolean\"}")	\
  TEST(cls, large) {							\
    cls ## _tester sComm(SEND);						\
    std::string name = "test_name";					\
    std::string key_env = name + "_IN";					\
    std::string val_env = sComm.getAddress();				\
    setenv(key_env.c_str(), val_env.c_str(), 1);			\
    cls ## _tester rComm(name, RECV);					\
    if (sComm.getMaxMsgSize() > 0) {					\
      /* Add worker in advance so that send is successful */		\
      Comm_t* sComm_worker = sComm.getWorkers().get(&sComm, SEND);	\
      rComm.getWorkers().get(&rComm, RECV, new utils::Address(sComm_worker->getAddress())); \
      sComm_worker = nullptr;						\
      std::string bigMsg(sComm.getMaxMsgSize(), 'A');			\
      std::string data_send = "\"Test message\"";			\
      std::string data_recv;						\
      std::string schema("{\"serializer\": {\"datatype\": {\"type\": \"string\"}}, \"userData\": \""); \
      schema += bigMsg;							\
      schema += "\"}";							\
      sComm.addSchema(schema, true);					\
      EXPECT_EQ(rComm.wait_for_recv(100), 0);				\
      EXPECT_GE(sComm.send(data_send), 0);				\
      EXPECT_GE(rComm.recv(data_recv), 0);				\
      EXPECT_TRUE(sComm.afterSendRecv(&sComm, &rComm));			\
      EXPECT_EQ(data_send, data_recv);					\
      /* Error when sending message that can't fit in buffer */		\
      sComm.getFlags() |= COMM_ALWAYS_SEND_HEADER;			\
      utils::Metadata& metadata = sComm.getMetadata();				\
      metadata.initMeta();						\
      metadata.SetMetaString("invalid", bigMsg);			\
      EXPECT_THROW(sComm.send(data_send), std::exception);		\
    }									\
  }									\
  TEST(cls, global) {							\
    std::string name = "test_name";					\
    global_scope_comm = 1;						\
    {									\
      cls ## _tester sComm(name, nullptr, SEND);			\
      sComm.addSchema("{\"type\": \"number\"}");			\
      std::string key_env = name + "_IN";				\
      std::string val_env = sComm.getAddress();				\
      setenv(key_env.c_str(), val_env.c_str(), 1);			\
      cls ## _tester rComm(name, RECV);					\
      DO_SEND_RECV_EXCHANGE(INIT_DATA_SINGLE(double, 1.5),		\
			    COMP_DATA_SINGLE,				\
			    sendVar, (data_send),			\
			    recvVar, (data_recv));			\
    }									\
    {									\
      cls ## _tester sComm(name, nullptr, SEND);			\
      cls ## _tester rComm(name, RECV);					\
      DO_SEND_RECV_EXCHANGE(INIT_DATA_SINGLE(double, 1.5),		\
			    COMP_DATA_SINGLE,				\
			    sendVar, (data_send),			\
			    recvVar, (data_recv));			\
      DO_SEND_RECV_EOF(recvVar, (data_recv));				\
    }									\
    global_scope_comm = 0;						\
    Comm_t::_ygg_cleanup();						\
  }

#ifdef ELF_AVAILABLE
#if COMM_BASE == IPC_COMM
#define ELF_SEND(ret)				\
  ELF_BEGIN_F_RET(msgsnd, ret);			\
  RETVAL_INC_SEND = 0;				\
  RETVAL_INC_POLL = 0
#define ELF_SEND_REVERT				\
  ELF_END_F(msgsnd)
#define ELF_RECV(ret)				\
  ELF_BEGIN_F_RET(msgrcv, ret);			\
  RETVAL_INC_RECV = 0;				\
  RETVAL_INC_POLL = 0
#define ELF_RECV_REVERT				\
  ELF_END_F(msgrcv)
#elif COMM_BASE == ZMQ_COMM
#define ELF_SEND(ret)				\
  ELF_BEGIN_F_RET(zmq_sendmsg, ret);		\
  RETVAL_INC_SEND = 0;				\
  RETVAL_INC_POLL = 0
#define ELF_SEND_REVERT				\
  ELF_END_F(zmq_sendmsg)

#ifdef ZMQ_HAVE_POLLER
#define ELF_RECV(ret)				\
  ELF_BEGIN_F_RET(zmq_recvmsg, ret);		\
  ELF_BEGIN_F(zmq_poller_wait_all);		\
  RETVAL_INC_RECV = 0;				\
  RETVAL_INC_POLL = 0
#define ELF_RECV_REVERT				\
  ELF_END_F(zmq_recvmsg);			\
  ELF_END_F(zmq_poller_wait_all)
#else // ZMQ_HAVE_POLLER
#define ELF_RECV(ret)				\
  ELF_BEGIN_F_RET(zmq_recvmsg, ret);		\
  ELF_BEGIN_F(zmq_poll);			\
  RETVAL_INC_RECV = 0;				\
  RETVAL_INC_POLL = 0
#define ELF_RECV_REVERT				\
  ELF_END_F(zmq_recvmsg);			\
  ELF_END_F(zmq_poll)
#endif // ZMQ_HAVE_POLLER

#endif
#endif // ELF_AVAILABLE
