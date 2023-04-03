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
./pipex /dev/random cat rev out
```

### Here Doc

```bash
./pipex here_doc STOP "cat -e" "cat -e" outfile
```
(we must append the new entry in the file)
(test exit here doc with ctr+d)
