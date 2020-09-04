#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

uint32_t refin(uint32_t x)
{
  x = ((x & 0x55555555) << 1) | ((x & 0xAAAAAAAA) >> 1);
  x = ((x & 0x33333333) << 2) | ((x & 0xCCCCCCCC) >> 2);
  x = ((x & 0x0F0F0F0F) << 4) | ((x & 0xF0F0F0F0) >> 4);
  return x;
}

uint32_t refout(uint32_t x)
{
  x = ((x & 0x55555555) << 1) | ((x & 0xAAAAAAAA) >> 1);
  x = ((x & 0x33333333) << 2) | ((x & 0xCCCCCCCC) >> 2);
  x = ((x & 0x0F0F0F0F) << 4) | ((x & 0xF0F0F0F0) >> 4);
  x = ((x & 0x00FF00FF) << 8) | ((x & 0xFF00FF00) >> 8);
  x = ((x & 0x0000FFFF) << 16) | ((x & 0xFFFF0000) >> 16);
  return x;
}

uint32_t calc(uint8_t val, uint32_t poly)
{
  uint32_t ret = val << 24;
  for(int j = 0; j < 8; ++j ) 
  {
    ret = (ret & 0x80000000) ? ((ret << 1) ^ poly) : (ret << 1);
  }
  return ret;
}

uint32_t CRC32_Calc(const uint8_t *val, int32_t len, uint32_t init, uint32_t poly, bool isRefIn, bool isRefOut, uint32_t xor)
{
  if(val == NULL)
    return init;
  uint32_t ret = 0;
  uint32_t crc = init;
  for(int32_t i = 0; i < len; i++)
  {
    uint32_t tmp = (isRefIn?refin(val[i]):val[i]);
    crc = (crc << 8) ^ calc(((crc >> 24) ^ tmp) & 0xFF, poly);
  }
  ret = isRefOut?refout(crc):crc;
  ret ^= xor;
  return ret;
}



int main(int argc, char** argv)
{
  const char* str = "123456789";
  printf("CRC32 of\t%s =\t%#x\n", str, CRC32_Calc(str,9,0xFFFFFFFF,0x04C11DB7,true,true,0xFFFFFFFF));
  printf("CRC32/BZIP2 of\t%s =\t%#x\n", str, CRC32_Calc(str,9,0xFFFFFFFF,0x04C11DB7,false,false,0xFFFFFFFF));
  printf("CRC32C of\t%s =\t%#x\n", str, CRC32_Calc(str,9,0xFFFFFFFF,0x1EDC6F41,true,true,0xFFFFFFFF));
  printf("CRC32D of\t%s =\t%#x\n", str, CRC32_Calc(str,9,0xFFFFFFFF,0xA833982B,true,true,0xFFFFFFFF));
  printf("CRC32/MPEG2 of\t%s =\t%#x\n", str, CRC32_Calc(str,9,0xFFFFFFFF,0x04C11DB7,false,false,0x0));
  printf("CRC32/POSIX of\t%s =\t%#x\n", str, CRC32_Calc(str,9,0x00000000,0x04C11DB7,false,false,0xFFFFFFFF));
  printf("CRC32Q of\t%s =\t%#x\n", str, CRC32_Calc(str,9,0x00000000,0x814141AB,false,false,0x0));
  printf("CRC32/JAMCRC of\t%s =\t%#x\n", str, CRC32_Calc(str,9,0xFFFFFFFF,0x04C11DB7,true,true,0x0));
  printf("CRC32/XFER of\t%s =\t%#x\n", str, CRC32_Calc(str,9,0x00000000,0x000000AF,false,false,0x0));
  return 0;
}