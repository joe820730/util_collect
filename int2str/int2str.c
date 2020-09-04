#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/** @file
  * Integer to string, for MCU use.
  */

/**
  * @brief integer to string, for embedded MCU use.
  * @param num An integer to be converted
  * @param buf A buffer to store converted string
  * @param bufsize Buffer size
  * @return Num of digits
  *   @retval -1 Buffer is null or not enough space.
  *   @retval >0 Convert success.
  */
int8_t int2str(int32_t num, uint8_t* buf, int bufsize)
{
  int8_t digit = 0; /* How many digits, include sign */
  int8_t i = 0;     /* counter */
  int8_t j = 0;     /* counter */
  bool isneg = (num < 0);  /* Check sign */
  int32_t tmp = isneg ? -num : num; /* Make negative num to positive */
  if(buf == NULL)
    return -1;
  do
  {
    buf[i] = (uint8_t)((tmp%10)+0x30u); /* Store num to ascii */
    ++i;  /* move index */
    tmp /= 10;
    if(i >= bufsize) return -1;  /* buffer size is not enough, return -1 */
  }while(tmp > 0);
  if(isneg)  /* If negative number */
  {
    buf[i] = '-';  /* Add sign */
    ++i;           /* move index */
    if(i >= bufsize) return -1;  /* buffer size is not enough, return -1 */
  }
  digit = i;       /* num of digits, include sign */
  buf[i] = '\0';   /* add '\0' */
  --i;   /* move index, don't swap the last position. */
  while(j < i)
  {
    buf[i] ^= buf[j];  /* xor swap */
    buf[j] ^= buf[i];
    buf[i] ^= buf[j];
    ++j;
    --i;
  }
  return digit;  /* return num of digits. */
}

int main()
{
  uint8_t buf[12] = {0};
  int cnt = 0;
  int a = 0;
  printf("Input num: ");
  scanf("%d", &a);
  cnt =  int2str(a, buf, 12);
  printf("%s, cnt=%d\n", buf, cnt);
  return 0;
}
