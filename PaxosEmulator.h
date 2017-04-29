#ifndef __PAXOS_EMULATOR__
#define __PAXOS_EMULATOR__

#include <vector>
#include <unordered_map>
#include <thread>
#include <chrono>

#include "Node.h"
#include "Message.h"
#include "SequenceNumberProvider.h"

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

  bool runLoopEnds() {
    for (const Node& node : nodes) {
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
