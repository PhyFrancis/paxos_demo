#ifndef __NODE__
#define __NODE__

#include "PaxosEmulator.h"
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
  long currentTime;
  long lastActiveTime;

  SequenceNumberProvider &seqProvider;
  PaxosEmulator &emulator;

 public:
  Node(int _id, SequenceNumberProvider &_seqProvider, PaxosEmulator &_emulator) :
    id(_id), state(IDLE), currentTime(0), lastActiveTime(0),
    seqProvider(_seqProvider), emulator(_emulator) {
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
