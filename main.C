#include <iostream>
#include "PaxosEmulator.h"

using namespace std;

int main() {
  PaxosEmulator emulator;
  emulator.init(1);
  emulator.startPaxos();
}
