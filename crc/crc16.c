#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

uint16_t refin(uint16_t x)
{
  x = ((x & 0x5555) << 1) | ((x & 0xAAAA) >> 1);
  x = ((x & 0x3333) << 2) | ((x & 0xCCCC) >> 2);
  x = ((x & 0x0F0F) << 4) | ((x & 0xF0F0) >> 4);
  return x;
}

uint16_t refout(uint16_t x)
{
  x = ((x & 0x5555) << 1) | ((x & 0xAAAA) >> 1);
  x = ((x & 0x3333) << 2) | ((x & 0xCCCC) >> 2);
  x = ((x & 0x0F0F) << 4) | ((x & 0xF0F0) >> 4);
  x = ((x & 0x00FF) << 8) | ((x & 0xFF00) >> 8);
  return x;
}
uint16_t calc(uint16_t val, uint16_t poly)
{
  uint16_t ret = val;
  for(int j = 0; j < 8; ++j ) 
  {
    if((ret & 0x8000) != 0) 
    {
      ret = (ret << 1) ^ poly;
    } 
    else 
    {
      ret <<= 1;
    }
  }
  return ret;
}

uint16_t CRC16_Calc(const uint8_t *val, int32_t len, uint16_t init, uint16_t poly, bool isRefIn, bool isRefOut, uint16_t xor)
{
  if(val == NULL)
    return init;
  uint16_t ret = 0;
  uint16_t crc = init;
  for(int32_t i = 0; i < len; i++)
  {
    uint8_t tmp = (isRefIn?refin(val[i]):val[i]);
    crc = calc((tmp<<8) ^ crc, poly);
  }
  ret = isRefOut?refout(crc):crc;
  ret ^= xor;
  return ret;
}

int main(int argc, char** argv)
{
  const char* str = "123456789";
  printf("CRC-16/USB of %s =\t%#x\n", str, CRC16_Calc(str,9,0xFFFF,0x8005,true,true,0xFFFF));
  printf("CRC-16/ARC of %s =\t%#x\n", str, CRC16_Calc(str,9,0x0000,0x8005,true,true,0x0));
  printf("CRC-16/DECT-R of %s =\t%#x\n", str, CRC16_Calc(str,9,0x0000,0x0589,false,false,0x1));
  printf("CRC-16/MODBUS of %s =\t%#x\n", str, CRC16_Calc(str,9,0xFFFF,0x8005,true,true,0x0));
  return 0;
}