# Pipex

## Project

the purpose of the project is to recode redirection and bash pipes

### Multipipe

#### Bash:

```bash
< infile cmd1 | cmd2 | cmd3 | [...] > outfile
```

#### Pipex:

```bash
./pipex infile "cmd1" "cmd2" "cmd3" [...] outfile
```
### Here doc

#### Bash:

```bash
cmd << LIMITER | cmd1 >> file
```

#### Pipex:

```bash
./pipex here_doc "limiter" "cmd1" "cmd2" "cmd3" [...] outfile
```

## Download

```bash
git clone --recurse-submodules git@github.com:Elnop/42-Pipex.git
cd 42-Pipex
make
```

## Usage exemples

### Mandatory & Multipipe

create infile

```bash
touch infile
```

simple

```bash
./pipex infile "cat" "cat" outfile
```

commands runs in parallel

```bash
./pipex /dev/stdin "cat" "cat" "cat" "ls" /dev/stdout
```

```bash
./pipex infile "yes" "yes" "yes" "head" outfile
```

invalid commands

```bash
./pipex infile "notexistingcmd" "cat" outfile
```

```bash
./pipex infile "" "cat" outfile
```

invalid files

```bash
./pipex notexist "cat" "cat" outfile
```

```bash
chmod 000 infile
./pipex infile "cat" "cat" outfile
```

random

```bash
./pipex /dev/random cat rev outfile
```

## Valgrind
```bash
valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=yes ./pipex file1 cmd1 cmd2 file2
```

--leak-check                    -- search for memory leaks at exit

--show-leak-kinds               -- specify leak kinds to show

--track-fds                     -- track open file descriptors

--trace-children                -- valgrind-ise child processes

⚠️ "trace-children" swow leaks in executed files

exemple: ls leak

```bash
valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=yes ./pipex Makefile ls ls test
```

### Here Doc

```bash
./pipex here_doc STOP "cat -e" "cat -e" outfile
```
(we must append the new entry in the file)

(test exit here doc with ctr+d)
