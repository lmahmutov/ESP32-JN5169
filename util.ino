String u64toStr(uint64_t u64Input)
{
    unsigned long long1 = (unsigned long)((u64Input & 0xFFFF0000) >> 16 );
    unsigned long long2 = (unsigned long)((u64Input & 0x0000FFFF));
    String u64Strhex ="0x" + String(long1, HEX) + String(long2, HEX);
    return u64Strhex;
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
