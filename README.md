# NaN Boxing Demo

This is a simple demonstration of the NaN Boxing implementation described in my blog post [Dynamic Typing and NaN Boxing](https://averyburke.github.io/blog/2023/08/23/Nan-Boxing.html). 

NaN boxing is an optimization in dynamically typed languages to store type data and value data together, at runtime, by exploiting some features of the [IEEE specification for NaN](https://en.wikipedia.org/wiki/NaN). It turns out the specification leaves 51 mantissa bits that get ignored when a double is detected to be Not a Number. NaN boxing works by storing the type and the value of a parsed token in those overlooked bits. Essentially every value is a double and every non-double is NaN.

I use the three most significant digits of the 51 mantissa bits to store type data. The three bit type data is also referred to as a *tag*. The remaining 48 digits store the value data. I will refer to this as the *payload*. Since modern architectures only use the lower 48 bits of a 64 bit pointer, the payload can even hold pointers.

## Features

This implementation supports three types: `float`, `int` and `string`. The repl will try to first parse a float, if successful it will test if the float can be represented as a 32 bit int. Otherwise it will treat the token as a string.

### Floats

With NaN boxing you get floats for free. So if a token can be parsed as a float nothing more needs to be done.

### Ints

This implementation uses 32 bit ints. An int is encoded by tagging a NaN as an int and directly encoding the int’s bits into the payload. An int is decoded by masking the 32 bits from the tagged NaN, truncating the masked payload and casting the result as an int32_t.

### strings

A parsed string is first copied to the heap. Then a pointer to the new address is encoded into a tagged NaN. Strings are decoded by masking the 48 bit payload and casting the result to a char pointer


## Dependencies

You will need [just](https://github.com/casey/just) installed if you want to build and test the project. You can also simply run the REPL from the binary

## Usage

Start the REPL with `./repl` or with `just run`. Enter expressions and watch as they are evaluated right before your eyes!

```lisp
$> (+ 8 -8 .1234)
type: string, value: (
type: string, value: +
type: int, value: 8
type: int, value: -8
type: float, value: 0.123400
type: string, value: )
```

Run tests with `just test`.