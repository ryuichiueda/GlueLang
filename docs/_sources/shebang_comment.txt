===================================
Shebang, comments, and Arguments
===================================

Shebang
===============================

When the glue command is installed in a directory, a script has a shebang as follows.

.. code-block:: bash
        :linenos:

	$ cat shebang.glue 
	#!/usr/local/bin/glue
	
	/bin/echo 'shebang'
	$ chmod +x shebang.glue
	$ ./shebang.glue 
	shebang
	
The directory for the glue command can be checked by the following command.

.. code-block:: bash
        :linenos:

        $ which glue
        /usr/local/bin/glue

Comments
===============================

The words after ``#`` are regarded as comments.

.. code-block:: bash
        :linenos:

	$ cat comment.glue 
	/bin/echo 'aaa'  #This is a comment.
	/bin/echo 'a#aa' #The # in the literal is just a normal charcter.
	
	$ glue comment.glue 
	aaa
	a#aa
	

* bug

  * Comments in a pipeline or a job are not treated as comments, and yield errors in the current implementation.


Arguments
===============================

The glue command receives arguments from the command line.

.. code-block:: bash
        :linenos:

	$ cat args1.glue 
	/bin/echo argv[1] argv[2]
	$ glue ./args1.glue abc def
	abc def

At a short of arguments, an error occurs.


.. code-block:: bash
        :linenos:

	$ glue ./args1.glue a
	Execution error at line 1, char 19
		line1: /bin/echo argv[1] argv[2]
		                         ^
	
		Array index out of range (pos: 2)
		process_level 0
		exit_status 3
		pid 76075
	ERROR: 3
	
		
