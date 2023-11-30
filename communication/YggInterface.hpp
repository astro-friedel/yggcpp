#pragma once
#include "communicators/comms.hpp"

/*! @brief Memory to keep track of global scope comms. */
#define WITH_GLOBAL_SCOPE(COMM) global_scope_comm_on(); COMM; global_scope_comm_off()


using namespace communication::communicator;

/*!
  @brief Input communicator that can be used to receive messages from
    other models/files in a Yggdrasil integration.
 */
class YggInput : public WrapComm {
public:

  /*!
    @brief Constructor for YggInput.
    @param[in] name Name of input channel. This should be named as a
      model input the in YAML for the model calling it.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggInput(const char *name, int flags = 0,
	   const COMM_TYPE commtype = DEFAULT_COMM) :
    WrapComm(name, RECV, flags | COMM_FLAG_INTERFACE, commtype) {
  }
  
  /*!
    @brief Constructor for YggInput w/ C++ std::string.
    @param[in] name Name of input channel. This should be named as a
      model input the in YAML for the model calling it.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggInput(const std::string name, int flags = 0,
	   const COMM_TYPE commtype = DEFAULT_COMM) :
    WrapComm(name, RECV, flags | COMM_FLAG_INTERFACE, commtype) {}

  /*!
    @brief Constructor for YggInput with format.
    @param[in] name Name of input channel. This should be named as a
      model input the in YAML for the model calling it.
    @param[in] fmt Format string specifying the datatype of messages
      that will be received using this comm.
    @param[in] as_array If true, messages will contain arrays for columns
      in the table.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggInput(const char *name, const char *fmt, bool as_array = false,
	   int flags = 0, const COMM_TYPE commtype = DEFAULT_COMM) :
    WrapComm(name, RECV, flags | COMM_FLAG_INTERFACE, commtype) {
    if (fmt && !this->addFormat(fmt, as_array))
      this->throw_error("Invalid format");
  }

  /*!
    @brief Constructor for YggInput with format.
    @param[in] name Name of input channel. This should be named as a
      model input the in YAML for the model calling it.
    @param[in] fmt Format string specifying the datatype of messages
      that will be received using this comm.
    @param[in] as_array If true, messages will contain arrays for columns
      in the table.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggInput(const std::string name, const std::string fmt,
	   bool as_array = false, int flags = 0,
	   const COMM_TYPE commtype = DEFAULT_COMM) :
    WrapComm(name, RECV, flags | COMM_FLAG_INTERFACE, commtype) {
    if (!this->addFormat(fmt, as_array))
      this->throw_error("Invalid format");
  }    

  /*!
    @brief Constructor for YggInput with explicit datatype.
    @param[in] name Name of input channel. This should be named as a
      model input the in YAML for the model calling it.
    @param[in] schema Document containing JSON schema describing the type
      of data expected by the communicator.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggInput(const char *name, const rapidjson::Document& schema,
	   int flags = 0, const COMM_TYPE commtype = DEFAULT_COMM) :
    WrapComm(name, RECV, flags | COMM_FLAG_INTERFACE, commtype) {
    if (!this->addSchema(schema))
      this->throw_error("Invalid schema");
  }

  /*!
    @brief Constructor for YggInput with explicit datatype.
    @param[in] name Name of input channel. This should be named as a
      model input the in YAML for the model calling it.
    @param[in] schema Document containing JSON schema describing the type
      of data expected by the communicator.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggInput(const std::string name, const rapidjson::Document& schema,
	   int flags = 0, const COMM_TYPE commtype = DEFAULT_COMM) :
    WrapComm(name, RECV, flags | COMM_FLAG_INTERFACE, commtype) {
    if (!this->addSchema(schema))
      this->throw_error("Invalid schema");
  }

};


/*!
  @brief Output communicator that can be used to send messages to
    other models/files in a Yggdrasil integration.
 */
class YggOutput : public WrapComm {
public:
  
  /*!
    @brief Constructor for YggOutput.
    @param[in] name Name of output channel. This should be named as a
      model output the in YAML for the model calling it.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggOutput(const char *name, int flags = 0,
	    const COMM_TYPE commtype = DEFAULT_COMM) :
    WrapComm(name, SEND, flags | COMM_FLAG_INTERFACE, commtype) {}
  
  /*!
    @brief Constructor for YggOutput.
    @param[in] name Name of output channel. This should be named as a
      model output the in YAML for the model calling it.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggOutput(const std::string name, int flags = 0,
	    const COMM_TYPE commtype = DEFAULT_COMM) :
    WrapComm(name, SEND, flags | COMM_FLAG_INTERFACE, commtype) {}
  
  /*!
    @brief Constructor for YggOutput with format.
    @param[in] name Name of output channel. This should be named as a
      model output the in YAML for the model calling it.
    @param[in] fmt Format string specifying the datatype of messages
      that will be sent using this comm. If messages are sent to an
      ASCII table, fmt will also be used to format messages in the table.
    @param[in] as_array If true, messages will contain arrays for columns
      in the table.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggOutput(const char *name, const char *fmt, bool as_array=false,
	    int flags = 0, const COMM_TYPE commtype = DEFAULT_COMM) :
    WrapComm(name, SEND, flags | COMM_FLAG_INTERFACE, commtype) {
    if (fmt && !this->addFormat(fmt, as_array))
      this->throw_error("Invalid format");
  }

  /*!
    @brief Constructor for YggOutput with format.
    @param[in] name Name of output channel. This should be named as a
      model output the in YAML for the model calling it.
    @param[in] fmt Format string specifying the datatype of messages
      that will be sent using this comm. If messages are sent to an
      ASCII table, fmt will also be used to format messages in the table.
    @param[in] as_array If true, messages will contain arrays for columns
      in the table.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggOutput(const std::string name, const std::string fmt,
	    bool as_array=false, int flags = 0,
	    const COMM_TYPE commtype = DEFAULT_COMM) :
    WrapComm(name, SEND, flags | COMM_FLAG_INTERFACE, commtype) {
    if (!this->addFormat(fmt, as_array))
      this->throw_error("Invalid format");
  }
    

  /*!
    @brief Constructor for YggOutput with explicit datatype.
    @param[in] name Name of output channel. This should be named as a
      model output the in YAML for the model calling it.
    @param[in] schema Document containing JSON schema describing the type
      of data expected by the communicator.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggOutput(const char *name, rapidjson::Document& schema,
	    int flags = 0, const COMM_TYPE commtype = DEFAULT_COMM) :
    WrapComm(name, SEND, flags | COMM_FLAG_INTERFACE, commtype) {
    if (!this->addSchema(schema))
      this->throw_error("Invalid schema");
  }

  /*!
    @brief Constructor for YggOutput with explicit datatype.
    @param[in] name Name of output channel. This should be named as a
      model output the in YAML for the model calling it.
    @param[in] schema Document containing JSON schema describing the type
      of data expected by the communicator.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggOutput(const std::string name, rapidjson::Document& schema,
	    int flags = 0, const COMM_TYPE commtype = DEFAULT_COMM) :
    WrapComm(name, SEND, flags | COMM_FLAG_INTERFACE, commtype) {
    if (!this->addSchema(schema))
      this->throw_error("Invalid schema");
  }

};
	

/*!
  @brief Server communicator that can be used to receive and reply to
    requests from client models in a Yggdrasil integration.
 */
class YggRpcServer : public ServerComm {
public:

  /*!
    @brief Constructor for YggRpcServer.
    @param[in] name Name of server input channel. This should be named in
      a model's 'server' field in the YAML or the name of the model with
      'server' set to 'true'.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] request_commtype Type of communicator that should be used
      for the request communicator. Defaults to DEFAULT_COMM that is set
      based on the available packages at compilation.
    @param[in] response_commtype Type of communicator that should be used
      for the response communicator. Defaults to DEFAULT_COMM that is
      set based on the available packages at compilation.
  */
  YggRpcServer(const char *name, int flags = 0,
	       const COMM_TYPE request_commtype = DEFAULT_COMM,
	       const COMM_TYPE response_commtype = DEFAULT_COMM) :
    ServerComm(name, flags | COMM_FLAG_INTERFACE,
	       SERVER_COMM, request_commtype, response_commtype) {}
  /*!
    @brief Constructor for YggRpcServer.
    @param[in] name Name of server input channel. This should be named in
      a model's 'server' field in the YAML or the name of the model with
      'server' set to 'true'.
    @param[in] inFormat Format string specifying the datatype of messages
      that will be received using this comm.
    @param[in] outFormat Format string specifying the datatype of
      messages that will be sent using this comm.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] request_commtype Type of communicator that should be used
      for the request communicator. Defaults to DEFAULT_COMM that is set
      based on the available packages at compilation.
    @param[in] response_commtype Type of communicator that should be used
      for the response communicator. Defaults to DEFAULT_COMM that is
      set based on the available packages at compilation.
   */
  YggRpcServer(const char *name, const char *inFormat,
	       const char *outFormat, int flags = 0,
	       const COMM_TYPE request_commtype = DEFAULT_COMM,
	       const COMM_TYPE response_commtype = DEFAULT_COMM) :
    ServerComm(name, flags | COMM_FLAG_INTERFACE,
	       SERVER_COMM, request_commtype, response_commtype) {
    if (!this->addFormat(inFormat))
      this->throw_error("Invalid request format");
    if (!this->addResponseFormat(outFormat))
      this->throw_error("Invalid response format");
  }

  /*!
    @brief Constructor for YggRpcServer.
    @param[in] name Name of server input channel. This should be named in
      a model's 'server' field in the YAML or the name of the model with
      'server' set to 'true'.
    @param[in] inFormat Format string specifying the datatype of messages
      that will be received using this comm.
    @param[in] outFormat Format string specifying the datatype of
      messages that will be sent using this comm.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] request_commtype Type of communicator that should be used
      for the request communicator. Defaults to DEFAULT_COMM that is set
      based on the available packages at compilation.
    @param[in] response_commtype Type of communicator that should be used
      for the response communicator. Defaults to DEFAULT_COMM that is
      set based on the available packages at compilation.
   */
  YggRpcServer(const std::string name, const std::string inFormat,
	       const std::string outFormat, int flags = 0,
	       const COMM_TYPE request_commtype = DEFAULT_COMM,
	       const COMM_TYPE response_commtype = DEFAULT_COMM) :
    ServerComm(name, flags | COMM_FLAG_INTERFACE,
	       SERVER_COMM, request_commtype, response_commtype) {
    if (!this->addFormat(inFormat))
      this->throw_error("Invalid request format");
    if (!this->addResponseFormat(outFormat))
      this->throw_error("Invalid response format");
  }

  /*!
    @brief Constructor for YggRpcServer with explicit datatype.
    @param[in] name Name of server input channel. This should be named in
      a model's 'server' field in the YAML or the name of the model with
      'server' set to 'true'.
    @param[in] inType Document containing JSON schema describing the type
      of data expected to be received by the communicator.
    @param[in] outType Document containing JSON schema describing the
      type of data expected to be sent by the communicator.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] request_commtype Type of communicator that should be used
      for the request communicator. Defaults to DEFAULT_COMM that is set
      based on the available packages at compilation.
    @param[in] response_commtype Type of communicator that should be used
      for the response communicator. Defaults to DEFAULT_COMM that is
      set based on the available packages at compilation.
   */
  YggRpcServer(const std::string name, const rapidjson::Document& inType,
	       const rapidjson::Document& outType, int flags = 0,
	       const COMM_TYPE request_commtype = DEFAULT_COMM,
	       const COMM_TYPE response_commtype = DEFAULT_COMM) :
    ServerComm(name, flags | COMM_FLAG_INTERFACE,
	       SERVER_COMM, request_commtype, response_commtype) {
    if (!this->addSchema(inType))
      this->throw_error("Invalid request schema");
    if (!this->addResponseSchema(outType))
      this->throw_error("Invalid response schema");
  }

};


/*!
  @brief Client communicator that can be used to send requests to and
    receive replies from server models in a Yggdrasil integration.
 */
class YggRpcClient : public ClientComm {
public:

  /*!
    @brief Constructor for YggRpcClient.
    @param[in] name Name of client input channel. This should be of the
      format '{server model name}_{client model name}' with the model
      names specified in the YAML.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] request_commtype Type of communicator that should be used
      for the request communicator. Defaults to DEFAULT_COMM that is set
      based on the available packages at compilation.
    @param[in] response_commtype Type of communicator that should be
      used for response communicators. Defaults to DEFAULT_COMM that is
      set based on the available packages at compilation.
  */
  YggRpcClient(const char *name, int flags = 0,
	       const COMM_TYPE request_commtype = DEFAULT_COMM,
	       const COMM_TYPE response_commtype = DEFAULT_COMM) :
    ClientComm(name, flags | COMM_FLAG_INTERFACE,
	       CLIENT_COMM, request_commtype, response_commtype) {}
  /*!
    @brief Constructor for YggRpcClient.
    @param[in] name Name of client input channel. This should be of the
      format '{server model name}_{client model name}' with the model
      names specified in the YAML.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] request_commtype Type of communicator that should be used
      for the request communicator. Defaults to DEFAULT_COMM that is set
      based on the available packages at compilation.
    @param[in] response_commtype Type of communicator that should be
      used for response communicators. Defaults to DEFAULT_COMM that is
      set based on the available packages at compilation.
  */
  YggRpcClient(const std::string name, int flags = 0,
	       const COMM_TYPE request_commtype = DEFAULT_COMM,
	       const COMM_TYPE response_commtype = DEFAULT_COMM) :
    ClientComm(name, flags | COMM_FLAG_INTERFACE,
	       CLIENT_COMM, request_commtype, response_commtype) {}
  /*!
    @brief Constructor for YggRpcClient.
    @param[in] name Name of client input channel. This should be of the
      format '{server model name}_{client model name}' with the model
      names specified in the YAML.
    @param[in] outFormat Format string specifying the datatype of
      messages that will be sent using this comm.
    @param[in] inFormat Format string specifying the datatype of messages
      that will be received using this comm.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] request_commtype Type of communicator that should be used
      for the request communicator. Defaults to DEFAULT_COMM that is set
      based on the available packages at compilation.
    @param[in] response_commtype Type of communicator that should be
      used for response communicators. Defaults to DEFAULT_COMM that is
      set based on the available packages at compilation.
   */
  YggRpcClient(const char *name, const char *outFormat,
	       const char *inFormat, int flags = 0,
	       const COMM_TYPE request_commtype = DEFAULT_COMM,
	       const COMM_TYPE response_commtype = DEFAULT_COMM) :
    ClientComm(name, flags | COMM_FLAG_INTERFACE,
	       CLIENT_COMM, request_commtype, response_commtype) {
    if (!this->addFormat(std::string(outFormat)))
      this->throw_error("Invalid request format");
    if (!this->addResponseFormat(std::string(inFormat)))
      this->throw_error("Invalid response format");
  }
    

  /*!
    @brief Constructor for YggRpcClient.
    @param[in] name Name of client input channel. This should be of the
      format '{server model name}_{client model name}' with the model
      names specified in the YAML.
    @param[in] outFormat Format string specifying the datatype of
      messages that will be sent using this comm.
    @param[in] inFormat Format string specifying the datatype of messages
      that will be received using this comm.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] request_commtype Type of communicator that should be used
      for the request communicator. Defaults to DEFAULT_COMM that is set
      based on the available packages at compilation.
    @param[in] response_commtype Type of communicator that should be
      used for response communicators. Defaults to DEFAULT_COMM that is
      set based on the available packages at compilation.
   */
  YggRpcClient(const std::string name, const std::string outFormat,
	       const std::string inFormat, int flags = 0,
	       const COMM_TYPE request_commtype = DEFAULT_COMM,
	       const COMM_TYPE response_commtype = DEFAULT_COMM) :
    ClientComm(name, flags | COMM_FLAG_INTERFACE,
	       CLIENT_COMM, request_commtype, response_commtype) {
    if (!this->addFormat(outFormat))
      this->throw_error("Invalid request format");
    if (!this->addResponseFormat(inFormat))
      this->throw_error("Invalid response format");
  }

  /*!
    @brief Constructor for YggRpcClient with explicit datatype.
    @param[in] name Name of client input channel. This should be of the
      format '{server model name}_{client model name}' with the model
      names specified in the YAML.
    @param[in] outType Document containing JSON schema describing the type
      of data expected to be sent by the communicator.
    @param[in] inType Document containing JSON schema describing the type
      of data expected to be received by the communicator.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] request_commtype Type of communicator that should be used
      for the request communicator. Defaults to DEFAULT_COMM that is set
      based on the available packages at compilation.
    @param[in] response_commtype Type of communicator that should be
      used for response communicators. Defaults to DEFAULT_COMM that is
      set based on the available packages at compilation.
   */
  YggRpcClient(const std::string name, const rapidjson::Document& outType,
	       const rapidjson::Document& inType, int flags = 0,
	       const COMM_TYPE request_commtype = DEFAULT_COMM,
	       const COMM_TYPE response_commtype = DEFAULT_COMM) :
    ClientComm(name, flags | COMM_FLAG_INTERFACE,
	       CLIENT_COMM, request_commtype, response_commtype) {
    if (!this->addSchema(outType))
      this->throw_error("Invalid request schema");
    if (!this->addResponseSchema(inType))
      this->throw_error("Invalid response schema");
  }
  
};


/*!
  @brief Timesync communicator for syncing model states with other
    models in a Yggdrasil integration.
 */
class YggTimesync : public YggRpcClient {
public:

  /*!
    @brief Constructor for YggTimesync.
    @param[in] name Name of timesync input channel. This should be
      the name of the timesync parameter in the YAML of the model
      calling it. If one is not provided, it will default to
      'timesync'.
    @param[in] t_units Units that should be used for the timestep. ""
      indicates no units.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] request_commtype Type of communicator that should be used
      for the request communicator. Defaults to DEFAULT_COMM that is set
      based on the available packages at compilation.
    @param[in] response_commtype Type of communicator that should be
      used for response communicators. Defaults to DEFAULT_COMM that is
      set based on the available packages at compilation.
   */
  YggTimesync(const std::string name="timesync",
	      const std::string t_units="", int flags = 0,
	      const COMM_TYPE request_commtype = DEFAULT_COMM,
	      const COMM_TYPE response_commtype = DEFAULT_COMM) :
    YggRpcClient(name, flags, request_commtype, response_commtype) {
    if (!this->addSchema("{ \"type\": \"array\","
			 "  \"items\": ["
			 "    {"
			 "      \"type\": \"scalar\", "
			 "      \"subtype\": \"float\","
			 "      \"precision\": 8"
			 "    },"
			 "    { \"type\": \"object\" }"
			 "  ]"
			 "}"))
      this->throw_error("Invalid time schema");
    if (t_units.size() > 0) {
      (*(this->getMetadata().getSchema()))["items"][0].AddMember(
	 rapidjson::Value("units", 5,
			  this->getMetadata().GetAllocator()).Move(),
	 rapidjson::Value(t_units.c_str(),
			  static_cast<rapidjson::SizeType>(t_units.size()),
			  this->getMetadata().GetAllocator()).Move(),
	 this->getMetadata().GetAllocator());
    }
    if (!this->addResponseSchema("{ \"type\": \"object\" }", true))
      this->throw_error("Invalid state schema");
  }
  
  /*!
    @brief Constructor for YggTimesync.
    @param[in] name Name of timesync input channel. This should be
      the name of the timesync parameter in the YAML of the model
      calling it. If one is not provided, it will default to
      'timesync'.
    @param[in] t_units Units that should be used for the timestep. ""
      indicates no units.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] request_commtype Type of communicator that should be used
      for the request communicator. Defaults to DEFAULT_COMM that is set
      based on the available packages at compilation.
    @param[in] response_commtype Type of communicator that should be
      used for response communicators. Defaults to DEFAULT_COMM that is
      set based on the available packages at compilation.
   */
  YggTimesync(const char *name="timesync", const char *t_units="",
	      int flags = 0,
	      const COMM_TYPE request_commtype = DEFAULT_COMM,
	      const COMM_TYPE response_commtype = DEFAULT_COMM) :
    YggTimesync(std::string(name), std::string(t_units),
		flags, request_commtype, response_commtype) {}
  
};


/*!
  @brief Output communicator that can be used to send bytes messages to
    other models/files in a Yggdrasil integration.
 */
class YggAsciiFileOutput : public YggOutput {
public:

  /*!
    @brief Constructor for YggAsciiFileOutput.
    @param[in] name Name of output channel. This should be named as a
      model output the in YAML for the model calling it.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggAsciiFileOutput(const char *name, int flags = 0,
		     const COMM_TYPE commtype = DEFAULT_COMM) :
    YggOutput(name, flags, commtype) {}
  
  /*!
    @brief Constructor for YggAsciiFileOutput.
    @param[in] name Name of output channel. This should be named as a
      model output the in YAML for the model calling it.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggAsciiFileOutput(const std::string name, int flags = 0,
		     const COMM_TYPE commtype = DEFAULT_COMM) :
    YggOutput(name, flags, commtype) {}
  
  /*!
    @brief Send a single line to a file or queue.
    @param[in] line character pointer to line that should be sent.
    @returns int 0 if send was succesfull. All other values indicate errors.
   */
  int send_line(const char *line) { return send(line, strlen(line)); }

};


/*!
  @brief Input communicator that can be used to receive bytes messages
    from other models/files in a Yggdrasil integration.
 */
class YggAsciiFileInput : public YggInput {
public:

  /*!
    @brief Constructor for YggAsciiFileInput.
    @param[in] name Name of input channel. This should be named as a
      model input the in YAML for the model calling it.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggAsciiFileInput(const char *name, int flags = 0,
		    const COMM_TYPE commtype = DEFAULT_COMM) :
    YggInput(name, flags, commtype) {}

  /*!
    @brief Constructor for YggAsciiFileInput.
    @param[in] name Name of input channel. This should be named as a
      model input the in YAML for the model calling it.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggAsciiFileInput(const std::string name, int flags = 0,
		    const COMM_TYPE commtype = DEFAULT_COMM) :
    YggInput(name, flags, commtype) {}

  /*!
    @brief Receive a single line from an associated file or queue.
    @param[out] line Pointer to allocate memory where the received
      should be stored.
    @param[in] n Size of the allocated memory block in bytes.
    @returns Number of bytes read/received. Negative values indicate
      that there was either an error or the EOF message was received.
   */
  long recv_line(char *line, const size_t n)
  { return this->recv(line, n, false); }
  
};


/*!
  @brief Output communicator that can be used to send table rows to
    other models/files in a Yggdrasil integration.
 */
class YggAsciiTableOutput : public YggOutput {
public:

  /*!
    @brief Constructor for YggAsciiTableOutput.
    @param[in] name Name of output channel. This should be named as a
      model output the in YAML for the model calling it.
    @param[in] fmt Format string specifying the datatype of messages
      that will be sent using this comm. If messages are sent to an
      ASCII table, fmt will also be used to format messages in the table.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggAsciiTableOutput(const char *name, const char *fmt, int flags = 0,
		      const COMM_TYPE commtype = DEFAULT_COMM) :
    YggOutput(name, fmt, false, flags, commtype) {}

  /*!
    @brief Constructor for YggAsciiTableOutput.
    @param[in] name Name of output channel. This should be named as a
      model output the in YAML for the model calling it.
    @param[in] fmt Format string specifying the datatype of messages
      that will be sent using this comm. If messages are sent to an
      ASCII table, fmt will also be used to format messages in the table.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggAsciiTableOutput(const std::string name, const std::string fmt,
		      int flags = 0,
		      const COMM_TYPE commtype = DEFAULT_COMM) :
    YggOutput(name, fmt, false, flags, commtype) {}

};


/*!
  @brief Input communicator that can be used to receive table rows
    from other models/files in a Yggdrasil integration.
 */
class YggAsciiTableInput : public YggInput {
public:

  /*!
    @brief Constructor for YggAsciiTableInput.
    @param[in] name Name of input channel. This should be named as a
      model input the in YAML for the model calling it.
    @param[in] fmt Format string specifying the datatype of messages
      that will be received using this comm.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggAsciiTableInput(const char *name, const char* fmt = nullptr,
		     int flags = 0,
		     const COMM_TYPE commtype = DEFAULT_COMM) :
    YggInput(name, fmt, false, flags, commtype) {}

  /*!
    @brief Constructor for YggAsciiTableInput.
    @param[in] name Name of input channel. This should be named as a
      model input the in YAML for the model calling it.
    @param[in] fmt Format string specifying the datatype of messages
      that will be received using this comm.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggAsciiTableInput(const std::string name, std::string fmt = "",
		     int flags = 0,
		     const COMM_TYPE commtype = DEFAULT_COMM) :
    YggInput(name, fmt, false, flags, commtype) {}

};

/*!
  @brief Output communicator that can be used to send table columns to
    other models/files in a Yggdrasil integration as arrays.
 */
class YggAsciiArrayOutput : public YggOutput {
public:

  /*!
    @brief Constructor for YggAsciiArrayOutput.
    @param[in] name Name of output channel. This should be named as a
      model output the in YAML for the model calling it.
    @param[in] fmt Format string specifying the datatype of messages
      that will be sent using this comm. If messages are sent to an
      ASCII table, fmt will also be used to format messages in the table.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggAsciiArrayOutput(const char *name, const char *fmt, int flags = 0,
		      const COMM_TYPE commtype = DEFAULT_COMM) :
    YggOutput(name, fmt, true, flags, commtype) {}

  /*!
    @brief Constructor for YggAsciiArrayOutput.
    @param[in] name Name of output channel. This should be named as a
      model output the in YAML for the model calling it.
    @param[in] fmt Format string specifying the datatype of messages
      that will be sent using this comm. If messages are sent to an
      ASCII table, fmt will also be used to format messages in the table.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggAsciiArrayOutput(const std::string name, const std::string fmt,
		      int flags = 0,
		      const COMM_TYPE commtype = DEFAULT_COMM) :
    YggOutput(name, fmt, true, flags, commtype) {}

};


/*!
  @brief Input communicator that can be used to receive table columns
    from other models/files in a Yggdrasil integration as arrays.
 */
class YggAsciiArrayInput : public YggInput {
public:

  /*!
    @brief Constructor for YggAsciiArrayInput.
    @param[in] name Name of output channel. This should be named as a
      model output the in YAML for the model calling it.
    @param[in] fmt Format string specifying the datatype of messages
      that will be received using this comm.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggAsciiArrayInput(const char *name, const char* fmt = nullptr,
		     int flags = 0,
		     const COMM_TYPE commtype = DEFAULT_COMM) :
    YggInput(name, fmt, true, flags, commtype) {}

  /*!
    @brief Constructor for YggAsciiArrayInput.
    @param[in] name Name of output channel. This should be named as a
      model output the in YAML for the model calling it.
    @param[in] fmt Format string specifying the datatype of messages
      that will be received using this comm.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggAsciiArrayInput(const std::string name, std::string fmt = "",
		     int flags = 0, const COMM_TYPE commtype = DEFAULT_COMM) :
    YggInput(name, fmt, true, flags, commtype) {}

};


/*!
  @brief Output communicator that can be used to send Ply objects to
    other models/files in a Yggdrasil integration.
 */
class YggPlyOutput : public YggOutput {
public:

  /*!
    @brief Constructor for YggPlyOutput.
    @param[in] name Name of output channel. This should be named as a
      model output the in YAML for the model calling it.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggPlyOutput(const char *name, int flags = 0,
	       const COMM_TYPE commtype = DEFAULT_COMM) :
    YggOutput(name, flags, commtype) {
    if (!this->addSchema("{\"type\": \"ply\"}"))
      this->throw_error("Invalid schema");
  }
  
  /*!
    @brief Constructor for YggPlyOutput.
    @param[in] name Name of output channel. This should be named as a
      model output the in YAML for the model calling it.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggPlyOutput(const std::string name, int flags = 0,
	       const COMM_TYPE commtype = DEFAULT_COMM) :
    YggOutput(name, flags, commtype) {
    if (!this->addSchema("{\"type\": \"ply\"}"))
      this->throw_error("Invalid schema");
  }

};


/*!
  @brief Input communicator that can be used to receive Ply objects from
    other models/files in a Yggdrasil integration.
 */
class YggPlyInput : public YggInput {
public:

  /*!
    @brief Constructor for YggPlyInput.
    @param[in] name Name of input channel. This should be named as a
      model input the in YAML for the model calling it.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggPlyInput(const char *name, int flags = 0,
	      const COMM_TYPE commtype = DEFAULT_COMM) :
    YggInput(name, flags, commtype) {
    if (!this->addSchema("{\"type\": \"ply\"}"))
      this->throw_error("Invalid schema");
  }

  /*!
    @brief Constructor for YggPlyInput.
    @param[in] name Name of input channel. This should be named as a
      model input the in YAML for the model calling it.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggPlyInput(const std::string name, int flags = 0,
	      const COMM_TYPE commtype = DEFAULT_COMM) :
    YggInput(name, flags, commtype) {
    if (!this->addSchema("{\"type\": \"ply\"}"))
      this->throw_error("Invalid schema");
  }
  
};


/*!
  @brief Output communicator that can be used to send ObjWavefront
    objects to other models/files in a Yggdrasil integration.
 */
class YggObjOutput : public YggOutput {
public:

  /*!
    @brief Constructor for YggObjOutput.
    @param[in] name Name of output channel. This should be named as a
      model output the in YAML for the model calling it.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggObjOutput(const char *name, int flags = 0,
	       const COMM_TYPE commtype = DEFAULT_COMM) :
    YggOutput(name, flags, commtype) {
    if (!this->addSchema("{\"type\": \"obj\"}"))
      this->throw_error("Invalid schema");
  }
  
  /*!
    @brief Constructor for YggObjOutput.
    @param[in] name Name of output channel. This should be named as a
      model output the in YAML for the model calling it.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggObjOutput(const std::string name, int flags = 0,
	       const COMM_TYPE commtype = DEFAULT_COMM) :
    YggOutput(name, flags, commtype) {
    if (!this->addSchema("{\"type\": \"obj\"}"))
      this->throw_error("Invalid schema");
  }
  
};


/*!
  @brief Input communicator that can be used to receive ObjWavefront
    objects from other models/files in a Yggdrasil integration.
 */
class YggObjInput : public YggInput {
public:

  /*!
    @brief Constructor for YggObjInput.
    @param[in] name Name of input channel. This should be named as a
      model input the in YAML for the model calling it.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggObjInput(const char *name, int flags = 0,
	      const COMM_TYPE commtype = DEFAULT_COMM) :
    YggInput(name, flags, commtype) {
    if (!this->addSchema("{\"type\": \"obj\"}"))
      this->throw_error("Invalid schema");
  }

  /*!
    @brief Constructor for YggObjInput.
    @param[in] name Name of input channel. This should be named as a
      model input the in YAML for the model calling it.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggObjInput(const std::string name, int flags = 0,
	      const COMM_TYPE commtype = DEFAULT_COMM) :
    YggInput(name, flags, commtype) {
    if (!this->addSchema("{\"type\": \"obj\"}"))
      this->throw_error("Invalid schema");
  }

};


/*!
  @brief Output communicator that can be used to send rapidjson::Document
    objects to other models/files in a Yggdrasil integration.
 */
class YggGenericOutput : public YggOutput {
public:

  /*!
    @brief Constructor for YggGenericOutput.
    @param[in] name Name of output channel. This should be named as a
      model output the in YAML for the model calling it.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggGenericOutput(const char *name, int flags = 0,
		   const COMM_TYPE commtype = DEFAULT_COMM) :
    YggOutput(name, flags, commtype) {
    if (!this->addSchema("{\"type\": \"any\"}"))
      this->throw_error("Invalid schema");
  }
  
  /*!
    @brief Constructor for YggGenericOutput.
    @param[in] name Name of output channel. This should be named as a
      model output the in YAML for the model calling it.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggGenericOutput(const std::string name, int flags = 0,
		   const COMM_TYPE commtype = DEFAULT_COMM) :
    YggOutput(name, flags, commtype) {
    if (!this->addSchema("{\"type\": \"any\"}"))
      this->throw_error("Invalid schema");
  }
  
};


/*!
  @brief Input communicator that can be used to receive
    rapidjson::Document objects from other models/files in a
    Yggdrasil integration.
 */
class YggGenericInput : public YggInput {
public:

  /*!
    @brief Constructor for YggGenericInput.
    @param[in] name Name of input channel. This should be named as a
      model input the in YAML for the model calling it.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggGenericInput(const char *name, int flags = 0,
		  const COMM_TYPE commtype = DEFAULT_COMM) :
    YggInput(name, flags, commtype) {
    if (!this->addSchema("{\"type\": \"any\"}"))
      this->throw_error("Invalid schema");
  }

  /*!
    @brief Constructor for YggGenericInput.
    @param[in] name Name of input channel. This should be named as a
      model input the in YAML for the model calling it.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggGenericInput(const std::string name, int flags = 0,
		  const COMM_TYPE commtype = DEFAULT_COMM) :
    YggInput(name, flags, commtype) {
    if (!this->addSchema("{\"type\": \"any\"}"))
      this->throw_error("Invalid schema");
  }

};


/*!
  @brief Output communicator that can be used to send rapidjson::Document
    objects to other models/files in a Yggdrasil integration.
 */
class YggAnyOutput : public YggOutput {
public:

  /*!
    @brief Constructor for YggAnyOutput.
    @param[in] name Name of output channel. This should be named as a
      model output the in YAML for the model calling it.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggAnyOutput(const char *name, int flags = 0,
	       const COMM_TYPE commtype = DEFAULT_COMM) :
    YggOutput(name, flags, commtype) {
    if (!this->addSchema("{\"type\": \"any\"}"))
      this->throw_error("Invalid schema");
  }
  
  /*!
    @brief Constructor for YggAnyOutput.
    @param[in] name Name of output channel. This should be named as a
      model output the in YAML for the model calling it.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggAnyOutput(const std::string name, int flags = 0,
	       const COMM_TYPE commtype = DEFAULT_COMM) :
    YggOutput(name, flags, commtype) {
    if (!this->addSchema("{\"type\": \"any\"}"))
      this->throw_error("Invalid schema");
  }
  
};


/*!
  @brief Input communicator that can be used to receive
    rapidjson::Document objects from other models/files in a
    Yggdrasil integration.
 */
class YggAnyInput : public YggInput {
public:

  /*!
    @brief Constructor for YggAnyInput.
    @param[in] name Name of input channel. This should be named as a
      model input the in YAML for the model calling it.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggAnyInput(const char *name, int flags = 0,
	      const COMM_TYPE commtype = DEFAULT_COMM) :
    YggInput(name, flags, commtype) {
    if (!this->addSchema("{\"type\": \"any\"}"))
      this->throw_error("Invalid schema");
  }

  /*!
    @brief Constructor for YggAnyInput.
    @param[in] name Name of input channel. This should be named as a
      model input the in YAML for the model calling it.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggAnyInput(const std::string name, int flags = 0,
	      const COMM_TYPE commtype = DEFAULT_COMM) :
    YggInput(name, flags, commtype) {
    if (!this->addSchema("{\"type\": \"any\"}"))
      this->throw_error("Invalid schema");
  }

};


/*!
  @brief Output communicator that can be used to send rapidjson::Document
    array type objects to other models/files in a Yggdrasil integration.
 */
class YggJSONArrayOutput : public YggOutput {
public:

  /*!
    @brief Constructor for YggJSONArrayOutput.
    @param[in] name Name of output channel. This should be named as a
      model output the in YAML for the model calling it.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggJSONArrayOutput(const char *name, int flags = 0,
		     const COMM_TYPE commtype = DEFAULT_COMM) :
    YggOutput(name, flags, commtype) {
    if (!this->addSchema("{\"type\": \"array\"}"))
      this->throw_error("Invalid schema");
  }
  
  /*!
    @brief Constructor for YggJSONArrayOutput.
    @param[in] name Name of output channel. This should be named as a
      model output the in YAML for the model calling it.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggJSONArrayOutput(const std::string name, int flags = 0,
		     const COMM_TYPE commtype = DEFAULT_COMM) :
    YggOutput(name, flags, commtype) {
    if (!this->addSchema("{\"type\": \"array\"}"))
      this->throw_error("Invalid schema");
  }
  
};


/*!
  @brief Input communicator that can be used to receive
    rapidjson::Document array type objects from other models/files in a
    Yggdrasil integration.
 */
class YggJSONArrayInput : public YggInput {
public:

  /*!
    @brief Constructor for YggJSONArrayInput.
    @param[in] name Name of input channel. This should be named as a
      model input the in YAML for the model calling it.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggJSONArrayInput(const char *name, int flags = 0,
		    const COMM_TYPE commtype = DEFAULT_COMM) :
    YggInput(name, flags, commtype) {
    if (!this->addSchema("{\"type\": \"array\"}"))
      this->throw_error("Invalid schema");
  }

  /*!
    @brief Constructor for YggJSONArrayInput.
    @param[in] name constant std::string the name of an input channel.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggJSONArrayInput(const std::string name, int flags = 0,
		    const COMM_TYPE commtype = DEFAULT_COMM) :
    YggInput(name, flags, commtype) {
    if (!this->addSchema("{\"type\": \"array\"}"))
      this->throw_error("Invalid schema");
  }

};


/*!
  @brief Output communicator that can be used to send rapidjson::Document
    object type objects to other models/files in a Yggdrasil integration.
 */
class YggJSONObjectOutput : public YggOutput {
public:

  /*!
    @brief Constructor for YggJSONObjectOutput.
    @param[in] name Name of output channel. This should be named as a
      model output the in YAML for the model calling it.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggJSONObjectOutput(const char *name, int flags = 0,
		      const COMM_TYPE commtype = DEFAULT_COMM) :
    YggOutput(name, flags, commtype) {
    if (!this->addSchema("{\"type\": \"object\"}"))
      this->throw_error("Invalid schema");
  }
  
  /*!
    @brief Constructor for YggJSONObjectOutput.
    @param[in] name Name of output channel. This should be named as a
      model output the in YAML for the model calling it.
   */
  YggJSONObjectOutput(const std::string name, int flags = 0,
		      const COMM_TYPE commtype = DEFAULT_COMM) :
    YggOutput(name, flags, commtype) {
    if (!this->addSchema("{\"type\": \"object\"}"))
      this->throw_error("Invalid schema");
  }
  
};


/*!
  @brief Input communicator that can be used to receive
    rapidjson::Document object type objects from other models/files in a
    Yggdrasil integration.
 */
class YggJSONObjectInput : public YggInput {
public:

  /*!
    @brief Constructor for YggJSONObjectInput.
    @param[in] name Name of input channel. This should be named as a
      model input the in YAML for the model calling it.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggJSONObjectInput(const char *name, int flags = 0,
		     const COMM_TYPE commtype = DEFAULT_COMM) :
    YggInput(name, flags, commtype) {
    if (!this->addSchema("{\"type\": \"object\"}"))
      this->throw_error("Invalid schema");
  }

  /*!
    @brief Constructor for YggJSONObjectInput.
    @param[in] name Name of input channel. This should be named as a
      model input the in YAML for the model calling it.
    @param[in] flags Bit flags to set communicator properties.
    @param[in] commtype Type of communicator that should be used. Defaults
      to DEFAULT_COMM that is set based on the available packages at
      compilation.
   */
  YggJSONObjectInput(const std::string name, int flags = 0,
		     const COMM_TYPE commtype = DEFAULT_COMM) :
    YggInput(name, flags, commtype) {
    if (!this->addSchema("{\"type\": \"object\"}"))
      this->throw_error("Invalid schema");
  }
  
};
