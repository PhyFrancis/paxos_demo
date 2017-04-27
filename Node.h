#ifndef __NODE__
#define __NODE__

#include <string>

enum NodeState {
};

// TODO add node role (proposer, acceptor, learner)

class Node {
 private:
  const int id;
  NodeState state;
 public:
  NodeState getState();

  std::string toString() const {
    return std::to_string(this->id);
  };
};

#endif
