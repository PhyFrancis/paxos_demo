#include <iostream>
#include "PaxosEmulator.h"
#include <time.h>

using namespace std;

int main() {
  srand(time(NULL));
  PaxosEmulator emulator;
  emulator.init(1);
  emulator.startPaxos();
}
