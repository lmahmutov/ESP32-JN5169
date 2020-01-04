String u64toStr(uint64_t input)
{
  String result = "";//
  uint8_t base = 16; //hex 10 dec

  do {
    char c = input % base; input /= base;
    if (c < 10) {
      c += '0';
    } else {
      c += 'A' - 10;
    }
    result = c + result;
  } while (input);
  switch (result.length()) {
    case 1: {
        result = "000000000000000" + result;
      } break;
    case 2: {
        result = "00000000000000" + result;
      } break;
    case 3: {
        result = "0000000000000" + result;
      } break;
    case 4: {
        result = "000000000000" + result;
      } break;
    case 5: {
        result = "00000000000" + result;
      } break;
    case 6: {
        result = "0000000000" + result;
      } break;
    case 7: {
        result = "000000000" + result;
      } break;
    case 8: {
        result = "00000000" + result;
      } break;
    case 9: {
        result = "0000000" + result;
      } break;
    case 10: {
        result = "000000" + result;
      } break;
    case 11: {
        result = "00000" + result;
      } break;
    case 12: {
        result = "0000" + result;
      } break;
    case 13: {
        result = "000" + result;
      } break;
    case 14: {
        result = "00" + result;
      } break;
    case 15: {
        result = "0" + result;
      } break;
  }
  return result;
}

String u16toStr(uint16_t u16Input)
{
  char tmp[16];
  sprintf(tmp, "0x%.4X", u16Input);
  return tmp;
}


String u8toStr(uint8_t u8Input)
{
  char tmp[8];
  sprintf(tmp, "0x%.2X", u8Input);
  return tmp;
}
