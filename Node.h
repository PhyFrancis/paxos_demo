#ifndef __NODE__
#define __NODE__

#include "SequenceNumberProvider.h"

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
  long timeSinceLastPropose;
  SequenceNumberProvider &seqProvider;

 public:
  Node(int _id, SequenceNumberProvider &_seqProvider) :
    id(_id), seqProvider(_seqProvider) {
      state = IDLE;
      timeSinceLastPropose = 0;
      std::cout << "Node " << toString() << " is created" << std::endl;
  }

  NodeState getState();

  void setState(NodeState newState) {
    state = newState;
  }

  bool isInPaxos() const {
    return state == IN_PAXOS;
  }

  void processMessage() {
  }

  std::string toString() const {
    return std::to_string(this->id);
  };
};

#endif
