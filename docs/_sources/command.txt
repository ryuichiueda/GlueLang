====================================
Call of commands
====================================


Simple call
====================================

 This is the most simple example for calling a command. The sample files are put in the ``examples`` directory in the repository.

.. code-block:: bash
        :linenos:

	$ cat command.glue 
	/bin/echo 'abc'
	$ ../glue command.glue 
	abc

Quotation of arguments
====================================

 The arguments should be quoted by the single quotation. A string without quotation is regarded as a variable.
	
.. code-block:: bash
	:linenos:
	
	$ cat command_error.glue 
	/bin/echo abc
	$ glue command_error.glue 
	Execution error at line 1, char 11
		line1: /bin/echo abc
		                 ^
	
		Variable abc not found
		process_level 0
		exit_status 3
		pid 31879
	ERROR: 3

 Exceptionally, quotations for the string that starts from ``-`` and integer variables can be omitted.

.. code-block:: bash
	:linenos:

	$ cat command_no_quote.glue 
	/bin/echo -n 10
	$ glue ./command_no_quote.glue 
	10$


Paths
====================================

Import
------------------------------------

 In the default condition, GlueLang does not use PATH, which is the environment variable for pointing directories of commands. A directory should be registered with ``import`` sentence. The following is an simple example.

.. code-block:: bash
 :linenos:

 $ cat import.glue 
 import /bin/ as b
 b.ls

In this example, the path ``/bin/`` is referred as ``b`` at Line 3.

 We think that this way is useful

* to write and store the scripts with a rigid manner, and
* to use command packages without changing PATH.

Import of all directories in PATH
------------------------------------

 To write a script casually, we prepare ``import PATH``. When ``import PATH`` is written at the header part of a script, commands under the direcotries in PATH can be used without any prefix. The following is a workable script.

.. code-block:: bash
 :linenos:

 $ cat import_path.glue 
 import PATH

 ls -l
