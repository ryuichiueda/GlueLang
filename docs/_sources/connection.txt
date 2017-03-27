=============================
Connections of commands
=============================

Pipeline
=============================

 The symbol of a pipe is ``>>=`` in GlueLang. Though ``|`` which is used in conventional shells is easier to be typed than ``>>=``, we thing that it is inconspicuous. Moreover, the role of pipelines on shells is similar to a sequence of functions connected by ``>>=`` in Haskell. This is an example.

.. code-block:: bash
	:linenos: 

	$ cat pipeline2.glue 
	import PATH
	echo 'abc' >>= rev

	$ glue pipeline2.glue 
	cba

We can give a pipeline some returns as follows.

.. code-block:: bash
	:linenos: 

	$ cat pipeline2_fold.glue 
	import PATH

	echo 'efg'
	>>= rev

	echo 'hij' >>=
	rev

	$ glue pipeline2_fold.glue 
	gfe
	jih

And
=============================

 ``&&`` in B shells is represented by ``>>`` in GlueLang.

.. code-block:: bash
        :linenos:

	$ cat and.glue 
	/bin/echo 'a' >> /bin/echo 'b' >> /bin/echo 'c'
	$ glue ./and.glue 
	a
	b
	c


