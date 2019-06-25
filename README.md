### Introduction

It has often been said that the art of computer programming is part
managing complexity and part naming things. I contend that this is
largely true with the addition of "and sometimes it requires drawing
boxes". 

In this article I'll name some things and manage some complexity while
writing a small C program that is loosely based on the structure I
[discussed earlier][1], but different. This one will do
something. Grab your favorite beverage, your favorite editor and
compiler, crank up some tunes and let's write a mildly interesting C
program together.

### Philosophy of A Good UNIX C Program

The first thing to know about this C program is that it's a [UNIX][2]
command-line tool. This means that it runs on, or can be ported to,
operating systems that provide a UNIX C run-time environment. When
UNIX was first invented at Bell Labs, it was imbued from the beginning
with a philosophy: programs do one thing, do it well and act on files.
While it makes sense to do one thing and do it well, the part about
acting on files may seem like a head-scratcher.

It turns out that the UNIX abstraction of a "file" is very powerful. A
UNIX file is a stream of bytes that ends with an End Of File (EOF)
marker. That's it. Any other structure in a file is imposed by the
application and not the operating system. The operating system
provides system calls which allow a program to perform a set of
standard operations on files: open, read, write, seek, fcntl, and
close. Standardizing access to files allows different programs to
share a common abstraction and work together even when they were
implemented by different people in possibly different languages.

Having a shared file interface makes it possible to build programs
that are composable. The output of one program can be the input of
another program. The UNIX family of operating systems provides
three files by default whenever a program is executed:
standard in (```stdin```), standard out (```stdout```) and standard
error (```stderr```). Two of these files are opened in write-only
mode; ```stdout``` and ```stderr```, while ```stdin``` is opened
read-only. We see this in action whenever we use file redirection
in a command shell like ```bash```:

```bash
   $ ls | grep foo | sed -e 's/bar/baz/g' > ack
```

This construction can be described briefly as; the output of ```ls```
is written to stdout which is redirected to the stdin of ```grep```
whose stdout is redirected to ```sed``` whose stdout is redirected to
write to a file called 'ack' in the current directory.

We want our program to play well in this ecosystem of equally flexible
and awesome programs, so lets write a program that reads and writes files. 

### Concept: MeowMeow - A Stream Encoder/Decoder

When I was a dewy-eyed kid studying computer science in the &lt;mumbles&gt;'s,
there were an actual plethora of encoding schemes. Some of them were
for compressing files, some were for packaging files together and
others had no purpose but to be excrutiatingly silly. An example of
the last is the [MooMoo encoding scheme][3].

For our example, I'll update this concept for the [2000s][4] and implement
a MeowMeow encoding since the Internet loves cats. The basic idea here is
to take files and encode each byte with the text "meow". A lower-case letter
indicates a zero and upper-case indicates a one. Yes, it will balloon the
size of a file since we are trading a single bit for eight bits. Yes it's
pointless. But imagine the surpise on someone's face when this happens:

```
   $ cat /home/your_sibling/.super_secret_journal_of_my_innermost_thoughts
   MeOWmeOWmeowMEoW...
```

This is going to be awesome.

### Implementation, Finally

The full source for this can be found on [GitHub][5] but I'm going to
talk through my thought process while writing it. The object here is
to illustrate how to struture a C program composed of multiple files.

Having already established that I want to write a program that encodes
and decodes files in MeowMeow format, I fired up a shell and issued the
following commands:

```bash
   $ mkdir meowmeow
   $ cd meowmeow
   $ git init
   $ touch Makefile     # recipes for compiling the program
   $ touch main.c       # handles command-line options
   $ touch main.h       # "global" constants and definitions
   $ touch mmencode.c   # implements encoding a MeowMeow file
   $ touch mmencode.h   # describes the encoding API
   $ touch mmdecode.c   # implements decoding a MeowMeow file
   $ touch mmdecode.h   # describes the decoding API
   $ touch README.md    # this awesome file
   $ touch .gitignore   # names in this file are ignored by git
   $ git add .
   $ git commit -m "initial commit of empty files"
```

In short, I created a directory full of empty files and committed them to git. 

Even though the files are empty, you can infer the purpose of each
from it's name. Just in case you can't, I annotated each ```touch```
with a brief description.

So normally a program starts as a ```main.c``` that is simple, with only two
or three functions that solve the problem. And then the programmer rashly
shows that program to a friend or her boss and suddenly the number of functions
in the file balloons to support all the new "features" and "requirements"
that pop up. First rule of "Program Club" is don't talk about "Program Club".
The second rule is minimize the number of functions in one file.

Now the C compiler does not care one little bit if every function in
your program is in one file. But we don't write programs for computers
or compilers, we write them for other people (who are sometimes us). I
know that is probably a surprise, but it's true. A program embodies a
set of algorithms that solve a problem with a computer, and it's
important that people understand it when the parameters of the problem
change in unanticipated ways. People will have to modify the program
and they will curse your name if you have all 2049 functions in one
file.

So we good and true programmers break functions out, grouping like
functions into seperate files. Here I've got files **```main.c```**,
**```mmencode.c```**, and **```mmdecode.c```**. For small programs
like this, it may seem like overkill. But small programs rarely stay
small, so planning for expansion is a "Good Idea™".

But what about those ```.h``` files? I'll explain them in detail
later, but in brief those are called _header_ files and they generally
contain C language type definitions and C preprocessor
directives. Header files should **not** have any functions in them.

### What The Heck is a Makefile?

I know all you cool kids are using the "Ultra CodeShredder 3000"
integrated development environment to write the next blockbuster app
written in a boutique languge for a curated marketplace and building
your project consists of mashing on "Ctrl-Meta-Shift-B". But back in
my day (and also today), lots of useful work gets done by programs
built with Makefiles. A Makefile is a text file that contains recipes
for working with files and programmers generally use it for building
their program binaries from source. Makefiles can be used to automate
just about anything, but their original purpose was building programs
from source.

For instance, this little gem:

```00 # Makefile
   01 my_sweet_program: main.c
   02    cc -o my_sweet_program main.c
```

Text after an octothorpe/pound/hash is a comment, like line 00.

Line 01 consists of the name of the file that the recipe creates
and the files it depends on. In this case the target is ```my_sweet_program```
and the dependency is ```main.c```.

The final line, 02, is indented with an honest-to-Crom tab (not four spaces)
and is the command (or set of commands ) that will be executed to create the
target. In this case, we call ```cc``` the C compiler front-end to compile
and link ```my_sweet_program```.

Using a Makefile is simple:

```bash
   $ make
   cc -o my_sweet_program main.c
   $ ls 
   Makefile  main.c  my_sweet_program
```

The Makefile that will build our MeowMeow encoder/decoder is
considerably more sophisticated than this example, but the basic
structure is the same. Some targets create real files, some targets
are 'phony' since they don't create a specific file but act as a sort
of alias. I smell another article here about writing Makefiles. Before
I forget, the GNU Make command has excellent [documentation][6].

### Form Follows Function

The idea here is to write a program that reads a file, transforms
it and then writes it to another file. The following fabricated
command-line interaction is how imagine using the program:

```bash
	$ meow -i clear_text -o meow_text
	$ unmeow -i meow_text -o new_clear_text
	$ diff clear_text new_clear_text
	no diff
```

We need to write code to handle command-line parsing and managing
the input and output streams. We'll need a function to encode a stream and
write it to another stream. And finally we'll need a function to decode a
stream and write it to another stream. Wait a second, I've only been
talking about writing one program but in the example above I 
call ```meow``` and ```unmeow```? 

### Minor Side Track - ```argv[0]``` and the ```ln``` command

If you recall, the signature of a C main function is:

```C
int main(int argc, char *argv[])
```

where ```argc``` is the number of command line arguments
and ```argv``` is a list of character pointers (strings).
The value of ```argv[0]``` is the path of the file containing
the program being executed. Many UNIX utility programs with
complementary functions (e.g. compress and uncompress ) look
like two programs, but in fact are one program with two names
in the file system. The two-name trick is accomplished by creating
a file system "link" using the ```ln``` command.

An example from ```/usr/bin``` on my laptop is:
```bash
   $ ls -li /usr/bin/git*
3376 -rwxr-xr-x. 113 root root     1.5M Aug 30  2018 /usr/bin/git
3376 -rwxr-xr-x. 113 root root     1.5M Aug 30  2018 /usr/bin/git-receive-pack
...
```

Here ```git``` and ```git-receive-pack``` are the same file with
different names. We can tell it's the same file since they have the
same i-node number (the first column). An i-node is a feature of the
UNIX file system and is super outside the scope of this article. Also
smells like another article.

Good and/or lazy programmers can use this feature of the UNIX
filesystem to write less code but double the number of programs they
deliver. First we write a program that changes it's behavior based on
the value of ```argv[0]``` and then we make sure to create links
(sometimes called hard-links) with the names that cause the behavior.

Things are getting complex for sure, but it's managed.

### Exploring ```main.c```

The structure of the ```main.c``` file for ```meow```/```unmeow```
should be familiar to my [long-time readers][1], and has the
following general outline:

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
       /* 09 process the command line options from the user */
	   /* 10 do the needful */
   }
   
   /* 11 ancillary functions if any */
```

### Including Header Files

The second section, ```01 project includes``` reads like this from the source:

```C
   /* main.c - MeowMeow, a stream encoder/decoder */
   ...
   /* 01 project includes */
   #include "main.h"
   #include "mmecode.h"
   #include "mmdecode.h"
```

The ```#include``` directive is a C pre-processor command that causes
the contents of the named file to be "included" at this point in the file.
If the programmer uses double-quotes around the name of the header file,
the compiler will look for that file in the current directory. If the file
is enclosed in &lt;&gt;, it will look for the file in a set of predefined
directories. There is a lot unpack in those header files, enough for a
whole 'nother article.














<!- XXX this could be a good outro (sentiment, if not wording) -->

This article would have turned into a very boring book if I try to
describe every line of every file, so some things I had to gloss over
to avoid putting everyone to sleep. Leave me comments to let me know
how bad my choices were and where my next article should take us.

[1]: https://opensource.com/article/19/5/how-write-good-c-main-function
[2]: https://FIXME/link_to_posix_unix_def?
[3]: http://www.jabberwocky.com/software/moomooencode.html
[4]: https://FIXME/link_to_nyan_cat_gif
[5]: https://github.com/JnyJny/meowmeow.git
[6]: https:///FIXME/make_documentation
