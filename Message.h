#ifndef __MESSAGE__
#define __MESSAGE__

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
  int fromId;
  int toId;
  long sequenceNumber;
  MessageType type;
  std::string value;

  Message(int _fromId,
          int  _toId,
          long _seq,
          MessageType _type,
          std::string _value) {
    this->fromId = _fromId;
    this->toId = _toId;
    this->sequenceNumber = _seq;
    this->type = _type;
    this->value = _value;
  }

  /**
   * @return ${type}:::${seq}:::${value}
   */
  std::string toString() const {
    std::ostringstream info;
    info << "Node " << fromId << " --> Node " << toId << ": "
         << MessageTypeString[type] << ":::" << sequenceNumber << ":::" << value;
    return info.str();
  }
};

#endif
