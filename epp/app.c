#include "crc16.c"
#include "memory.h"
#include "stdint.h"
#include "stdio.h"

#define PAYLOAD_SIZE 256
#define DEFAULT_PAYLOAD \
  { .head = 0xFFFF, .len = 0x0D, .flag = 0x40, .reserve = {0x00, 0x00, 0x00, 0x00, 0x00}, .data = {0}, .type = 0x02, .crc = 0x00 }

typedef unsigned char Byte;

typedef struct {
  Byte head[2];     // 2 0xFF 0xFF
  Byte len;         // 1 8~254
  Byte flag;        // 1 bit-6为1是表示CRC16校验，其他位都不用
  Byte reserve[5];  // 5 0x00 0x00 0x00 0x00 0x00
  Byte type;        // 1
  Byte data[256];   // ?
  int data_len;
  Byte accumulate;     // 1 len + flag + reserve + type + data,取得低字节包括冗余数据
  unsigned short crc;  // 2 flag.bit-6为1时才有此字段，len + flag + reserve + type + data

} EppPayload;

void epp_generate_data(EppPayload* payload, Byte* buffer, int* len) {
  unsigned short sum = 0;

  //
  int pos = 0;
  memcpy(buffer + pos, payload->head, 2);
  pos += 2;

  //
  *(buffer + pos) = payload->len;
  pos++;
  sum += (payload->len & 0x00FF);

  //
  *(buffer + pos) = payload->flag;
  pos++;
  sum += (payload->flag & 0x00FF);

  //
  memcpy(buffer + pos, payload->reserve, 5);
  pos += 5;
  for (int i = 0; i < 5; i++) {
    sum += (payload->reserve[i] & 0x00FF);
  }

  //
  *(buffer + pos) = payload->type;
  pos++;
  sum += payload->type;

  //
  for (int i = 0; i < payload->data_len; i++) {
    Byte item = *(payload->data + i);
    *(buffer + pos) = item;
    pos++;
    sum += (payload->data[i] & 0x00FF);

    if (item == 0xFF) {
      *(buffer + pos) = 0x55;
      pos++;

      sum += 0x55;
    }
  }

  //
  payload->accumulate = sum & 0x00FF;
  *(buffer + pos) = payload->accumulate;
  pos++;

  //
  if (payload->flag & 0x0040) {
    // crc16有很多版本，这里计算的与文档说的不一样
    // 另外crc计算也不包括0x55
    payload->crc = crc16(buffer + 2, pos - 3);  
    memcpy(buffer + pos, &payload->crc, 2);
    pos += 2;
  }

  *len = pos;
}

int main() {
  //   EppPayload testdata1 = {.head = {0xFF, 0xFF}, .len = 0x0D, .flag = 0x01, .reserve = {0x02, 0x03, 0x04, 0x05, 0x06}, .type = 0x02, .data = {0x6D, 0x01, 0x05, 0xFF, 0xFF}, .data_len = 5};
  EppPayload testdata2 = {.head = {0xFF, 0xFF}, .len = 0x0D, .flag = 0x41, .reserve = {0x02, 0x03, 0x04, 0x05, 0x06}, .type = 0x02, .data = {0x6D, 0x01, 0x05, 0xFF, 0xFF}, .data_len = 5};

  Byte buffer[PAYLOAD_SIZE];
  int len;
  epp_generate_data(&testdata2, buffer, &len);
  for (int i = 0; i < len; i++) {
    printf("%02X ", *(buffer + i));
  }
  printf("\n");
  return 0;
}