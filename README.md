# Minishell


## Overview

Minishell is a simple, interactive shell program inspired by bash. It supports basic command execution, piping, redirection, and built-in commands, providing an educational opportunity to understand processes, file descriptors, and Unix system calls.

## Features

    Custom command prompt.
    Command history.
    Command execution with PATH resolution.
    Single and double quote handling.
    Input and output redirection.
    Command piping.
    Environment variable handling.
    Built-in commands (echo, cd, pwd, export, unset, env, exit).
    Signal handling for ctrl-C, ctrl-D, and ctrl-\.

## Installation

### Prerequisites

    Unix-like operating system (Linux, macOS).
    GCC compiler.
    Make utility.
    libft library.

### Compilation

Clone the repository and compile the project:

    git clone https://github.com/MarSonyTM/minishell.git
    cd minishell
    make

This will generate an executable named minishell.

## Usage

Launch the shell with:

    ./minishell

### Commands and Features

    Prompt: Displays a prompt and waits for user input.
    History: Maintains a history of commands entered.
    Command Execution: Executes commands with path resolution.
    Quotes: Handles single (') and double (") quotes for escaping meta-characters.
    Redirection:
        < file: Redirect standard input from file.
        > file: Redirect standard output to file.
        >> file: Append standard output to file.
        << delimiter: Here-document; read input until delimiter is found.
    Pipes:
        command1 | command2: Pipe the output of command1 to command2.
    Environment Variables: Supports environment variable expansion.
    Special Variables: $? expands to the exit status of the last executed command.
    Built-in Commands:
        echo [-n] [string...]: Display a line of text.
        cd [directory]: Change the current directory.
        pwd: Print the current working directory.
        export [name[=value] ...]: Set environment variables.
        unset [name ...]: Unset environment variables.
        env: Display the environment variables.
        exit [n]: Exit the shell with a status of n.

### Example

    $ ./minishell
    minishell> echo "Hello, World!"
    Hello, World!
    minishell> ls -l | grep minishell
    -rwxr-xr-x  1 user  group  4096 Jul  4 12:34 minishell
    minishell> exit

### Signal Handling

    ctrl-C: Displays a new prompt on a new line.
    ctrl-D: Exits the shell.
    ctrl-\: Does nothing.

## Error Handling

The shell displays appropriate error messages and continues running in case of errors.

### Examples

    minishell> unknowncommand
    minishell: command not found: unknowncommand
    minishell> cat non_existent_file
    minishell: no such file or directory: non_existent_file

## Makefile Rules

    make or make all: Compile the program.
    make clean: Remove object files.
    make fclean: Remove object files and the executable.
    make re: Recompile the project from scratch.
