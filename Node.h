#ifndef __NODE__
#define __NODE__

#include <iostream>
#include <string>

enum NodeState {
  IDLE,
  IN_PAXOS,
};

// TODO add node role (proposer, acceptor, learner)

class Node {

 private:
  const int id;
  NodeState state;

 public:
  Node(int _id) : id(_id), state(IDLE) {
    std::cout << "Node " << toString() << " is created" << std::endl;
  }

  void startPaxos() {
    state = IN_PAXOS;
  }

  bool isInPaxos() const {
    return state == IN_PAXOS;
  }

  NodeState getState();

  std::string toString() const {
    return std::to_string(this->id);
  };
};

#endif
