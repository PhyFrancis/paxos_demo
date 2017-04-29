#ifndef __NODE__
#define __NODE__

#include "SequenceNumberProvider.h"
#include "Message.h"

#include <iostream>
#include <string>
#include <stdlib.h>

#define PROPOSE_PROBABILITY 0.2

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

  int getId() {
    return this->id;
  }

  void setState(NodeState newState) {
    this->state = newState;
  }

  void resetTimeSinceLastPropose() {
    this->timeSinceLastPropose = 0;
  }

  void tickClock() {
    this->timeSinceLastPropose += 1;
  }

  bool isInPaxos() const {
    return state == IN_PAXOS;
  }

  bool shouldPropose() {
    return isInPaxos() &&
        PROPOSE_PROBABILITY * timeSinceLastPropose > (rand() % 100) / 100.0;
  }

  void processMessage() {
  }

  std::string toString() const {
    return std::to_string(this->id);
  };
};

#endif
