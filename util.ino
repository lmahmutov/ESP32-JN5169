String Hex( uint16_t ) {
  String hex_str[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"}; String z;
  z = hex_str[(bin >> 4) & 0x0F] + hex_str[(bin) & 0x0F]; return z;
}
