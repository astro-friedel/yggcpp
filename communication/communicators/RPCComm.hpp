#pragma once

#include "DefaultComm.hpp"
#include "CommBase.hpp"
#include "Requests.hpp"

#ifdef COMM_BASE
namespace communication {
namespace communicator {

// @brief Structure for storing requests
class RPCComm : public COMM_BASE {
public:
  explicit RPCComm(const std::string &name, utils::Address *address,
		   int flgs, DIRECTION dir, DIRECTION req_dir);
  explicit RPCComm(const std::string name, int flgs,
		   DIRECTION dir, DIRECTION req_dir);

  using Comm_t::send;
  using Comm_t::recv;
  using COMM_BASE::comm_nmsg;
  
  void addResponseSchema(const std::string& s);
  void addResponseSchema(const rapidjson::Value& s);
  void addResponseFormat(const std::string& fmt);
  
#ifndef YGG_TEST
protected:
#endif
  
  Metadata& get_metadata(const DIRECTION dir) override;
  RequestList requests;
};
  
}
} // communication

#endif
