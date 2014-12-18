# GlueLang

A programming language that has a strong nature to be a glue of commands.


# Language Specification

The following code is a sample script of GlueLang.

```io.glue
import /bin/ as b
import /usr/bin/ as ub

proc main file:
	file f = cattac $file
	b.cat $f

func cattac file:
	b.cat $file
	ub.tail -r
```

## Pipeline

In the "cattac func" block, a pipeline of two commands
"cat $file | tail -r" is implemented.

In the "main proc" block,
a temporary file "f" is used and it stores
the result of the func block. 
Then the contents of this temporary file is
released to the stdout by the cat command.

Licence
========

MIT Licence is applied to. See LICENSE.md.
