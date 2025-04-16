# Micro Shell

A simple implementation of a shell program with basic functionality including command execution, environment variable management, built-in commands, and I/O redirection.

## Features

- Command execution via PATH lookup
- Variable assignment and expansion
- Built-in commands (`echo`, `cd`, `exit`, `export`)
- Environment variable management
- I/O redirection (`<`, `>`, `2>`)

## Compilation

To compile the micro shell, use:

```bash
gcc -o microshell main.c
```

## Usage

After compilation, run the shell with:

```bash
./microshell
```

You will see the shell prompt: `Micro shell prompt >`

## Supported Commands

### Built-in Commands

1. **exit** - Exit the shell
   ```
   Micro shell prompt > exit
   Good Bye
   ```

2. **echo** - Display text or variable values
   ```
   Micro shell prompt > echo Hello World
   Hello World
   ```

3. **cd** - Change directory
   ```
   Micro shell prompt > cd /tmp
   ```

4. **export** - Export a local variable to environment variables
   ```
   Micro shell prompt > export VARIABLE_NAME
   ```

### Variable Management

1. **Variable Assignment** - Create or set local variables
   ```
   Micro shell prompt > varname=value
   ```

2. **Variable Expansion** - Use variables in commands
   ```
   Micro shell prompt > echo $varname
   value
   ```

### I/O Redirection

The shell supports the following I/O redirection operators:

1. **Input Redirection** (`<`): Redirect standard input from a file
   ```
   Micro shell prompt > command < input_file
   ```

2. **Output Redirection** (`>`): Redirect standard output to a file
   ```
   Micro shell prompt > command > output_file
   ```

3. **Error Redirection** (`2>`): Redirect standard error to a file
   ```
   Micro shell prompt > command 2> error_file
   ```

4. **Multiple Redirections**: Combine multiple redirections in a single command
   ```
   Micro shell prompt > command < input_file > output_file 2> error_file
   ```

Note: If an error occurs during redirection (e.g., unable to open a file), the command will not be executed.

### External Commands

Any command available in the system PATH can be executed:

```
Micro shell prompt > ls -la
```

## Test Cases

### Basic Operation Tests

1. **Shell Startup and Exit**
   ```
   $ ./microshell
   Micro shell prompt > exit
   Good Bye
   ```

2. **Basic Echo**
   ```
   Micro shell prompt > echo Hello World
   Hello World
   ```

3. **Invalid Command**
   ```
   Micro shell prompt > invalidcommand
   invalid_command: command not found
   ```

### Variable Tests

1. **Variable Assignment and Echo**
   ```
   Micro shell prompt > testvar=hello
   Micro shell prompt > echo $testvar
   hello
   ```

2. **Multiple Variable Operations**
   ```
   Micro shell prompt > name=John
   Micro shell prompt > age=30
   Micro shell prompt > echo $name is $age years old
   John is 30 years old
   ```

3. **Invalid Variable Assignment**
   ```
   Micro shell prompt > var =value
   Invalid command
   ```

4. **Invalid Variable Assignment (2)**
   ```
   Micro shell prompt > var= value
   Invalid command
   ```

### Directory Navigation Tests

1. **Change Directory**
   ```
   Micro shell prompt > cd /tmp
   Micro shell prompt > pwd
   /tmp
   ```

2. **Invalid Directory**
   ```
   Micro shell prompt > cd /invalid_directory
   cd: /invalid_directory: No such file or directory
   ```

### Export Tests

1. **Export Variable**
   ```
   Micro shell prompt > testvar=hello
   Micro shell prompt > export testvar
   Micro shell prompt > env | grep testvar
   testvar=hello
   ```

2. **Export Non-existent Variable**
   ```
   Micro shell prompt > export nonexistent
   Variable nonexistent not found
   ```

### I/O Redirection Tests

1. **Input Redirection**
   ```
   Micro shell prompt > echo "This is a test file" > test.txt
   Micro shell prompt > cat < test.txt
   This is a test file
   ```

2. **Output Redirection**
   ```
   Micro shell prompt > ls -la > listing.txt
   Micro shell prompt > cat listing.txt
   [directory listing content]
   ```

3. **Error Redirection**
   ```
   Micro shell prompt > ls /nonexistent 2> error.txt
   Micro shell prompt > cat error.txt
   ls: cannot access '/nonexistent': No such file or directory
   ```

4. **Multiple Redirections**
   ```
   Micro shell prompt > cat < input.txt > output.txt 2> error.txt
   ```

5. **Redirection with Non-existent File**
   ```
   Micro shell prompt > cat < nonexistent.txt
   [Error message about not being able to open file, command not executed]
   ```

6. **Variable Expansion in Redirection**
   ```
   Micro shell prompt > filename=test.txt
   Micro shell prompt > cat < $filename
   [Content of test.txt]
   ```

### Command Execution Tests

1. **Command with Arguments**
   ```
   Micro shell prompt > ls -la
   [directory listing appears]
   ```

2. **Command with Variable Expansion**
   ```
   Micro shell prompt > dir=/tmp
   Micro shell prompt > ls $dir
   [listing of /tmp directory]
   ```

### Edge Cases

1. **Empty Command**
   ```
   Micro shell prompt > 
   Micro shell prompt >
   ```

2. **Variable Expansion in Command Path**
   ```
   Micro shell prompt > bindir=/bin
   Micro shell prompt > $bindir/ls
   [directory listing appears]
   ```

3. **Long Variable Names**
   ```
   Micro shell prompt > verylongvariablename=testvalue
   Micro shell prompt > echo $verylongvariablename
   testvalue
   ```

4. **Redirection with No File Specified**
   ```
   Micro shell prompt > cat <
   [Error message about missing redirection file]
   ```

5. **Redirection to/from the Same File**
   ```
   Micro shell prompt > cat < file.txt > file.txt
   [Behavior depends on implementation]
   ```

## Known Limitations

1. No support for command chaining (e.g., with `&&`, `||`, or `;`)
2. No command history or tab completion
3. Limited error handling for edge cases
4. No support for background processes (with `&`)
5. No support for command substitution (with backticks or `$()`)
6. No support for wildcard expansion
7. No support for pipes (`|`)
8. No support for appending to files (`>>`, `2>>`)

## Debugging Tips

- If your shell gets stuck or behaves unexpectedly, try running with strace to see system calls:
  ```
  strace ./microshell
  ```
- For memory-related issues, consider using valgrind:
  ```
  valgrind --leak-check=full ./microshell
  ```
- To test I/O redirection functionality, create test files with known content:
  ```
  echo "Test input" > input.txt
  ```
- For complex redirection scenarios, you can compare the behavior with bash:
  ```
  bash -c "your command with redirections" # Compare with your shell's behavior
  ```
