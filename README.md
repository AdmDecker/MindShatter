**Please report any bugs or issues to Adam Decker**

MindShatter is a free, all-purpose Brainfuck interpreter.
It supports both file and commandline input and output.

By default, MindShatter will start in commandline mode for both input and output.
The memory tape used is currently limited to 30,000 bytes and will loop on boundaries.



-------------------------------------------------------------------------------------------------------------
##Compile Instructions:

g++:
    g++ -static main.cpp -o Build/MindShatter.exe
	
-------------------------------------------------------------------------------------------------------------


-------------------------------------------------------------------------------------------------------------
##Commandline Arguments:

######--Implemented--

-i \<file\>: Run with input mode set to file and specify the file to run

-o \<file\>: Run with output mode set to file and specify the file to output to

######--Pending--

-d : Start with dynamic memory allocation for unlimited memory tape

-a \<bytes\>: Allocate <bytes> amount of memory for the tape to run on. Must be an integer greater than 0.

-------------------------------------------------------------------------------------------------------------


-------------------------------------------------------------------------------------------------------------
##Supported Brainfuck Operations:

**+** Increment the value at the pointer by one

**\-** Decrement the value at the pointer by one

**\>** Move the pointer one unit forward on the tape

**<** Move the pointer one unit back on the tape

**,** Input a value through the commandline

**.** Output (display) the value at the pointer

**\[** Start a loop

**\]** If the value at the pointer is 0, go back to the start of the loop. Otherwise, exit the loop

--------------------------------------------------------------------------------------------------------------


--------------------------------------------------------------------------------------------------------------
##Current issues:

-Input from using the input operator ',' is not cleaned, and can cause the program to act sporadically

 if it is not proper.
 
-Executable could be made smaller if dependencies to std::vector and std::string were removed

--------------------------------------------------------------------------------------------------------------