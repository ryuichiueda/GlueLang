[![Build Status](https://travis-ci.org/ryuichiueda/GlueLang.svg?branch=master)](https://travis-ci.org/ryuichiueda/GlueLang)

# GlueLang: Good enough Language for Unix Environment

A programming language that has a strong nature to be a glue of commands.

* User's manual: http://ryuichiueda.github.io/GlueLang/

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

The examples are located in the ./exmaples/ dir.


# License

MIT License is applied to. See LICENSE.md.

# Document (in Japanese)

* User manual: https://ryuichiueda.github.io/GlueLangDoc_ja/
* Language Reference: https://ryuichiueda.github.io/GlueLangArch_ja/
