#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

uint8_t ref8(uint8_t x)
{
  x = ((x & 0x55) << 1) | ((x & 0xAA) >> 1);
  x = ((x & 0x33) << 2) | ((x & 0xCC) >> 2);
  x = ((x & 0x0F) << 4) | ((x & 0xF0) >> 4);
  return x;
}

uint8_t calc(uint8_t val, uint8_t poly)
{
  uint8_t ret = val;
  for(int j = 0; j < 8; ++j ) 
  {
    if((ret & 0x80) != 0) 
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

uint8_t CRC8_Calc(const uint8_t *val, int32_t len, uint8_t init, uint8_t poly, bool isRefIn, bool isRefOut, uint8_t xor)
{
  if(val == NULL)
    return init;
  uint8_t ret = 0;
  uint8_t crc = init;
  for(int32_t i = 0; i < len; i++)
  {
    uint8_t tmp = (isRefIn?ref8(val[i]):val[i]);
    crc = calc(crc ^ tmp, poly);
  }
  ret = isRefOut?ref8(crc):crc;
  ret ^= xor;
  return ret;
}

int main(int argc, char** argv)
{
  const char* str = "123456789";
  printf("CRC8 of\t%s =\t%#x\n", str, CRC8_Calc(str,9,0x00,0x07,false,false,0x00));
  printf("CRC8/CDMA2000 of\t%s =\t%#x\n", str, CRC8_Calc(str,9,0xFF,0x9b,false,false,0x00));
  printf("CRC8/DARC of\t%s =\t%#x\n", str, CRC8_Calc(str,9,0x00,0x39,true,true,0x00));
  printf("CRC8/DVB-S2 of\t%s =\t%#x\n", str, CRC8_Calc(str,9,0x00,0xd5,false,false,0x00));
  printf("CRC8/EBU of\t%s =\t%#x\n", str, CRC8_Calc(str,9,0xFF,0x1d,true,true,0x00));
  printf("CRC8/I-CODE of\t%s =\t%#x\n", str, CRC8_Calc(str,9,0xFD,0x1d,false,false,0x00));
  printf("CRC8/ITU of\t%s =\t%#x\n", str, CRC8_Calc(str,9,0x00,0x07,false,false,0x55));
  printf("CRC8/MAXIM of\t%s =\t%#x\n", str, CRC8_Calc(str,9,0x00,0x31,true,true,0x00));
  printf("CRC8/ROHC of\t%s =\t%#x\n", str, CRC8_Calc(str,9,0xFF,0x07,true,true,0x00));
  printf("CRC8/WCDMA of\t%s =\t%#x\n", str, CRC8_Calc(str,9,0x00,0x9b,true,true,0x00));
  return 0;
}