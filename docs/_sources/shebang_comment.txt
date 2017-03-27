===============================
Shebang and comments
===============================

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
