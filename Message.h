#ifndef __MESSAGE__
#define __MESSAGE__

#include "Node.h"
#include <string>
#include <sstream>

enum MessageType {
  PROPOSE,
  PROMISE,
  ACCEPT,
  ACCEPTED,
};

static const std::string MessageTypeStrings[] = {
  "PROPOSE",
  "PROMISE",
  "ACCEPT",
  "ACCEPTED",
};

struct Message {
  const Node &from;
  const Node &to;
  const long sequenceNumber;
  const MessageType type;
  const std::string value;

  std::string messageSentLog() {
    std::ostringstream logStream;
    logStream << "Sent a " << toString() << " messgae to Node " << to.toString();
    return logStream.str();
  }

  /**
   * @return ${type}:::${seq}:::${value}
   */
  std::string toString() {
    std::string ret;
    ret += MessageTypeStrings[type];
    ret += ":::";
    ret += sequenceNumber;
    ret += ":::";
    ret += value;
    return ret;
  }
};

#endif
