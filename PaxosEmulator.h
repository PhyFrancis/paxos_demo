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
      for (Node &toNode : nodes) {
        long arriveTime = currentTime + (rand() % 100) * AVG_TIME_LAG / 50;
        Message message(node.getId(), toNode.getId(), true, seqProvider.get(), PREPARE, node.toString());
        messages.insert({arriveTime, message});
      }
      node.resetTimeSinceLastPropose();
    }
  }

  bool runLoopEnds() {
    for (const Node &node : nodes) {
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

    for (Node &node : nodes) {
      node.tickClock();
      maybePropose(node);
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    tickClock();
    runLoop();
  }

 public:
  void init(int numNodes) {
    currentTime = 0;
    for (int i = 0; i < numNodes; i++) {
      nodes.push_back(Node(i, seqProvider));
    }
    nodeCount = nodes.size();
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
