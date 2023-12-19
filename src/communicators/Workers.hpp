#pragma once

#include <vector>
#include "utils/enums.hpp"
#include "utils/Address.hpp"
#include "utils/logging.hpp"

namespace YggInterface {
namespace communicator {
  
  // Forward declare
  class Comm_t;

  class Worker : public YggInterface::utils::LogBase {
  private:
    Worker(const Worker& rhs) = delete;
    Worker& operator=(const Worker& rhs) = delete;
  public:
    Worker(Comm_t* parent, DIRECTION dir, utils::Address& adr);
    Worker(Worker&& rhs);
    Worker& operator=(Worker&& rhs);
    bool matches(DIRECTION dir, utils::Address& adr);
    ~Worker();
    std::string logClass() const override { return "Worker"; }
    std::string logInst() const override;
    Comm_t* comm;
    std::string request;
  };
  class WorkerList : public YggInterface::utils::LogBase {
  private:
    WorkerList(const WorkerList& rhs) = delete;
    WorkerList& operator=(const WorkerList& rhs) = delete;
  public:
    WorkerList() : workers() {}
    ~WorkerList();
    Comm_t* add_worker(Comm_t* parent, DIRECTION dir,
		       utils::Address& adr);
    void remove_worker(Comm_t*& worker);
    Comm_t* find_worker(DIRECTION dir, utils::Address& adr,
			size_t* idx = nullptr);
    int find_worker(Comm_t* worker);
    Comm_t* get(Comm_t* parent, DIRECTION dir,
		utils::Address& adr);
    Comm_t* get(Comm_t* parent, DIRECTION dir);
    Comm_t* get(Comm_t* parent, DIRECTION dir, std::string adr);
    bool setRequest(Comm_t* worker, std::string request);
    bool setResponse(std::string request);
    std::string logClass() const override { return "WorkerList"; }
    std::vector<Worker> workers;
  };
}
}