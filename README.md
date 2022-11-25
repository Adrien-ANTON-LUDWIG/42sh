# Description 

This is the result of a project shell POSIX implementation in C, following the XCU SCL as closely as possible. 

# Features

Available built-ins : 
- alias/unalias
- echo
- cd
- source
- break/continue
- exit
- shopt
- export

# Compilation
```
$ cd Shell-POSIX
$ mkdir build
$ cd build
$ cmake ..
$ make
```
# Usage

There are two ways to use this program. The first one is with the shell commands in command line, the second one is with the shell commands into a .sh file.

### Method 1

Use the -c option for command.

`$ ./42sh -c "echo example"`

### Method 2

You need to write your shell program into a '.sh' file and to use this file on execution. To try it, you can use the available scripts in the [tests' folder](/tests/scripts/test_scripts).

`$ ./42sh [PATH_TO_SH_FILE]`

# Contributors

- Adrien ANTON LUDWIG
- Pierre PELLETIER
- Adèle PLUQUET
- Florent TAUTY
