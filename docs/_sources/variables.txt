==================================
Variables
==================================

"File type variables" and "string type variables" are available in GlueLang. Basically, they are immutable.

File type variables
==================================

Temporary files are treated as variables in GlueLang. An example is shown below.

.. code-block:: bash
        :linenos:
	
	$ cat file1.glue 
	import PATH
	file f = seq 10        # defining a file "f"
	head -n 1 f            # using "f" with head as a file

	$ glue file1.glue 
	1

This file is deleted at the end of the procedure of this script. We therefore don't need to care the aftermath of the use of temporary files. The directory in which temporary files are placed is fixed to ``/tmp/``. It should be flexible and safe in future.

When you want to change a file variable to "a real file," please use ``mv(1)``. This is an example.

.. code-block:: bash
        :linenos:

	$ cat file2.glue 
	import PATH
	
	file f = seq 3
	mv f './a'
	
	$ glue file2.glue 
	$ cat a
	1
	2
	3
	
String type variables
==================================

A string type variable is defined with ``str <name>``. Here is an example.

.. code-block:: bash
        :linenos:

	$ cat str1.glue 
	import PATH
	
	str s = seq 2
	echo s
	echo s
	
	$ glue str1.glue 
	1
	2
	1
	2
	
String type variables are stored on DRAM allocated to the process of glue.

Where phrase
==================================

When we want to use variables for one procedure, they can be concealed in a "where phrase." In the following example, arguments for ``grep(1)`` are defined in a where phrase.

.. code-block:: bash
	:linenos:

	$ cat where2.glue 
	import PATH
	
	grep re f
	  where          
	    str re = echo 1
	    file f = seq 1 10

	$ glue where2.glue 
	1
	10

These variables are not visible outside of the phrase. In the following script, an error occurs.

.. code-block:: bash
	:linenos:

	$ glue where2_error.glue 
	1
	10
	Execution error at line 8, char 6
		line8: echo re
		            ^
	
		Variable re not found
		process_level 0
		exit_status 3
		pid 40165
	ERROR: 3
	
When two variables with the identical name are defined in/out a where phrase, the inside one is referred at the command that has the where phrase. Here is an example.

.. code-block:: bash
	:linenos:

	$ cat where_scope3.glue 
	import PATH
	
	str a = 'out'
	
	echo a
	  where str a = 'in'
	
	$ glue where_scope3.glue 
	in


