# CS685 Endsem - Part 1

## Requirements

To code file requires the following libraries to be installed - 
	
	1. scipy
	2. numpy
	3. pandas
	4. seaborn		(optional; for plotting)
	5. matplotlib	(optional; for plotting)

## Instructions to run the code

If all the above libraries are installed, then run the file as follows

	> python main.py [-t 45] [-p]

The ouliers idx will be written to the file "output_150259.csv" in the parent
folder.

*Arguments*
	1. The argument "-t|--threshold <int>" specifies the number of outliers to output
	   (default is 45) and the argument 
	2. The argument "-p|--plotting" specifies whether to generate plots or not
