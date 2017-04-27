#ifndef __NODE__
#define __NODE__

enum NodeState {
};

// TODO add node role (proposer, acceptor, learner)

class Node {
 private:
  NodeState state;
 public:
  NodeState getState();
};

#endif
