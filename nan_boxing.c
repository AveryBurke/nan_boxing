#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "repl.h"

/* 
    returns the type of a possibly encoded double. If the double is NOT a NaN, then it is a double. 
    Otherwise the type is read off the the NaN
*/
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

/* int */

/* encode an a 32 bit int into to the payload of a qNaN, along with the int tag*/
double encode_int(double value)
{
  uint32_t narrow = (int32_t)value; //cast to avoid 
  uint64_t boxed = MASK_QNAN | MASK_INT | (uint64_t)narrow;
  return *(double *)&boxed;
}

/* get a 32 bit int from the payload of a qNaN and cast the result*/
int32_t decode_int(double value)
{
  return *(uint64_t *)&value & MASK_PAYLOAD_INT;
}

/* string */

/* encode a stirng pointer into the payload of a qNaN, along wtih the string tag*/
double encode_string(char *value)
{
  uint64_t boxed = MASK_QNAN | MASK_STRING | (uint64_t)value;
  return *(double *)&boxed;
}
/* get a 48 bit char pointer from the payload of a qNaN and cast the result to uint 64*/
uint64_t decode_string(double value)
{
  return *(uint64_t *)&value & MASK_PAYLOAD;
}

/* copy the the string to a new address. Pass the address to encode_string */
double new_string(char *value)
{
  char *new_sympol_address = malloc(strlen(value) + 1);
  strcpy(new_sympol_address, value);
  return encode_string(new_sympol_address);
}

/* print the value appropriately based on the type */
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



