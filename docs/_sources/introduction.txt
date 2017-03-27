==================================
Introduction
==================================

GlueLang
==================================

 GlueLang is a shell scripting language not for interaction but scripting. In GlueLang, we try realizing following things:

*  to clear the role of shell

    * We think that shell scripting languages exist for description of process handling.
      It is not for implementing program in a process. However, existing shells
      look like imitations of programming languages for single process programming.
      We think that various gimmicks in shell scripting languages for imitation confuse beginner
      Unix and Linux users.

* to be convenient

    * We add the following features for the shell:

      * temporary files that can be created and destroyed easily
      * friendly error messages 
      * structured exit status numbers
      * indifference toward the SIGPIPE signal
      * ...

Setup
==================================

Environment
----------------------------------

GlueLang is tested on

* macOS Sierra, and 
* Ubuntu 14.04 on `Travis CI <https://travis-ci.org/ryuichiueda/GlueLang>`_.

Installation
----------------------------------

 This is a procedure.

.. code-block:: bash
        :linenos:

        $ git clone https://github.com/ryuichiueda/GlueLang.git
        $ cd GlueLang
        $ make
        $ sudo make install

And check the sushi and beer.

.. code-block:: bash
        :linenos:

	$ glue --version
	GlueLang v0.1.1 🍣 🍺
	
	Copyright (C) 2017 Ryuichi Ueda
	Released under the MIT License.
	https://github.com/ryuichiueda/GlueLang

LICENSE
==================================

GlueLang is distributed with MIT license.
