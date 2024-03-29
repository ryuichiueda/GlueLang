[![Build Status](https://travis-ci.org/ryuichiueda/GlueLang.svg?branch=master)](https://travis-ci.org/ryuichiueda/GlueLang)

# GlueLang: Good enough Language for Unix Environment

A programming language that has a strong nature to be a glue of commands.

* User's manual: http://ryuichiueda.github.io/GlueLang/

# Install

Run `make install` then add `~/.glue/bin` to PATH.

# Quick trial

```
~/GIT/GlueLang$ make
~/GIT/GlueLang$ ./glue examples/pipeline.glue
5
4
3
2
1
~/GIT/GlueLang$ cat examples/pipeline.glue 
/usr/bin/seq '1' '5' >>= /usr/bin/tac
```

# Language Specification

The examples are located in the ./examples/ dir.


# License

MIT License is applied to. See LICENSE.md.

# Document

* English: http://ryuichiueda.github.io/GlueLang/
* Japanese: https://ryuichiueda.github.io/GlueLangDoc_ja/

# Contributors

See CONTRIBUTORS.


© 2014-2022 Ryuichi Ueda

<!-- * Language Reference: https://ryuichiueda.github.io/GlueLangArch_ja/ -->
