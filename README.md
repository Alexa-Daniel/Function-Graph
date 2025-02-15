# Function-Graph
A C++ console application which displays the graph of a function.

# How to use
Download and unzip the repository, then run "Graficul Functiei.exe".

An interval also needs to be specified and the zoom distance, as the graph can be zoomed in, out, moved to the left and right. \
Every function which is input through the application will be saved in the "functii.txt" file and will be visible in the "History" tab. \
Functions can also be input directly in the "functii.txt" file, but need to follow the structure: \
\
'*Number of functions in this set*' \
'*Number of the current set*'. '*function1*' '*beginning of interval1*' '*end of interval1*' '*zoom distance1*' (Number of the current set should be 1. if this is the first set of functions or number of previous sets of functions + 1) \
(if number of functions is >1 then type each function of a new line) \
'*function2*' '*beginning of interval2*' '*end of interval2*' '*zoom distance2*' \
'*function3*' '*beginning of interval3*' '*end of interval3*' '*zoom distance3*' \
... \
'*functionN*' '*beginning of intervalN*' '*end of intervalN*' '*zoom distanceN*' 

Each function will be represented on its respective interval.
