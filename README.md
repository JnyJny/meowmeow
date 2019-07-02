### MeowMeow - a file encoder/decoder

This project implements two commands in C, ```meow``` and ```unmeow```,
that will encode and decode a file in the **MeowMeow** encoding scheme.
While not intended to be useful in and of itself, the program is an
example of how to structure a small to medium sized C command-line program.

#### Requirements

- C compiler
- GNU make

#### Build

0. Clone

```bash
   $ git clone https://github.com/JnyJny/meowmeow.git
```

1. Compile
```bash
   $ make
```

2. Run
```bash
   $ ./meow < clear_text > meow_text
   $ ./unmeow < meow_text | diff clear_text
   $
```

#### MeowMeow Encoding Scheme

The **MeowMeow** encoding scheme is direct rip-off of another encoding
scheme, [MooMoo Encode][2]. In short, each byte in file is encoded
into a string "meowmeow" where uppercase indicates a 1 and lowercase
is a zero, in this case the string encodes a zero. Consider these three
bytes:

```C
   unsigned char zeros = 0x0;
   unsigned char ones = 0xff;
   unsigned char camel = 0xA5;
```

- The value in ```zeros``` encodes to "meowmeow".
- The value in ```ones``` encodes to "MEOWMEOW".
- The value in ```camel``` encodes to "MeOwmEoW.
   


<!-- [1]: https://FIXME/internal/link/MeowMeowEncodingScheme --> 
[2]: http://www.jabberwocky.com/software/moomooencode.html
