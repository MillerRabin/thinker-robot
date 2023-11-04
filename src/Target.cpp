#include "Target.h"

//-----------Range----------

const float Range::middle() {
  return from + (to - from) / 2;
};