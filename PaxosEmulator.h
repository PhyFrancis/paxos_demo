#ifndef __PAXOS_EMULATOR__
#define __PAXOS_EMULATOR__

#include <vector>
#include <unordered_map>
#include <thread>
#include <chrono>

#include "Node.h"
#include "Message.h"
#include "SequenceNumberProvider.h"

#define AVG_TIME_LAG 2

class PaxosEmulator {
 private:
  long currentTime;

  std::vector<Node> nodes;
  int nodeCount;
  std::unordered_multimap<long, Message> messages;

  SequenceNumberProvider seqProvider;

  long getCurrentTime() {
    return currentTime;
  }

  void maybePropose(Node &node) {
    if (node.shouldPropose()) {
      for (Node &toNode : this->nodes) {
        Message message(node.getId(), toNode.getId(), seqProvider.get(), PREPARE, node.toString());
        sendMessage(sometimeInFuture(), message);
      }
      node.resetTimeSinceLastPropose();
    }
  }

  bool runLoopEnds() {
    for (const Node &node : this->nodes) {
      if (node.isInPaxos()) {
        return false;
      }
    }
    return messages.size() == 0;
  }

  void tickClock() {
    currentTime++;
  }

  void runLoop() {
    if (runLoopEnds()) return;
    std::cout << "=== Current time: " << getCurrentTime() << " ===" << std::endl;

    std::unordered_multimap<long, Message>::iterator itr;
    while ((itr = messages.find(currentTime)) != messages.end()) {
      receiveMessage(itr->second);
      messages.erase(itr);
    }

    for (Node &node : this->nodes) {
      node.tickClock();
      maybePropose(node);
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    tickClock();
    runLoop();
  }

  void receiveMessage(const Message &message) {
    std::cout<<"[Received]\t"<< message.toString() << std::endl;
    Node &node = nodes[message.toId];
    switch (message.type) {
      case PREPARE:
        if (node.maybePromise(message.sequenceNumber)) {
          Message reply(message.toId, message.fromId, node.getPromisedSeq(), PROMISE, "");
          sendMessage(sometimeInFuture(), reply);
        }
        break;
    }
  }

  void sendMessage(long time, Message message) {
    messages.insert({time, message});
    std::cout << "[Sent]\t\t" << message.toString() << std::endl;
  }

  long sometimeInFuture() {
    return currentTime + 1 + (rand() % 100) * AVG_TIME_LAG / 50;
  }

 public:
  void init(int numNodes) {
    currentTime = 0;
    for (int i = 0; i < numNodes; i++) {
      nodes.push_back(Node(i, seqProvider));
    }
    nodeCount = nodes.size();
    std::cout<< "Paxos emulator initialized, total number of agents = "
             << nodeCount << std::endl;
  }

  void startPaxos() {
    for (Node &node : nodes) {
      node.setState(IN_PAXOS);
    }
    runLoop();
  }

  ~PaxosEmulator() {
    nodes.clear();
    messages.clear();
  }
};

#endif
