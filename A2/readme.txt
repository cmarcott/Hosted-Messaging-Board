
Christopher Marcotte
0892486

You can type ‘make’ to compile the program
This will create a library libstream.a, and
Two executables addauthor and post. 

There is also a view.py program in the folder

Restrictions:
A restriction is that a text input line cannot start
With the phrases “Sender: “ or “Date: “
Those include the semicolon, the capital first letter,
and the space after the semicolon. For example, you can
Still type “Sender”, “sender”, “Sender:dog”, etc.

The program will automatically just omit any line that
does not follow these guidelines, giving an error message,
rather than exiting.

