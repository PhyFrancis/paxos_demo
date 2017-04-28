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
  std::vector<Node> nodes;
  std::unordered_multimap<long, Message> messages;
  long currentTime;
  int nodeCount;

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

  void runLoop() {
    if (runLoopEnds()) return;
    std::cout << "Current time: " << getCurrentTime() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    currentTime++;
    runLoop();
  }

 public:
  void init(int numNodes) {
    currentTime = 0;
    for (int i = 0; i < numNodes; i++) {
      nodes.push_back(Node(i));
    }
    nodeCount = nodes.size();
  }

  void startPaxos() {
    for (Node &node : nodes) {
      node.startPaxos();
    }
    runLoop();
  }

  ~PaxosEmulator() {
    nodes.clear();
    messages.clear();
  }
};

#endif
