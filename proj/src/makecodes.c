#include "makecodes.h"
#include <stdint.h>
#include <stdbool.h>


char make_to_char(uint32_t makecode) {
  switch (makecode) {
  case am:
    return 'a';
  case bm:
    return 'b';
  case cm:
    return 'c';
  case dm:
    return 'd';
  case em:
    return 'e';
  case fm:
    return 'f';
  case gm:
    return 'g';
  case hm:
    return 'h';
  case im:
    return 'i';
  case jm:
    return 'j';
  case km:
    return 'k';
  case lm:
    return 'l';
  case mm:
    return 'm';
  case nm:
    return 'n';
  case om:
    return 'o';
  case pm:
    return 'p';
  case qm:
    return 'q';
  case rm:
    return 'r';
  case sm:
    return 's';
  case tm:
    return 't';
  case um:
    return 'u';
  case vm:
    return 'v';
  case wm:
    return 'w';
  case xm:
    return 'x';
  case ym:
    return 'y';
  case zm:
    return 'z';
  }
  return -1;
}

int char_to_pont(char letra)
{
    switch (letra) {
  case 'a':
    return apont;
  case 'b':
    return bpont;
  case 'c':
    return cpont;
  case 'd':
    return dpont;
  case 'e':
    return epont;
  case 'f':
    return fpont;
  case 'g':
    return gpont;
  case 'h':
    return hpont;
  case 'i':
    return ipont;
  case 'j':
    return jpont;
  case 'k':
    return kpont;
  case 'l':
    return lpont;
  case 'm':
    return mpont;
  case 'n':
    return npont;
  case 'o':
    return opont;
  case 'p':
    return ppont;
  case 'q':
    return qpont;
  case 'r':
    return rpont;
  case 's':
    return spont;
  case 't':
    return tpont;
  case 'u':
    return upont;
  case 'v':
    return vpont;
  case 'w':
    return wpont;
  case 'x':
    return xpont;
  case 'y':
    return ypont;
  case 'z':
    return zpont;
  }
  return 0;
}

