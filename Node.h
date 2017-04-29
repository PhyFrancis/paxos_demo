#ifndef __NODE__
#define __NODE__

#include "SequenceNumberProvider.h"
#include "Message.h"

#include <assert.h>
#include <iostream>
#include <utility>
#include <string>
#include <stdlib.h>
#include <unordered_map>
#include <map>

#define PROPOSE_PROBABILITY 0.2

enum NodeState {
  IDLE,
  IN_PAXOS,
};

// TODO
// - [ ] add node role (proposer, acceptor, learner).
// - [ ] don't propose if there is any activity from outside.

class Node {

 private:
  const int id;
  NodeState state;
  SequenceNumberProvider &seqProvider;
  long timeSinceLastPropose;
  int highestPromisedSeq;

  // seqNum -> acceptedSeqNum-> acceptedValue
  std::unordered_map<int, std::map<int, std::string> > receivedPromises;

  // <seqNum, value>
  std::pair<int, std::string> latestAccepted;

 public:
  Node(int _id, SequenceNumberProvider &_seqProvider) :
    id(_id), seqProvider(_seqProvider) {
      state = IDLE;
      timeSinceLastPropose = 0;
      highestPromisedSeq = -1;
      latestAccepted = std::make_pair(-1, "");
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

  /**
   * @param message: a prepare message.
   */
  bool shouldPromise(const Message &message) {
    return highestPromisedSeq <  message.sequenceNumber;
  }

  /**
   * @param message: a prepare message.
   */
  Message promise(const Message &message) {
    assert(this->id == message.toId);
    return Message::promiseMessage(
        this->id,
        message.fromId,
        message.sequenceNumber,
        this->latestAccepted.first,
        this->latestAccepted.second);
  }

  int getPromisedSeq() {
    return highestPromisedSeq;
  }


  /**
   * @return <# of received promises, latest accepted value>
   */
  std::pair<int, std::string> receivePromise(const Message &promise) {
    int seqNum = promise.sequenceNumber;
    if (receivedPromises.find(seqNum) == receivedPromises.end()) {
      receivedPromises[seqNum] = std::map<int, std::string>();
    }
    receivedPromises[seqNum][promise.acceptedSeqNum] = promise.acceptedValue;
    return std::make_pair(receivedPromises[seqNum].size(), receivedPromises[seqNum].cbegin()->second);
  }

  std::string toString() const {
    return std::to_string(this->id);
  };
};

#endif
