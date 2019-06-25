### Introduction

It has often been said that the art of computer programming is part
managing complexity and part naming things. I contend that this is
largely true with the addition of "and sometimes it requires drawing
boxes". 

In this article I'll name some things and manage some complexity while
writing a small C program that is loosely based on the structure I
introduced discussing [another C program][1], but different in that it
will do something. Grab your favorite beverage, your favorite editor
and compiler, crank up some tunes and let's write a mildly interesting
C program together.

### Philosophy of A Good UNIX C Program

The first thing to know about this C program is that it's a [UNIX][2]
command-line tool. This means that it runs on, or can be ported to,
operating systems that provide a UNIX C run-time environment. When
UNIX was first invented at Bell Labs, it was imbued from the beginning
with a philosophy: programs do one thing, do it well and act on files.
While it makes sense to do one thing and do it well, the part about
acting on files may seem like a head-scratcher.

It turns out that the abstraction of a "file" is very powerful. A UNIX
file is a stream of bytes that ends with an End Of File (EOF)
marker. Any other structure in a file is imposed by the application
and not the operating system. The operating system provides system
calls which allow a program to perform a set of standard operations on
files: open, read, write, seek, fcntl, and close. Standardizing access
to files allows different programs to share a common abstraction and
work together even when they were implemented by different people in
possibly different languages.

Having a shared file interface makes it possible to build programs
that are composable. The output of one program can be the input of
another program. The UNIX family of operating systems provides
three files by default every time a program is executed:
standard in (```stdin```), standard out (```stdout```) and standard
error (```stderr```). Two of these files are opened in write-only
mode; ```stdout``` and ```stderr```, while ```stdin``` is opened
read-only. We see this in action whenever we use file redirection
in a command shell like ```bash```:

```bash
   $ ls | grep foo | sed -e 's/bar/baz/g' > ack
```

Briefly, the output of ```ls``` is written to stdout which is
redirected to the stdin of ```grep``` whose stdout is redirected
to ```sed``` whose stdout is redirected to write to a file called
'ack' in the current directory.

We want our program to play well in this ecosystem of equally flexible
and awesome programs, so lets write a program that reads and writes files. 

### Concept: MeowMeow - A Stream Encoder/Decoder

When I was a dewy-eyed kid studying computer science in the <mumbles>,
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
to illustrate how to write a C program composed of multiple files.

Having already established that I want to write a program that encodes
and decodes files in MeowMeow format, I fired up a shell and issued the
following commands:

```bash
   $ mkdir meowmeow
   $ cd meowmeow
   $ git init
   $ touch Makefile     # recipes for compiling the program
   $ touch main.c       # handles command-line options
   $ touch mmencode.c   # implements encoding a MeowMeow file
   $ touch mmencode.h   # describes the encoding API
   $ touch mmdecode.c   # implements decoding a MeowMeow file
   $ touch mmdecode.h   # describes the decoding API
   $ touch README.md    # this awesome file
   $ touch .gitignore   # names in this file are ignored by git
   $ git add .
   $ git commit -m "initial commit of empty files"
```

In short, I created directory full of empty files and committed them to git. 

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

### 





<!- XXX this could be a good outro (sentiment, if not wording) -->

This article would have turned into a very boring book if I try to describe
every line of every file, so some things I chose to gloss over. Leave me
comments to let me know how bad my choices were and where my next article
should take us.

[1]: https://opensource.com/article/19/5/how-write-good-c-main-function
[2]: https://FIXME/link_to_posix_unix_def?
[3]: http://www.jabberwocky.com/software/moomooencode.html
[4]: https://FIXME/link_to_nyan_cat_gif
[5]: https://github.com/JnyJny/meowmeow.git
