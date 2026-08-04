# MyShell

A Unix-like shell written in C as an Operating Systems learning project. The goal of this project is to understand how real shells work internally by implementing process creation, program execution, redirection, pipelines, signals, and job control from scratch using POSIX system calls.

---

# Features

## Command Execution

Execute external programs:

```bash
ls
pwd
sleep 5
```

Implemented using:

- `fork()`
- `execvp()`
- `waitpid()`

---

## Built-in Commands

The following built-ins run directly in the shell process:

```bash
cd
pwd
exit
history
jobs
```

Built-ins do not create child processes.

---

## Command History

Store and display previously executed commands.

Example:

```bash
history
```

---

## Input Redirection

Redirect standard input from a file:

```bash
cat < input.txt
```

Implemented using:

- `open()`
- `dup2()`

---

## Output Redirection

Overwrite a file:

```bash
echo hello > output.txt
```

Append to a file:

```bash
echo hello >> output.txt
```

Implemented using:

- `open()`
- `dup2()`

---

## Pipelines

Supports pipelines of arbitrary length.

Examples:

```bash
ls | wc -l
```

```bash
cat file.txt | grep hello | sort
```

```bash
echo hello | cat | wc -c
```

Implemented using:

- `pipe()`
- `fork()`
- `dup2()`

---

## Background Execution

Run commands without blocking the shell.

Example:

```bash
sleep 30 &
```

Output:

```text
[1] 12345
```

The shell immediately returns to the prompt while the process continues running.

---

## Job Management

View active jobs:

```bash
jobs
```

Example output:

```text
[1] Running sleep 30
```

Jobs are stored internally and tracked by:

- Job ID
- Process ID (PID)
- Command string
- Job state

---

## Job Completion Notifications

Finished background jobs are automatically detected and removed.

Example:

```text
[1] Done                    sleep 30
```

Implemented using:

- `SIGCHLD`
- `waitpid(..., WNOHANG)`

---

## Ctrl+C Support (SIGINT)

Kill the foreground process without terminating the shell.

Example:

```bash
sleep 100
```

Press:

```text
Ctrl+C
```

Result:

```text
MyShell>>
```

Implemented using:

- `SIGINT`
- custom signal handlers

---

## Ctrl+Z Support (SIGTSTP)

Stop the currently running foreground process.

Example:

```bash
sleep 100
```

Press:

```text
Ctrl+Z
```

Result:

```text
[1] Stopped sleep 100
```

Stopped jobs are saved in the job table for later resumption.

Implemented using:

- `SIGTSTP`
- `waitpid(..., WUNTRACED)`

---

# Project Architecture

## Parser

Responsible for:

- Tokenizing input
- Parsing commands
- Parsing pipelines
- Detecting redirection
- Detecting background execution

Files:

```text
parser.h
parser.c
```

---

## Executor

Responsible for:

- Process creation
- Program execution
- Waiting for foreground processes

Files:

```text
executor.h
executor.c
```

---

## Redirection

Responsible for:

```bash
<
>
>>
```

Files:

```text
redirection.h
redirection.c
```

---

## Pipeline

Responsible for:

```bash
cmd1 | cmd2 | cmd3
```

Files:

```text
pipeline.h
pipeline.c
```

---

## Job System

Responsible for:

- Background jobs
- Stopped jobs
- Job tracking
- Job state management

Files:

```text
jobs.h
jobs.c
```

---

## Signal Handling

Responsible for:

- SIGINT
- SIGTSTP
- SIGCHLD

Files:

```text
signals.h
signals.c
```

---

# Supported Shell Features

| Feature | Status |
|----------|----------|
| External Commands | ✅ |
| Builtins | ✅ |
| History | ✅ |
| Input Redirection (`<`) | ✅ |
| Output Redirection (`>`) | ✅ |
| Append Redirection (`>>`) | ✅ |
| Multi-Pipe Support | ✅ |
| Background Jobs (`&`) | ✅ |
| Job Tracking | ✅ |
| Job Notifications | ✅ |
| Ctrl+C | ✅ |
| Ctrl+Z | ✅ |
| Foreground Resume (`fg`) | 🚧 |
| Background Resume (`bg`) | 🚧 |
| Process Groups | 🚧 |
| Quotes | 🚧 |
| Environment Variables | 🚧 |

---

# Concepts Learned

This project demonstrates:

- Process Creation
- Process Hierarchies
- Program Execution
- File Descriptors
- Pipe Communication
- Signal Handling
- Job Control
- Process Synchronization
- Background Processing
- Shell Design

---

# Example Session

```text
MyShell>> sleep 30 &
[1] 4187

MyShell>> jobs
[1] Running sleep 30

MyShell>> sleep 100
^Z
[2] Stopped sleep 100

MyShell>> jobs
[1] Running sleep 30
[2] Stopped sleep 100

[1] Done                    sleep 30
```

---

# Future Work

## Job Control

Implement:

```bash
fg
bg
```

and support process groups with:

```c
setpgid()
killpg()
```

---

## Advanced Parsing

Support:

```bash
echo "hello world"
```

```bash
echo 'hello world'
```

```bash
export NAME=Boaz
echo $NAME
```

---

## Command Chaining

Support:

```bash
pwd ; ls
```

```bash
make && ./app
```

```bash
make || echo failed
```

---

## Resource Limits

Implement a `ulimit`-style builtin using:

```c
getrlimit()
setrlimit()
```

Examples:

```bash
ulimit -n
ulimit -t
```

---

## Operating Systems Extensions

Future standalone projects:

### Scheduler Simulator

Implement:

- FCFS
- Round Robin
- Priority Scheduling

---

### Page Table Simulator

Simulate:

- Virtual Addresses
- Physical Addresses
- Frame Translation
- Page Faults
- TLB Lookups

---

### Virtual Memory Simulator

Implement:

- Paging
- Demand Paging
- Replacement Algorithms
- LRU
- FIFO

---

# Author

Boaz King

Educational project built to learn Unix shell internals, process management, signals, pipes, job control, and operating systems concepts.
