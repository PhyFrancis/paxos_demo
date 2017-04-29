#ifndef __SEQUENCE_NUMBER_PROVIDER__
#define __SEQUENCE_NUMBER_PROVIDER__

/**
 * This is not thread safe.
 */
class SequenceNumberProvider {

 private:
  long counter;

 public:

  SequenceNumberProvider() : counter(0) {
  }

  long get() {
    return counter++;
  }
};

#endif
