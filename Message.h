#ifndef __MESSAGE__
#define __MESSAGE__

#include "Node.h"
#include <string>
#include <sstream>

enum MessageType {
  PREPARE,
  PROMISE,
  ACCEPT,
  ACCEPTED,
};

static const std::string MessageTypeString[] = {
  "PREPARE",
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
    logStream << "Node" << from.toString() << " sent a " << toString()
              << " messgae to Node " << to.toString();
    return logStream.str();
  }

  std::string messageReceivedLog() {
    std::ostringstream logStream;
    logStream << "Node" << to.toString() << " received a " << toString()
              << " messgae from Node " << from.toString();
    return logStream.str();
  }

  /**
   * @return ${type}:::${seq}:::${value}
   */
  std::string toString() {
    std::ostringstream info;
    info << MessageTypeString[type] << ":::" << sequenceNumber << ":::" << value;
    return info.str();
  }
};

#endif
