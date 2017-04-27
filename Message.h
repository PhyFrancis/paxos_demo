#ifndef __MESSAGE__
#define __MESSAGE__

#include "Node.h"

struct Message {
  const Node &from;
  const Node &to;
};

#endif
