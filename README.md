Abstraction layer for talking to SQL servers (and SQLite 3).

### Building from git

You will need git, automake, autoconf and libtool. You'll also need a
DocBook 5 and DocBook-XSL toolchain (i.e., xsltproc and the stylesheets)
if you wish to rebuild any documentation.

    $ git clone git://github.com/nevali/libsql.git
    $ cd libsql
    $ git submodule update --init --recursive
    $ autoreconf -i
    $ ./configure --prefix=/some/path
    $ make
    $ sudo make install

## Licenses

libsql is licensed under the Apache License, Version 2.0

Incorporates a modified version of vasprintf() from NetBSD, licensed under
the following terms:

Copyright (c) Ian F. Darwin 1986-1995.
Software written by Ian F. Darwin and others;
maintained 1995-present by Christos Zoulas and others.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

* Redistributions of source code must retain the above copyright
  notice immediately at the beginning of the file, without modification,
  this list of conditions, and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.

The included liburi is licensed under the Apache License, Version 2.0, and
incorporates uriparser, which is licensed under the terms of the
[New BSD license](http://uriparser.git.sourceforge.net/git/gitweb.cgi?p=uriparser/uriparser;a=blob;f=COPYING).

The included [libedit](http://www.thrysoee.dk/editline/) is licensed under
the terms of the [New BSD license](http://www.netbsd.org/about/redistribution.html).
