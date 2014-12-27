[![Build Status](https://travis-ci.org/ryuichiueda/GlueLang.svg?branch=master)](https://travis-ci.org/ryuichiueda/GlueLang)

# GlueLang

A programming language that has a strong nature to be a glue of commands.


# Language Specification

The following code is a sample script of GlueLang.

```io.glue
import /bin/ as b
import /usr/bin/ as ub

main infile = do
	file tmp = cattac infile 
	b.cat tmp

cattac file = b.cat file >>= ub.tail '-r'
```

## Pipeline

In the "cattac func", a pipeline of two commands
"cat $file | tail -r" is implemented.

## Temporary files

In the "main" block,
a temporary file "f" is used and it stores
the result of the function. 
Then the contents of this temporary file is
released to the stdout by the cat command.

Temporary files are removed at the end of the program.

## import

An import statement is used for indicating the directory 
in which commands exist explicitly. 
Commands in the directory are accessed with the prefix
that is written after "as."

When we omit the prefix, we can use commands
in the directories that are designated in PATH,
which s the shell environment variable.

## other specification

Please join the discussion in issues. 
You can use Japanese. I will translate to English
if necessary.

Licence
========

MIT Licence is applied to. See LICENSE.md.
