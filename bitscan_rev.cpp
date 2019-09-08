/**
 * bitScanReverse
 * @author Eugene Nalimov
 * @param bb bitboard to scan
 * @return index (0..63) of most significant one bit
 */
int bitScanReverse(U64 bb)
{
   int result = 0;
   if (bb > 0xFFFFFFFF) {
      bb >>= 32;
      result = 32;
   }
   if (bb > 0xFFFF) {
      bb >>= 16;
      result += 16;
   }
   if (bb > 0xFF) {
      bb >>= 8;
      result += 8;
   }
   return result + ms1bTable[bb];
}
