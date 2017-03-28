===============================
Blocks and procedures
===============================

To create a block is another way for combining commands. Blocks are also used for implementing loops. Procedures are derived from the concept of blocks. 

Do blocks
===============================

A do block combines multiple commands. Here is an example though it does not give any effect on the procedure.

.. code-block:: bash
        :linenos:

	$ cat do_block.glue 
	import PATH
	
	do
	  echo 'a'  
	  echo 'b'  
	$ glue do_block.glue 
	a
	b
	
This ``do`` is regarded as a command that includes the two echoes, which are written with two space indents. The indents must be aligned as done in Python. We recommend two space indents for GlueLang.

Since a do block is regarded as one command, the following procedure consists.

.. code-block:: bash
        :linenos:

	$ cat do_block_pipe.glue 
	import PATH
	
	do
	  echo 'a'  
	  echo 'b'  
	
	>>= cat -n
	$ glue do_block_pipe.glue 
	     1	a
	     2	b
	
A do block can receive data through a pipe. We give an example.

.. code-block:: bash
        :linenos:

	$ cat do_block_pipe2.glue 
	import PATH
	
	seq 3 >>= do
	  echo '+++'
	  cat
	  echo '+++'
	uedamb:examples ueda$ glue do_block_pipe2.glue 
	+++
	1
	2
	3
	+++


While blocks
===============================

Simple use
-------------------------------

Implementation of while blocks are derived from that of do blocks. In a while block, commands in the block are repeated until a command gives a non-zero exit status. In the case of the following example, the indented commands are repeated while the string ``t`` is not zero. 

.. code-block:: bash
        :linenos:

	$ cat simple_while.glue 
	import PATH
	 
	while
	  str t = date '+%s' >>= awk '{print $1%5}'
	  echo t
	  test t -ne 0
	  sleep 1
	 
	echo 'end'
	
	$ glue simple_while.glue 
	1
	2
	3
	4
	0
	end

When an error that is not a command error occurs, not the while loop but the script stops immediately. The following is a case where the command ``ech`` is not found.

.. code-block:: bash
        :linenos:

	$ cat simple_while_error.glue 
	import PATH
	 
	while
	  str t = date '+%s' >>= awk '{print $1%5}'
	  ech t          # There is no ech command!!!
	  test t -ne 0
	  sleep 1
	 
	echo 'never come here'
	
	$ glue simple_while_error.glue 
	Parse error at line 2, char 1
		line2: ech t          # There is no ech command!!!
		       ^
	
		Command ech not exist
        ###snip###
	ERROR: 2

Combination with ``?>``
-------------------------------

When a use of ``?>`` in a while block makes it possible to implement B shell like while loops. This is an example.

.. code-block:: bash
        :linenos:
	
	$ cat while_do.glue 
	import PATH
	 
	while
	  str t = date '+%s' >>= awk '{print $1%5}'
	  test t -ne 0 !> break ?> do
	    echo t
	    sleep 1
	 
	echo 'end'
	
	$ glue while_do.glue 
	2
	3
	4
	end
	
The commands before ``!>`` are regarded as conditions for continuing this while loop. The ``break`` in this example is an internal command. This command is used for breaking ``while`` or ``foreach``.

* memo: ``!> break ?>`` should be reduced into one symbol (``<?>`` for example).

Foreach blocks
===============================

In sh or bash, ``while`` also receives date via pipeline. In GlueLang, this feature is implemented as ``foreach``. Here is an example.

.. code-block:: bash
        :linenos:

	$ cat foreach_simple.glue 
	import PATH 
	
	seq 1 3 >>= foreach
	  echo '@' argv[1]
	
	$ glue foreach_simple.glue 
	@ 1
	@ 2
	@ 3

Each line (1, 2, and 3 in this case) is read to the foreach block one by one. In the block, ``i`` th word of the input line can be referred as ``argv[i]``.

Procedures
===============================

Procedure definition with do block
----------------------------------------------

We can define a procedure by using a do block. As in the following example, we put ``proc <name> =`` before a do block for definition of a procedure. To call a procedure, we need to put a prefix ``this.`` to the name of the procedure.

.. code-block:: bash
	:linenos:

	$ cat proc2.glue 
	proc hoge = do
	  /bin/echo 'abc' >>= /usr/bin/rev
	  /bin/echo 'OK'
	
	this.hoge
	
	$ glue proc2.glue 
	cba
	OK

Arguments
----------------------------------------------

As done in the foreach blocks, ``argv[i]`` can be used in a procedure for refering arguments given by the procedure. We give an example.

.. code-block:: bash
	:linenos:

	$ cat proc_args.glue 
	import PATH
	
	proc hoge = do
	  /bin/echo argv[1] >>= /usr/bin/rev
	  /bin/echo argv[2]
	
	this.hoge 'abc' 'def'

	$ glue proc_args.glue 
	cba
	def


Procedure definition without do block
----------------------------------------------

A sequence of commands can be directory substituted into a procedure without do block. We show an example below.

.. code-block:: bash
	:linenos:

	$ cat proc1.glue 
	import PATH
	
	proc f = echo 'HELLO' >> echo 'HELLO' >>= rev
	this.f

	$ glue proc1.glue 
	HELLO
	OLLEH


Scope
===============================

Blocks and procedures are executed in subshells, which are generated by the fork system call. After the system call, a subshell inherits all resources from the main process. Therefore, all variables in the main process can be used in the subshell. Inversely, variables defined in the subshell cannot be used in the main process. In the following example, ``f`` cannot be used in the main process.


.. code-block:: bash
	:linenos:

	$ cat do_block_scope.glue 
	import PATH
	
	do
	  file f = echo 'a'  
	
	cat f    # error

	$ glue do_block_scope.glue 
	Execution error at line 6, char 5
		line6: cat f    # error
		           ^
	
		Variable f not found
		process_level 0
		exit_status 3
		pid 57712
	ERROR: 3
	
To use the data in a subshell, a ``cp`` or ``mv`` is useful. This is a workable example.

.. code-block:: bash
	:linenos:

	$ cat do_block_scope3.glue 
	import PATH
	
	do
	  file f = echo 'a'  
	  mv f '/tmp/hoge'
	
	cat '/tmp/hoge'
