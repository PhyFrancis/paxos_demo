#ifndef __NODE__
#define __NODE__

#include <iostream>
#include <string>
#include "SequenceNumberProvider.h"

enum NodeState {
  IDLE,
  IN_PAXOS,
};

// TODO add node role (proposer, acceptor, learner)

class Node {

 private:
  const int id;
  NodeState state;
  long currentTime;
  long lastActiveTime;
  SequenceNumberProvider &seqProvider;

 public:
  Node(int _id, SequenceNumberProvider &_seqProvider)
      : id(_id), state(IDLE), currentTime(0), lastActiveTime(0), seqProvider(_seqProvider) {
    std::cout << "Node " << toString() << " is created" << std::endl;
  }

  void startPaxos() {
    state = IN_PAXOS;
  }

  bool isInPaxos() const {
    return state == IN_PAXOS;
  }

  void tickClock() {
    currentTime++;
  }

  NodeState getState();

  std::string toString() const {
    return std::to_string(this->id);
  };
};

#endif
