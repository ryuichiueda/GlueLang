=================================
Background processes
=================================

A backgound process in GlueLang has its name and is handled with the name. A job, which is composed of some pipelines connected by ``>>, !>`` and ``?>``, can be named when it runs background. In the following example, three jobs, named ``a, b`` and ``c`` are invoked at background.

.. code-block:: bash
	:linenos: 

	$ cat jobs.glue 
	import PATH
	
	sleep 3 >> echo 'a' >>= sed 's/./&&/' & a
	sleep 2 >> echo 'b' >>= sed 's/./&&/' & b
	sleep 1 >> echo 'c' >>= sed 's/./&&/' & c
	
	in.wait a b c
	echo 'd'
	
	$ glue jobs.glue 
	cc
	bb
	aa
	d
	
As shown in this example, a background job is defined with the ``& <name>`` symbol at the end of a job. We can wait each job in the script with ``in.wait`` command. 


* The prefix ``in`` means that this ``wait`` command is an internal command. However, it will be changed or erased in future.
