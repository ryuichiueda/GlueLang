[![Build Status](https://travis-ci.org/ryuichiueda/GlueLang.svg?branch=master)](https://travis-ci.org/ryuichiueda/GlueLang)

# GlueLang: Good enough Language for Unix Environment

A programming language that has a strong nature to be a glue of commands.

# Quick trial

```
~/GIT/GlueLang$ make
~/GIT/GlueLang$ ./glue TEST/pipeline.glue 
5
4
3
2
1
~/GIT/GlueLang$ cat TEST/pipeline.glue 
/usr/bin/seq '1' '5' >>= /usr/bin/tac
```

# Language Specification

The following code is a sample script of GlueLang.

```EXAMPLE/readme.glue
import PATH
 
# not a Haskell code but a glue code
 
# how to write a procedure (function of bash)
proc revten =
	/usr/bin/seq 1 10
	>>= /usr/bin/tail -r
 
# if, else if, else
{
	false
	this.revten
} !> {
	echo 'foo'
} !> {
	echo 'bar'
}
 
# output connection
str s = echo 'foofoo' >> echo 'barbar'
echo s

# tmpfile
file f = seq 1 10 >>= grep 5
cat f

# where
diff a b
        where file a = seq 1 10
              file b = seq 1 10
```

# Licence

MIT Licence is applied to. See LICENSE.md.

# Document (in Japanese)

* https://ryuichiueda.github.io/GlueLangDoc_ja/
