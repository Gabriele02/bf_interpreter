# BfInterpreter
**A one-pass BrainFuck interpreter**

# Story
Some time ago, I was surfing the internet, minding my own buisiness when I stumbled across this strange language: BrainFuck.
At first I was scared by it's appearence but, as I started to dig deeper on how it works, I found my self more and more comfortable with it. 
I thought that to fully understand how this crazy thing works I should have wrote an interpreter for it, so here we are, I present to you BfInterpreter!

# Features
- Interactive mode
- Fast and clean code
- Error reporting
- Debug functions

# Interactive mode
Fire up the interpreter without any argument and you'll be able to interact with the interpreter.
Open it with a file as an argument and it will interpret it or report an error.

# Fast and clean code
I tryied to make the interpreter's code as fast as possible while trying to keep it as readable as possible.

# Error reporting
The interpreter can report **missing square brackets** and **illegal characters**.

**NOTE**: in interactive mode all characters except "+-<>[].," and the debug commands are **forbidden**.<br/>
**NOTE**: while the interpreter interprets a file, every character but "+-<>[].," is treated as a **comment**.

# Debug functions
| Function     |                                          Description                                     |
| -------------|------------------------------------------------------------------------------------------|
| print        | prints the content of the current memory cell as an integer  (0-255)                     |
| printChar    | prints the content of the current memory cell as a character (exactly as the . command)  |
| printPtr     | prints the memory pointer                                                                |
| printMem     | prints the content of the memory aroud the memory pointer (+/-5)                         |

To use a debug function, just write it and press enter

# Usage
Compile it using the **compile.sh** script.
Run it in interactive mode or with an argument.
Type exit to quit.
