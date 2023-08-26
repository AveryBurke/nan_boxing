#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "repl.h"

Tag get_type(double value)
{
  uint64_t converted = *(uint64_t *)&value;
  uint64_t type = converted & MASK_TAG;
  /* if the value is NOT NaN then it is a float*/
  if ((~converted & MASK_EXPONENT) != 0)
    return TYPE_FLOAT;
  switch (type)
  {
  case (MASK_QNAN | MASK_INT):
    return TYPE_INT;
  case (MASK_QNAN | MASK_STRING):
    return TYPE_STRING;
  }
  return TYPE_ERROR;
}

int32_t decode_int(double value)
{
  return *(uint64_t *)&value & MASK_PAYLOAD_INT;
}

double encode_string(char *value)
{
  uint64_t boxed = MASK_QNAN | MASK_STRING | (uint64_t)value;
  return *(double *)&boxed;
}

uint64_t decode_string(double value)
{
  return *(uint64_t *)&value & MASK_PAYLOAD;
}

void print_value(double value)
{

  switch (get_type(value))
  {

  case TYPE_FLOAT:
  {
    printf("type: float, value: %f", value);
    break;
  }

  case TYPE_INT:
  {
    printf("type: int, value: %d", decode_int(value));
    break;
  }

  case TYPE_STRING:
  {
    printf("type: string, value: %s", (char *)decode_string(value));
    break;
  }

  default:
  {
    printf("Unknown value type!");
  }
  }
}

double new_string(char *value)
{
  char *new_sympol_address = malloc(strlen(value) + 1);
  strcpy(new_sympol_address, value);
  return encode_string(new_sympol_address);
}

double encode_int(double value)
{
  uint32_t narrow = (int32_t)value;
  uint64_t boxed = MASK_QNAN | MASK_INT | (uint64_t)narrow;
  return *(double *)&boxed;
}

