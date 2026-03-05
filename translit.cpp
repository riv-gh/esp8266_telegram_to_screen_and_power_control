#include "translit.h"

String translit(const String &src) {
  String out = "";
  for (int i = 0; i < src.length(); i++) {
    unsigned char c = src[i];

    if (c == 0xD0 || c == 0xD1) {
      unsigned char c2 = src[i + 1];
      i++;

      uint16_t code = (c << 8) | c2;

      switch (code) {
        case 0xD090: out += "A"; break;
        case 0xD091: out += "B"; break;
        case 0xD092: out += "V"; break;
        case 0xD093: out += "G"; break;
        case 0xD094: out += "D"; break;
        case 0xD095: out += "E"; break;
        case 0xD081: out += "Yo"; break;
        case 0xD096: out += "Zh"; break;
        case 0xD097: out += "Z"; break;
        case 0xD098: out += "I"; break;
        case 0xD099: out += "Y"; break;
        case 0xD09A: out += "K"; break;
        case 0xD09B: out += "L"; break;
        case 0xD09C: out += "M"; break;
        case 0xD09D: out += "N"; break;
        case 0xD09E: out += "O"; break;
        case 0xD09F: out += "P"; break;
        case 0xD0A0: out += "R"; break;
        case 0xD0A1: out += "S"; break;
        case 0xD0A2: out += "T"; break;
        case 0xD0A3: out += "U"; break;
        case 0xD0A4: out += "F"; break;
        case 0xD0A5: out += "Kh"; break;
        case 0xD0A6: out += "Ts"; break;
        case 0xD0A7: out += "Ch"; break;
        case 0xD0A8: out += "Sh"; break;
        case 0xD0A9: out += "Sch"; break;
        case 0xD0AA: out += ""; break;
        case 0xD0AB: out += "Y"; break;
        case 0xD0AC: out += ""; break;
        case 0xD0AD: out += "E"; break;
        case 0xD0AE: out += "Yu"; break;
        case 0xD0AF: out += "Ya"; break;

        case 0xD0B0: out += "a"; break;
        case 0xD0B1: out += "b"; break;
        case 0xD0B2: out += "v"; break;
        case 0xD0B3: out += "g"; break;
        case 0xD0B4: out += "d"; break;
        case 0xD0B5: out += "e"; break;
        case 0xD191: out += "yo"; break;
        case 0xD0B6: out += "zh"; break;
        case 0xD0B7: out += "z"; break;
        case 0xD0B8: out += "i"; break;
        case 0xD0B9: out += "y"; break;
        case 0xD0BA: out += "k"; break;
        case 0xD0BB: out += "l"; break;
        case 0xD0BC: out += "m"; break;
        case 0xD0BD: out += "n"; break;
        case 0xD0BE: out += "o"; break;
        case 0xD0BF: out += "p"; break;
        case 0xD180: out += "r"; break;
        case 0xD181: out += "s"; break;
        case 0xD182: out += "t"; break;
        case 0xD183: out += "u"; break;
        case 0xD184: out += "f"; break;
        case 0xD185: out += "kh"; break;
        case 0xD186: out += "ts"; break;
        case 0xD187: out += "ch"; break;
        case 0xD188: out += "sh"; break;
        case 0xD189: out += "sch"; break;
        case 0xD18A: out += ""; break;
        case 0xD18B: out += "y"; break;
        case 0xD18C: out += ""; break;
        case 0xD18D: out += "e"; break;
        case 0xD18E: out += "yu"; break;
        case 0xD18F: out += "ya"; break;

        default:
          out += "?";
      }
    } else {
      out += (char)c;
    }
  }
  return out;
}