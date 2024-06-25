# What is Minishell?
Minishell is a simple implementation of a Unix shell designed to execute commands in an environment similar to an operating system. It provides a command-line interface where users can input and execute commands, manage processes, and work with files and directories.

# What is it for?
Minishell serves as an educational and development tool that allows users to understand and interact with the basic functionalities of a Unix-like shell. It can be used to learn about command parsing, process management, and the execution of built-in commands.

# Features
Built-in Commands
Minishell includes several built-in commands that provide basic functionality. Some of these commands include:


* cd: Change the current directory.

* echo: Display a line of text.

* exit: Exit the shell.

* pwd: Print the current working directory.

* env: Display the environment variables.

* export: Set or export environment variables.

* unset: Remove environment variables.

* history: List or clear the command history

# Wildcards
Minishell supports the use of wildcards for pattern matching in filenames and directories. Below are the different types of wildcards available:

* \* (Asterisk): Matches zero or more characters.

* ? (Question mark): Matches exactly one character.
  
* `[ ]` (Square brackets): Matches any one of the enclosed characters. A range of characters can be specified using a hyphen (-), for example, [a-z] matches any lowercase letter.

* [! ] (Negated square brackets): Matches any character not enclosed. For example, [!a-z] matches any character that is not a lowercase letter.

# Environment Variables
Minishell supports environment variables, allowing users to set, export, and unset variables that can be used by the shell and its subprocesses.

# Shell Variables
Shell variables are supported to store data that can be used within the shell session. These variables can be set and accessed similarly to environment variables but are not exported to subprocesses by default.

# Subshell
Minishell supports the execution of commands in a subshell using parentheses (). This allows commands to be executed in a separate shell environment.

# Command Substitution with $()
Minishell supports command substitution using the $() syntax. This feature allows the output of a command to be captured and used as an argument in another command. For example:

# Command Separators
Minishell supports various command separators to control the flow of command execution:


* ; : Sequential execution. Commands separated by ; are executed one after another.

* &&: Conditional AND. The second command is executed only if the first command succeeds.

* ||: Conditional OR. The second command is executed only if the first command fails.


# Redirection and Pipes
Minishell allows for input and output redirection using >, <, >>, <<, <<< and supports pipes | for chaining commands.

# Signal Handling
Minishell handles common signals such as SIGINT (Ctrl+C) and SIGQUIT (Ctrl+\) to ensure proper behavior during command execution.


# Notes
Minishell is designed as a simplified shell and may not support all features of a full-fledged Unix shell.
Error handling and edge cases should be carefully considered during implementation and usage.
This project is a great opportunity to understand the inner workings of a shell, including parsing, process management, and command execution.


Kobayashi82 (vzurera-)
