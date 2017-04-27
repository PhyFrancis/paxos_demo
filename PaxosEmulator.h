#ifndef __PAXOS_EMULATOR__
#define __PAXOS_EMULATOR__

#include <vector>

#include "Node.h"
#include "Message.h"

class PaxosEmulator {
 private:
  std::vector<Node> nodes;
  std::vector<Message> messages;
};

#endif
