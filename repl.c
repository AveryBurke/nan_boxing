#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <editline/readline.h>
#include "repl.h"


/* tokenization buffer and the next character that we are looking at */
char buf[40], see = ' ';

double parse_atom()
{
  double d;
  int i;
  int32_t trunc;
  /* if the token scans as a float, first check if it can be stored as an int32, else return a float, else a symbol*/
  return (sscanf(buf, "%lg%n", &d, &i) > 0 && !buf[i]) ? d == (double)(trunc = d) ? encode_int(d) : d : new_string(buf);
}

/* advance to the next character */
void look()
{
  int c = getchar();
  see = c;
  if (c == EOF)
    exit(0);
}

/* return nonzero if we are looking at character c, ' ' means any white space */
uint64_t seeing(char c)
{
  return c == ' ' ? see > 0 && see <= c : see == c;
}

/* return the look ahead character from standard input, advance to the next */
char get()
{
  char c = see;
  look();
  return c;
}

/* tokenize into buf[], return first character of buf[] */
char scan()
{
  int i = 0;
  while (seeing(' '))
    look();
  if (seeing('(') || seeing(')') || seeing('\''))
    buf[i++] = get();
  else
    do
      buf[i++] = get();
    while (i < 39 && !seeing('(') && !seeing(')') && !seeing(' '));
  buf[i] = 0;
  return *buf;
}

double Read()
{
  scan();
  /* consider parse_atom(scan()); where scan returns a pointer to buf and parse_atom takes a char * */
  return parse_atom();
}

int main(int argc, char const *argv[])
{
  while (1)
  {
    printf("> ");
    print_value(Read());
    printf("\n");
  }
  return 0;
}
