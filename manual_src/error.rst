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

When an error occurs, the script stop immediately and gives a message. Here is an example.

.. code-block:: bash

	$ cat simple_error.glue 
	import PATH

	diff

	$ glue ./simple_error.glue
	(snip)
	Execution error at line 3, char 1
		line3: diff
		       ^
	
		Command error
		process_level 0
		exit_status 2
		pid 90120
	ERROR: 2
	
