In this project a command-line parcer is implemented.

The following arguments are supported:
--help | -h | ? - to get information about the program and list available arguments with their functions
--version | -V - to print the version of the program
--list | -l [=list_of_subargs] - to pass the list of arguments to be provided to the program at runtime
--value | -v [=list_of_subargs] - to specify a list of values to proccess

Argument passed several times (in long and/or short form(s)) is proccessed only once and if different subarguments are passed, only the first taken subarguments are taken into consideration.

List of subarguments to an argument should be passed after '=' and separated by commas without spaces.

Arguments in short form can be passed in group, in case of passing subarguments, they are considered as passed to the last argument.

Arguments passed to the program are printed in console, in case of passing unknown arguments or subarguments to unapproriate command, a corresponding error message is printed.