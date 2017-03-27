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
