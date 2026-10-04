# 🐚 Minishell

## 🧠 Overview

**Minishell** is a custom Unix shell written entirely in C, inspired by the behavior of **Bash**.

Instead of simply executing commands, the project recreates the core mechanisms behind a real shell: reading user input, tokenizing and parsing commands, expanding variables, managing processes, connecting pipelines, handling redirections, executing programs, and reacting correctly to Unix signals.

The goal is to understand what actually happens between typing:

```bash
cat file.txt | grep hello > result.txt
```

and the operating system launching and connecting those processes.

This project focuses heavily on:

- Process creation and management
- File descriptors
- Pipes and inter-process communication
- Command parsing
- Lexical analysis and tokenization
- Environment variable expansion
- Input/output redirections
- Signal handling
- Dynamic memory management
- Unix system calls
- Shell architecture

---

## 📂 Project Structure

```text
.
├── Makefile
├── include/
│   └── minishell.h
│
├── builtins/
│   ├── cd.c
│   ├── echo.c
│   ├── env.c
│   ├── exit.c
│   ├── export.c
│   ├── export2.c
│   ├── pwd.c
│   ├── unset.c
│   └── b_utils*.c
│
├── env/
│   ├── env.c
│   └── env_utils.c
│
├── tokinizer/
│   ├── tokenizer.c
│   ├── tokenizer2.c
│   └── tokenizer3.c
│
├── parser/
│   ├── parser.c
│   ├── parser2.c
│   ├── parser_validate.c
│   └── parser_validate2.c
│
├── expander/
│   ├── expander.c
│   └── expander_utils*.c
│
├── excution/
│   ├── exec_utils.c
│   ├── paths.c
│   ├── pipework.c
│   ├── pipework_utils.c
│   ├── pipework_utils2.c
│   ├── redir.c
│   ├── heredoc.c
│   └── heredocutils.c
│
├── promptandsigs/
│   ├── ctrl.c
│   └── fresh.c
│
├── genutils/
│   ├── errors.c
│   └── utils*.c
│
└── libft/
    ├── Makefile
    ├── libft.h
    └── ft_*.c
```

Each part of the shell is separated into its own subsystem, keeping parsing, expansion, execution, environment management, builtins, and signals independent and easier to maintain.

---

## 🚀 Compilation

Compile Minishell:

```bash
make
```

This generates:

```text
minishell
```

Run it:

```bash
./minishell
```

Clean object files:

```bash
make clean
```

Remove object files and the executable:

```bash
make fclean
```

Recompile everything:

```bash
make re
```

Compilation uses:

```text
-Wall -Wextra -Werror
```

and links against the **Readline** library for interactive command input and history.

---

## ⚙️ How It Works

A command entered into Minishell goes through several stages before anything is executed:

```text
User Input
    ↓
Tokenizer
    ↓
Syntax Validation
    ↓
Variable Expansion
    ↓
Parser
    ↓
Command Structures
    ↓
Redirections / Pipes
    ↓
Process Creation
    ↓
Command Execution
    ↓
Exit Status
```

For example:

```bash
cat $FILE | grep "hello world" >> output.txt
```

Minishell must:

1. Read the input.
2. Recognize words, quotes, pipes and redirections.
3. Expand `$FILE`.
4. Build separate command structures.
5. Create a pipe.
6. Fork the required processes.
7. Redirect their input and output.
8. Locate executables using `$PATH`.
9. Execute the programs.
10. Wait for the processes.
11. Store the final exit status.

All of this is handled manually using C and Unix system calls.

---

## 🔤 Tokenization

The tokenizer converts raw shell input into meaningful tokens.

Supported token types include:

```text
WORD
PIPE
R_IN
R_OUT
APPEND
HEREDOC
```

For example:

```bash
cat file.txt | grep hello >> result.txt
```

is interpreted conceptually as:

```text
WORD      → cat
WORD      → file.txt
PIPE      → |
WORD      → grep
WORD      → hello
APPEND    → >>
WORD      → result.txt
```

This gives the parser a structured representation of the user's input instead of working directly with a raw string.

---

## 🧩 Parsing & Syntax Validation

After tokenization, Minishell validates the command syntax and constructs command structures used by the execution engine.

It detects invalid input such as:

```bash
| ls
```

```bash
ls |
```

```bash
cat >
```

and reports shell-style syntax errors instead of attempting to execute malformed commands.

Commands, arguments and redirections are then organized into linked structures that can be processed by the executor.

---

## 💲 Environment Variable Expansion

Minishell performs shell-style environment expansion.

Example:

```bash
echo $USER
```

```bash
echo $HOME
```

```bash
echo $PATH
```

It also supports the special variable:

```bash
$?
```

which contains the exit status of the previously executed command.

Example:

```bash
ls
echo $?
```

Expansion also respects quoting rules.

### Single Quotes

```bash
echo '$USER'
```

Variable expansion is disabled.

### Double Quotes

```bash
echo "$USER"
```

Variables are expanded while preserving the quoted string.

---

## 🔗 Pipes

Minishell supports command pipelines using:

```text
|
```

Example:

```bash
ls -la | grep ".c"
```

Multiple commands can also be chained:

```bash
cat file.txt | grep hello | wc -l
```

Internally this requires:

- `pipe()`
- `fork()`
- `dup2()`
- Closing unused file descriptors
- Executing multiple processes
- Tracking process IDs
- Waiting for child processes
- Preserving the correct final exit status

Each command runs with its standard input and output connected to the appropriate side of the pipeline.

---

## 📥 Redirections

Minishell supports the major shell redirection operators.

### Input

```bash
command < file
```

Redirects the contents of a file to standard input.

### Output

```bash
command > file
```

Redirects standard output to a file and overwrites its previous contents.

### Append

```bash
command >> file
```

Redirects standard output while preserving the existing file contents.

### Heredoc

```bash
command << EOF
hello
world
EOF
```

Reads input until the specified delimiter is encountered.

Redirections are applied using file descriptors and `dup2()` before command execution.

---

## 🔎 Command Execution

External commands are located using the shell's `PATH` environment variable.

For example:

```bash
ls
```

Minishell searches directories such as:

```text
/usr/local/bin
/usr/bin
/bin
```

until an executable is found.

Commands can also be executed directly through paths:

```bash
/bin/ls
```

or:

```bash
./program
```

Execution is handled using:

```text
fork()
execve()
waitpid()
access()
```

giving Minishell direct control over process creation and execution.

---

## 🛠 Built-in Commands

Some commands cannot simply be launched as external programs because they need to modify the shell itself.

Minishell implements the required shell builtins internally.

### echo

```bash
echo Hello World
echo -n Hello
```

Prints arguments to standard output with support for the `-n` option.

### cd

```bash
cd directory
```

Changes the shell's current working directory.

### pwd

```bash
pwd
```

Displays the current working directory.

### export

```bash
export NAME=value
```

Creates or updates environment variables.

### unset

```bash
unset NAME
```

Removes variables from the environment.

### env

```bash
env
```

Displays the current environment.

### exit

```bash
exit
```

Terminates Minishell using the appropriate exit status.

---

## 🌍 Environment Management

Instead of relying entirely on the original `envp` array, Minishell converts the environment into its own linked-list representation.

Conceptually:

```c
typedef struct s_env
{
    char            *key;
    char            *value;
    struct s_env    *next;
}   t_env;
```

This allows builtins such as:

```text
export
unset
cd
```

to dynamically modify the shell's environment while it is running.

The environment can then be converted back into an array when required by `execve()`.

---

## ⚡ Signal Handling

An interactive shell must behave correctly when receiving keyboard signals.

Minishell handles:

### Ctrl-C — `SIGINT`

```text
^C
```

Interrupts the current operation and returns control to the prompt without terminating the shell.

### Ctrl-D — EOF

```text
exit
```

Ends the shell when received on an empty prompt.

### Ctrl-\ — `SIGQUIT`

Handled according to interactive shell behavior.

Signal behavior is adjusted depending on whether Minishell itself or one of its child processes is currently running.

---

## 🧠 Process Management

One of the central challenges of Minishell is controlling multiple processes correctly.

Commands may require:

```text
fork
 ↓
child process
 ↓
redirections
 ↓
execve
```

while pipelines require several processes to exist simultaneously:

```text
Command 1          Command 2          Command 3
   │                   │                  │
   └────── PIPE ───────┴────── PIPE ─────┘
```

Minishell tracks child process IDs, waits for them correctly, and determines the exit status of the final command in a pipeline.

---

## 🔩 Core Unix Functions

The project works directly with Unix/POSIX functionality including:

```text
readline
add_history
malloc
free
write
read
open
close
access
unlink
fork
pipe
dup
dup2
execve
waitpid
sigaction
signal
chdir
getcwd
fcntl
```

These functions provide the low-level building blocks required to recreate shell behavior.

---

## 🎯 Objectives

The project is designed to develop a deeper understanding of:

- How Unix shells interpret commands
- How processes are created with `fork`
- How programs are launched with `execve`
- How processes communicate through pipes
- How file descriptors control input and output
- How command-line syntax can be tokenized and parsed
- How environment variables are stored and expanded
- How builtins modify the state of a running shell
- How Unix signals affect interactive applications
- How exit statuses propagate between commands
- How to prevent file descriptor and memory leaks
- How multiple systems can be combined into one larger C application

---

## 🧪 Testing & Validation

Minishell should be tested against Bash behavior whenever possible.

### Simple Commands

```bash
ls
pwd
echo hello
```

### Arguments & Quotes

```bash
echo "hello world"
echo '$USER'
echo "$USER"
```

### Environment Expansion

```bash
echo $HOME
echo $PATH
echo $?
```

### Pipes

```bash
ls | wc -l
cat Makefile | grep minishell
cat Makefile | grep SRC | wc -l
```

### Redirections

```bash
echo hello > test.txt
cat < test.txt
echo world >> test.txt
```

### Heredoc

```bash
cat << EOF
hello
world
EOF
```

### Builtins

```bash
cd ..
pwd
export TEST=hello
echo $TEST
unset TEST
env
```

### Error Handling

```bash
command_that_does_not_exist
cat nonexistent_file
|
ls |
cat >
```

### Signals

Test:

```text
Ctrl-C
Ctrl-D
Ctrl-\
```

both at the prompt and while commands are running.

Memory and file descriptor leaks should also be checked using tools such as:

```bash
valgrind ./minishell
```

---

## 🏗 What Makes Minishell Different

Unlike smaller C projects where individual functions are implemented independently, Minishell combines multiple low-level systems into a single interactive application.

A single line such as:

```bash
cat input.txt | grep "$USER" >> output.txt
```

can involve:

```text
Input handling
      ↓
Tokenization
      ↓
Quote processing
      ↓
Environment expansion
      ↓
Syntax validation
      ↓
Parsing
      ↓
Command construction
      ↓
Pipe creation
      ↓
Process creation
      ↓
File descriptor duplication
      ↓
Redirection
      ↓
PATH resolution
      ↓
Program execution
      ↓
Process synchronization
      ↓
Exit status handling
```

Minishell is essentially a small command interpreter built from the ground up using **C, Unix processes, file descriptors, signals, and system calls**.

It turns the shell from something you simply use into something you understand.
