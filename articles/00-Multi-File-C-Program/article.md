### Introduction

It has often been said that the art of computer programming is part
managing complexity and part naming things. I would argue that this is
largely true, with the addition of "and sometimes it requires drawing
boxes".

In this article I'll name some things and manage some complexity while
writing a small C program that is loosely based on the program
structure I [discussed earlier][1], but different. This one will _do_
something. Grab your favorite beverage, a trusty editor and
compiler, crank up some tunes and let's write a mildly interesting C
program together. For those waiting for the other shoe to drop, there
won't be any boxes drawn ... yet.

### Philosophy of A Good Unix Program

The first thing to know about this C program is that it's a [Unix][2]
command-line tool. This means that it runs on, or can be ported to,
operating systems that provide a Unix C run-time environment. When
Unix was first invented at Bell Labs, it was infused from the
beginning with a [design philosophy][14]: programs do one thing, do it
well and act on files.  While it makes sense to do one thing and do it
well, the part about acting on files seems a little out of place.

It turns out that the Unix abstraction of a "file" is very powerful. A
Unix file is a stream of bytes that ends with an End Of File (EOF)
marker. That's it. Any other structure in a file is imposed by the
application and not the operating system. The operating system
provides system calls which allow a program to perform a set of
standard operations on files: open, read, write, seek, and close
(there are others but those are the biggies). Standardizing access to
files allows different programs to share a common abstraction and work
together even when they were implemented by different people in
different programming languages.

Having a shared file interface makes it possible to build programs
that are **composable**. The output of one program can be the input of
another program. The Unix family of operating systems provides
three files by default whenever a program is executed:

- Standard Input  - `stdin`
- Standard Output - `stdout`
- Standard Error  - `stderr`

Two of these files are opened in write-only mode; `stdout` and `stderr`,
while `stdin` is opened read-only. We see this in action whenever we use
file redirection in a command shell like `bash`:

```console
$ ls | grep foo | sed -e 's/bar/baz/g' > ack
```

This construction can be described briefly as: the output of `ls`
is written to stdout which is redirected to the stdin of `grep`
whose stdout is redirected to `sed` whose stdout is redirected to
write to a file called 'ack' in the current directory.

We want our program to play well in this ecosystem of equally flexible
and awesome programs, so lets write a program that reads and writes
files.

### Concept: MeowMeow - A Stream Encoder/Decoder

When I was a dewy-eyed kid studying computer science in the late 80s
and early 90s, there were an actual plethora of encoding schemes. Some
of them were for compressing files, some were for packaging files
together and others had no purpose but to be excrutiatingly silly. An
example of the last is the [MooMoo encoding scheme][3].

To give our program a purpose, I'll update this concept for the modern
era and implement a MeowMeow encoding since [the Internet loves
cats][4]. The basic idea here is to take a file and encode each nibble
(half of a byte or four bits) with the text "meow". A lower-case
letter indicates a zero and upper-case indicates a one. Yes, it will
balloon the size of a file since we are trading four bits for thirty
two bits. Yes, it's pointless. But just imagine the surpise on
someone's face when this happens:

```console
$ cat /home/your_sibling/.super_secret_journal_of_my_innermost_thoughts
mEowmEowmEOwmEoWmEOwmeoWmEOWmeOwmeOwmeowmEOwmEowmEOwMeoWmEOwmeoWmEOWmeOwmEOWMeoWmeOwMEowmeowMeOwmeowMeOwmEoWmEowmEOwMEOWmEOwmEowmEOwmeoWmEOWMeoWmeOwmeowmEOwMeoWmEOWmeOWmeOwmeowmEoWmeOWmEOwmeoWmEOWmEowmEOWmEoWmEOWmeOwmEOwmEowmEOwmeoWmEOWMeoWmeOwMEowmeOwmeowmeOWmeowmeOWmeoWmeOwMEoWmeOWmeOwmeOWMeoWmeOwMEoWmeOWmeOwmeOWmeowmeOWmeOwmeOWmeOwmeOwMEowmeOwmeowmEOwmeoWmEOwMEOwmEOwmEowmeOwmeowmEowMeoWmeOwmeowmEOwmeOWmEOwmeoWmEOwMEOwmeOwmEOWmEOWmEowmeOwmeowmEOwmeOwmEOwmEoWmEOwMEowmEOwMeoWmEOwmEoWmEOWmEOwmEOwmEoWmeOwmeowmEOwMEoWmEOWMeoWmeOwmeowmEOwMEowmEOWmEoWmEOwmeOWmEOwMeOWmeOwmeoWmeowMeOw
```

This is going to be _awesome_.

### Implementation, Finally

The full source for this can be found on [GitHub][0] but I'm going to
talk through my thought process while writing it. The object here is
to illustrate how to structure a C program composed of multiple files.

Having already established that I want to write a program that encodes
and decodes files in MeowMeow format, I fired up a shell and issued the
following commands:

```console
$ mkdir meowmeow
$ cd meowmeow
$ git init           # initialize this directory as git repo
$ touch Makefile     # recipes for compiling the program
$ touch main.c       # handles command-line options
$ touch main.h       # "global" constants and definitions
$ touch mmencode.c   # implements encoding a MeowMeow file
$ touch mmencode.h   # describes the encoding API
$ touch mmdecode.c   # implements decoding a MeowMeow file
$ touch mmdecode.h   # describes the decoding API
$ touch table.h      # defines encoding lookup table values
$ touch .gitignore   # names in this file are ignored by git
$ git add .
$ git commit -m "initial commit of empty files"
```

In short, I created a directory full of empty files and committed them
to a [git][6] source code repository.

Even though the files are empty, you can infer the purpose of each
from it's name. Just in case you can't infer, I annotated each `touch`
with a brief description.

Normally a program starts as a single `main.c` file that is simple,
with only two or three functions that solve the problem. And then the
programmer rashly shows that program to a friend or her boss and
suddenly the number of functions in the file balloons to support all
the new "features" and "requirements" that pop up. First rule of
"Program Club" is don't talk about "Program Club".  The second rule is
minimize the number of functions in one file.

To be honest, the C compiler does not care if every function in your
program is in one file. But we don't write programs for computers or
compilers, we write them for other people (who are sometimes us). I
know that is probably a surprise, but it's true. A program encodes a
set of algorithms that solve a problem with a computer, and it's
important that people understand the program when the parameters of
the problem change in unanticipated ways. People will have to modify
the program and they will curse your name if you have all 2049
functions in one file.

So we good and true programmers break functions out, grouping like
functions into seperate files. Here I've got files [main.c][10],
[mmencode.c][13], and [mmdecode.c][15]. For small programs like this,
it may seem like overkill. But small programs rarely stay small, so
planning for expansion is a "Good Idea".

But what about those `.h` files? I'll explain them in general terms
later, but in brief those are called _header_ files and they can
contain C language type definitions and C preprocessor
directives. Header files should **not** have any functions in them
(looking at you C++).  You can think of headers as a definition of the
application programming interface (API) offered by the `.c` flavored
file that is used by other `.c` files.

### But What The Heck is a Makefile?

I know all you cool kids are using the **"Ultra CodeShredder 3000™
V0.4-beta"** integrated development environment to write the next
blockbuster app and building your project consists of mashing on
"Ctrl-Meta-Shift-Alt-Super-B". But back in my day, and also today,
lots of useful work got done by C programs built with makefiles. A
makefile is a text file, typically named "**Makefile**", that contains
recipes for working with files which programmers use to automate
building their program binaries from source (and other stuff too!)
using the [make][13] command-line utility.

For instance, this little gem:

```Makefile
00 # Makefile
01 TARGET= my_sweet_program
02 $(TARGET): main.c
03 	cc -o my_sweet_program main.c
```

Text after an [octothorpe][17] is a comment, like line 00.

Line 01 is a variable assignment where the variable TARGET takes on
the string value "my_sweet_program". By convention, ok my
_preference_, all makefile variables are capitalized and use underscores
to seperate words.

Line 02 consists of the name of the file that the recipe creates
and the files it depends on. In this case the target is `my_sweet_program`
and the dependency is `main.c`.

The final line, 03, is indented with a tab and not four spaces. This is
the command that will be executed to create the target. In this case,
we call `cc` the C compiler front-end to [compile and link][article-compiling]
`my_sweet_program`.

Using a makefile is simple:

```console
$ make
cc -o my_sweet_program main.c
$ ls 
Makefile  main.c  my_sweet_program
```

Could you build your C program without a makefile? Yes, but why?

The [makefile][16] that will build our MeowMeow encoder/decoder is
only a little more sophisticated than this example, but the basic
structure is the same. I'll break it down [Barney-style][article-makefile]
for you, and you'll be writing cool makefiles in no time.

### Form Follows Function

My idea for this exercise is to write a program that reads a file,
transforms it and then writes the transformed data to another
file. The following fabricated command-line interaction is how I
imagine using the program:

```console
$ meow < clear.txt > meow.txt       # transform clear text to meowmeow text
$ unmeow < meow.txt > unmeowed.tx   # transform meowmeow text to clear text
$ diff clear.txt unmeowed.txt       # compare the original file to the unmeowed file
$                                   # no output means there was no difference
```

We need to write code to handle command-line parsing and managing the
input and output streams. We need a function to encode a stream and
write it to another stream. And finally we need a function to decode a
stream and write it to another stream. Wait a second, I've only been
talking about writing one program but in the example above I invoke
two commands; `meow` and `unmeow`? I know you are probably thinking
that this is getting complex as heck.

### Minor Side Track - `argv[0]` and the `ln` command

If you recall, the signature of a C main function is:

```C
int main(int argc, char *argv[])
```

where `argc` is the number of command-line arguments and `argv` is a
list of character pointers (strings).  The value of `argv[0]` is the
path of the file containing the program being executed. Many Unix
utility programs with complementary functions (e.g. compress and
uncompress ) look like two programs, but in fact they are somteimes
one program with two names in the filesystem. The two-name trick is
accomplished by creating a filesystem "link" using the `ln` command.

An example from the directory `/usr/bin` on a fictional computer is:
```console
$ ls -li /usr/bin/*compress | grep 3376
3376 -rwxr-xr-x. 113 root root     1.5M Aug 30  2018 /usr/bin/compress
3376 -rwxr-xr-x. 113 root root     1.5M Aug 30  2018 /usr/bin/uncompress
```

Here `compress` and `uncompress` are the same file with different
names. We can tell it's the same file since they have the same i-node
number (the first column). An i-node is a feature of the Unix
filesystem and is super outside the scope of this article, but not
[this one][article-unix-fs].

In our [makefile][16], the `unmeow` link is created using this recipe:

```Makefile
# Makefile
...
$(DECODER): $(ENCODER)
	$(LN) -f $< $@
...
```

I tend to parameterize everything in my makefiles, rarely using a
"bare" string. I group all the definitions at the top of the makefile
which makes it easy to find them and change them. This makes a big
difference when you are trying to port software to a new platform
and you need to change all your rules to use `cool-new-cc` instead of
`cc`.

The recipe should appear relatively straightforward except for the two
built-in variables `$@` and `$<`. The first is a short cut for
the target of the recipe; in this case `$(DECODER)`. I remember that
because the at-sign looks like a target to me. The second varaible `$<` is
the rule dependency. In this case it resolves to `$(ENCODER)`.

Good and/or lazy programmers can use this feature of the Unix
filesystem to write less code but double the number of programs they
deliver. First we write a program that changes it's behavior based on
the value of `argv[0]` and then we make sure to create links with
the names that cause the behavior.

For instance, this simple `main()` checks to see if the execution was
started with a file named "foo" or "bar" and executes a `foo()` or
`bar()` function based on the file name.

```C
int foo(int argc, char **argv);
int bar(int argc, char **argv);

int main(int argc, char **argv) {

  if (strncmp(argv[0], "foo", 3) == 0) {
    return foo(argc, argv);
  }

  if strncmp(argv[0], "bar" 3) == 0) {
    return bar(argc, argv);
  }
  
  return -1
}
```

Things are getting complex for sure, but it's managed.

### Exploring `main.c`

The structure of the [main.c][11] file for `meow`/`unmeow` should be
familiar to my [long-time readers][1], and has the following general
outline:

```C
/* main.c - MeowMeow, a stream encoder/decoder */

/* 00 system includes */
/* 01 project includes */
/* 02 externs */
/* 03 defines */
/* 04 typedefs */
/* 05 globals (but don't)*/
/* 06 ancillary function prototypes if any */

int main(int argc, char *argv[])
{
  /* 07 variable declarations */
  /* 08 check argv[0] to see how the program was invoked */
  /* 09 process the command-line options from the user */
  /* 10 do the needful */
}

/* 11 ancillary functions if any */
```

### Including Project Header Files

The second section, `/* 01 project includes /*` reads like this from
the source:

```C
/* main.c - MeowMeow, a stream encoder/decoder */
...
/* 01 project includes */
#include "main.h"
#include "mmecode.h"
#include "mmdecode.h"
```

The `#include` directive is a C pre-processor command that causes
the contents of the named file to be "included" at this point in the file.
If the programmer uses double-quotes around the name of the header file,
the compiler will look for that file in the current directory. If the file
is enclosed in `<>`, it will look for the file in a set of predefined
directories.

The file [main.h][10] contains definitions and typedefs that are used
in [main.c][11]. I like to collect these things here in case I want
to use those definitions elsewhere in my program.

The files [mmencode.h][12] and [mmdecode.h][14] are nearly
identical so I'll break down `mmencode.h`.

```C
/* mmencode.h - MeowMeow, a stream encoder/decoder */
    
#ifndef _MMENCODE_H
#define _MMENCODE_H
    
#include <stdio.h>
    
int mm_encode(FILE *src, FILE *dst);
    
#endif	/* _MMENCODE_H */
```

The `#ifndef`, `#define`, `#endif` construction is collectively known
as a "guard". This keeps the C compiler from including this file more
than once per file. The compiler will complain if it finds multiple
definitions/prototypes/declarations so the guard is a **must have**
for header files. Yes, some compilers provide support for only
including a header once, but once you release your software you lose
control over what compiler is used to build your source. Always
include a guard in your headers.

Inside the guard, there are only two things; a `#include`
directive and a function prototype declaration. I include `stdio.h`
here to bring in the definition of `FILE` which is used in
the function prototype. The function prototype can be included
by other C files to establish that function in the file's namespace.
You can think of each file as a seperate **namespace** which means
variables and functions in one file are not usable by functions or
variables in another file.

Writing header files is complex and it can be tough to manage in
larger projects. Use guards. I have more thoughts about [C header
files][article-headers] if you've got some more time to kill.

### MeowMeow Encoding, Finally

The meat and potatoes of this program, encoding and decoding bytes
into/out of "MeowMeow" strings, is actually the fun part of writing
this project.  All of our activities up to now have been putting the
scaffolding in place to support calling this function; parsing the
command-line, determining which operation to use and opening the files
that we'll operate upon. Here is the encoding loop:

```C
/* mmencode.c - MeowMeow, a stream encoder/decoder */
...
while (!feof(src)) {

  if (!fgets(buf, sizeof(buf), src))
    break;

  for(i=0; i<strlen(buf); i++) {
    lo = (buf[i] & 0x000f);
    hi = (buf[i] & 0x00f0) >> 4;
    fputs(tbl[hi], dst);
    fputs(tbl[lo], dst);
  }
}
```

In plain English, this loop reads in a chunk of the file while there
are chunks left to read (`feof(3)` and `fgets(3)`). Then it splits
each byte in the chunk into `hi` and `lo` nibbles. Remember, a nibble
is half of a byte, or four bits. The real magic here is realizing that
four bits can encode sixteen values. I use `hi` and `lo` as indices
into a sixteen string lookup table, `tbl`, that contains the
**MeowMeow** strings that encode each nibble. Those strings are
written to the destination `FILE` stream using `fputs(3)` and then we
move on to the next byte in the buffer.

The table is initialized with a macro defined in [table.h][17]
for no particular reason except to demonstrate including another project
local header file and I like [initialization macros][13].

### MeowMeow Decoding

Alright, I'll admit it took me a couple of runs at this before I got
it working. The decode loop is similar; read a buffer full of **MeowMeow**
strings and then reverse the encoding from strings to bytes. From the
file [mmdecode.c][15]:

```C
/* mmdecode.c - MeowMeow, a stream decoder/decoder */
...
int mm_decode(FILE *src, FILE *dst)
{
  if (!src || !dst) {
    errno = EINVAL;
    return -1;
  }
  return stupid_decode(src, dst);
}
```
	
Not what you were expecting?

Here, I'm exposing the function `stupid_decode()` via the externally
visible `mm_decode()` function. When I say "externally" visible I mean
visible outside this file. Since `stupid_decode()` isn't in the header
file [mmdecode.h][14], it isn't available to be called directly from
functions in other files.

Sometimes we do this when we want to publish a solid public interface
but we aren't quite done noodling around with functions to solve a
problem or we have different versions for different hardware or
software platforms. In this case, I've written a I/O intensive function
that reads eight bytes at a time from the source stream to decode one
byte to write to the destination stream. A better implementation would
work on a buffer bigger than eight bytes at a time. A _much_ better
implementation would also buffer the output bytes to reduce the number
of single byte writes to the destination stream.

```C
/* mmdecode.c - MeowMeow, a stream decoder/decoder */
...
int stupid_decode(FILE *src, FILE *dst)
{
  char           buf[9];
  decoded_byte_t byte;
  int            i;

  while (!feof(src)) {
    if (!fgets(buf, sizeof(buf), src))
      break;
    byte.field.f0 = isupper(buf[0]);
    byte.field.f1 = isupper(buf[1]);
    byte.field.f2 = isupper(buf[2]);
    byte.field.f3 = isupper(buf[3]);
    byte.field.f4 = isupper(buf[4]);
    byte.field.f5 = isupper(buf[5]);
    byte.field.f6 = isupper(buf[6]);
    byte.field.f7 = isupper(buf[7]);

    fputc(byte.value, dst);
  }
  return 0;
}
```

Instead of using the bit-shifting technique I used in the encoder, I
elected to create a custom data structure called `decoded_byte_t`.

```C
/* mmdecode.c - MeowMeow, a stream decoder/decoder */
...
typedef struct {
  unsigned char f7:1;
  unsigned char f6:1;
  unsigned char f5:1;
  unsigned char f4:1;
  unsigned char f3:1;
  unsigned char f2:1;
  unsigned char f1:1;
  unsigned char f0:1;
} fields_t;

typedef union {
  fields_t      field;
  unsigned char value;
} decoded_byte_t;
```

It's a little complex when viewed all at once, but hang tough.  The
type `decoded_byte_t` is defined as a `union` of a `fields_t`
structure and an `unsigned char`.  The named members of a union can be
thought of as aliases for the same region of memory. In this case,
`value` and `field` refer to the same eight-bit region of
memory. Setting `field.f0` to 1 would also set the least significant
bit in `value`.

While the type `unsigned char` shouldn't be a mystery, the `typedef`
for `fields_t` might look a little unfamiliar. Modern C compilers
allow programmers to specify 'bit fields' in a `struct`. The field
type needs to be an unsigned integral type and the member identifer is
followed by a colon and an integer that specifies the length of the
bit field.

This data structure makes it simple to access each bit in the byte by
field name and then access the assembled value via the `value`
field of the union. We depend on the compiler to generate the correct
bit-shifting instructions to access the fields, which can save you a
lot [heartburn][05] when you are debugging.

Lastly, `stupid_decode()` is *stupid* because it only reads eight
bytes at time from the source `FILE` stream. Normally, we try to
minimize the number of reads and writes to improve performance. I
explain the cost of system calls in more detail [here][article-syscalls],
for now just remember reading or writing a bigger chunk less often
is much better than reading/writing a lot of smaller chunks more
frequently.

### The Wrap Up

Writing a multi-file program in C requires a little more planning on
behalf of the programmer than just a single `main.c`. But just a
little effort up front can save a lot of time and headache when you
refactor as you add functionality.

To recap I like to have a lot of files with a few short functions in
them. I like to expose a small subset of the functions in those files
via header files. I like to keep my constants in header files, both
numeric and string constants. I _love_ makefiles and use them
instead of bash scripts to automate all sorts of things. I like my
`main()` function to handle command-line argument parsing and act as a
scaffold for the primary functionality of the program.

I know I've only touched the surface of what's going on in this simple
program and I'm excited to learn what things were helpful to you and
which topics need better explanations. Share your thoughts in the
comments to let me know.

<!-- fin, end links follow -->

[0]: https://github.com/JnyJny/meowmeow.git
[1]: https://opensource.com/article/19/5/how-write-good-c-main-function
[2]: https://en.wikipedia.org/wiki/Unix
[3]: http://www.jabberwocky.com/software/moomooencode.html
[4]: https://en.wikipedia.org/wiki/Cats_and_the_Internet
[5]: https://en.wikipedia.org/wiki/Endianness
[6]: https://git-scm.com
[7]: https://en.wikipedia.org/wiki/Make_(software)
[8]: http://harmful.cat-v.org/cat-v/
[9]: https://en.wikipedia.org/wiki/Octothorpe

<!-- GitHub project file links -->

[10]: https://github.com/JnyJny/meowmeow/blob/master/main.h
[11]: https://github.com/JnyJny/meowmeow/blob/master/main.c
[12]: https://github.com/JnyJny/meowmeow/blob/master/mmencode.h
[13]: https://github.com/JnyJny/meowmeow/blob/master/mmencode.c
[14]: https://github.com/JnyJny/meowmeow/blob/master/mmdecode.h
[15]: https://github.com/JnyJny/meowmeow/blob/master/mmdecode.c
[16]: https://github.com/JnyJny/meowmeow/blob/master/Makefile
[17]: https://github.com/JnyJny/meowmeow/blob/master/table.h

<!-- Links to articles referenced by this article that remain to be written -->

[article-man]: https://github.com/JnyJny/meowmeow/articles/05-Manpages.md
[article-makefile]: https://github.com/JnyJny/meowmeow/articles/10-Makefiles.md
[article-headers]: https://github.com/JnyJny/meowmeow/articles/20-Headers.md
[article-unix-fs]: https://github.com/JnyJny/meowmeow/articles/30-Unix_Filesystem.md
[article-syscalls]: https://github.com/JnyJny/meowmeow/articles/40-SystemCalls.md
[article-namespaces]: https://github.com/JnyJny/meowmeow/articles/50-Namespaces.md
[article-files]: https://github.com/JnyJny/meowmeow/articles/60-Working-With-Files.md
[article-compiling]: https://github.com/JnyJny/meowmeow/articles/70-Compiling.md
[article-shared-object]: https://github.com/JnyJny/meowmeow/articles/80-Shared-Object.md
