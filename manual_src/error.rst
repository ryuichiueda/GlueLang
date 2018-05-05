========================================
Toward errors
========================================

We think that conventional shells have poor functions toward errors. GlueLang will be enpowered to have strong error handling features. As such the feature, GlueLang does not return the exit status given by the last command differently from bash or sh. This nature will help us to detect the cause of errors.

Exit statuses
========================================

The following table shows the list of exit statuses.


.. list-table:: Exit statuses
   :widths: 40 20
   :header-rows: 1

   * - Statuses
     - Numbers
   * - A command gives non-zero exit status.
     - 1
   * - A command cannot be found from the shell.
     - 2
   * - An argument is used without definition.
     - 3
   * - The folk system call causes an error.
     - 4
   * - The current directory is not obtained.
     - 5
   * - ``>>`` is connected after ``?>`` 
     - 6
   * - An unknown token is written in the script.
     - 7
   * - An error occured in a then (``?>``) part.
     - 8
   * - Unknown parse error.
     - 128
   * - End by a signal (not implemented yet)
     - 128 + the signal number

Error messages
========================================

When an error occurs, the script stop immediately and gives a message form the standard error. Here is an example.

.. code-block:: bash
	:linenos:

	$ glue simple_error.glue
	(snip)
	Execution error at line 3, char 1
		line3: diff
		       ^
	
		Command error
		process_level 0
		exit_status 2
		pid 95348
	ERROR: 1
	
In the message, we can see the part where the error occurs, the cause of the error, the level of shells (how deep is the subshell is invoked), the exit status from the command or something that causes the error, and the process id. Finally, the script gives the exit status of the script with a red string.

When some subshells are invoked, the error message is given by each subshell. This is an example.

.. code-block:: bash
	:linenos:

	$ cat error_nest.glue 
	import PATH
	
	proc f = diff
	
	this.f
	
	$ glue error_nest.glue 
	/usr/bin/diff: missing operand after `/usr/bin/diff'
	/usr/bin/diff: Try `/usr/bin/diff --help' for more information.
	Execution error at line 1, char 1
		line1: diff
		       ^
	
		Command error
		process_level 1
		exit_status 2
		pid 95768
	Execution error at line 5, char 1
		line5: this.f
		       ^
	
		Command error
		process_level 0
		exit_status 1
		pid 95767
	ERROR: 1
	
