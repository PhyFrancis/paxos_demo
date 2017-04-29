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
  int acceptedSeqNum;
  std::string acceptedValue;

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

  // For promise type
  static Message promiseMessage(int _fromId,
          int  _toId,
          long _seq,
          int _acceptedSeqNum,
          std::string _acceptedValue) {
    Message ret(_fromId, _toId, _seq, PROMISE, "");
    ret.acceptedSeqNum = _acceptedSeqNum;
    ret.acceptedValue = _acceptedValue;
    return ret;
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
