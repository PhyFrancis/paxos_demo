#ifndef __NODE__
#define __NODE__

#include "Message.h"

#include <assert.h>
#include <iostream>
#include <utility>
#include <string>
#include <stdlib.h>
#include <unordered_map>
#include <map>

#define PROPOSE_PROBABILITY 0.01

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
  long timeSinceLastPropose;
  int highestPromisedSeq;

  // seqNum -> acceptedSeqNum-> acceptedValue
  std::unordered_map<int, std::multimap<int, std::string> > receivedPromises;

  // <seqNum, value>
  std::pair<long, std::string> latestAccepted;

  // seqNum -> nodeId -> accepted value
  std::unordered_map<int, std::unordered_map<int, std::string> > receivedAccepted;

  std::string consensusValue;

 public:
  Node(int _id) : id(_id) {
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

  Message receivePrepare(const Message &prepare) {
    assert(this->id == prepare.toId);
    this->highestPromisedSeq = prepare.sequenceNumber;
    return Message::promiseMessage(
        this->id,
        prepare.fromId,
        prepare.sequenceNumber,
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
      receivedPromises[seqNum] = std::multimap<int, std::string>();
    }
    receivedPromises[seqNum].insert(std::make_pair(promise.acceptedSeqNum, promise.acceptedValue));
    return std::make_pair(receivedPromises[seqNum].size(), receivedPromises[seqNum].cbegin()->second);
  }

  bool shouldAccept(const Message &accept) {
    return accept.sequenceNumber >= this->highestPromisedSeq;
  }

  Message receiveAccept(const Message &accept) {
    assert(accept.sequenceNumber >= this->highestPromisedSeq);
    assert(accept.toId == this->id);
    this->latestAccepted = std::make_pair(accept.sequenceNumber, accept.value);
    return Message::acceptedMessage(this->id, accept.fromId, accept.sequenceNumber, accept.value);
  }

  /**
   * @return <# of received accpted message, value>
   */
  std::pair<int, std::string> receiveAccepted(const Message &message) {
    long seqNum = message.sequenceNumber;
    if (receivedAccepted.find(seqNum) == receivedAccepted.end()) {
      receivedAccepted[seqNum] = std::unordered_map<int, std::string>();
    }

    if (receivedAccepted[seqNum].size() > 0) {
      assert(receivedAccepted[seqNum].begin()->second == message.value);
    }

    receivedAccepted[seqNum][message.fromId] = message.value;
    return std::make_pair(receivedAccepted[seqNum].size(), message.value);
  }

  std::string toString() const {
    return std::to_string(this->id);
  };
};

#endif
