# sysman

*NOTE: This is an old, old project (2000) tranferred to GitHub from sourceforge where it lay, dormant, for over a decade*

## Introduction

sysman is a multi-host management utility similar to VMS's Sysman tool. With sysman it is possible to execute a command or run a script on multiple hosts simultaneously with the aid of additional features designed to make the life easier. 
(In this documentation the words "host" and "node" are used interchangeably.)
 

## Features

 - Sequential and parallel execution of a system command on multiple hosts. 
 - Sophisticated I/O control. 
 - Execution of local shell scripts on remote nodes. 
 - Automatic conversion of commands and scripts according to the node's OS. 
 - Definition of host groups. 
 - Distribution of files. 
 - sysman scripts. 
 - Online help. 
 - Easily extendable.

## Requirements

In order to use sysman you will need the following: 
 - The Expect interpreter (sysman was written in Expect) 
 - The GNU Readline library. 
 - The lsof utility.

## Usage

### Command line

    sysman [sysman script filename] [-n <hosts/groups...>(.*)] [-- <command line arguments>]

`[sysman script filename]` = a sysman script to run. 
`[-n <hosts/groups...>(.*)]` = names of nodes or node-groups to connect to. 
`[-- <command line arguments>]` = arguments for use in a sysman script.

### Basic Use

    <command> [! <local pipes>]

The command is run on all connected nodes. the output may be piped to local files or programs. See topic "redirection" below for details. 
If the command is prefixed with a "!" then it refers to an internal sysman command or (if such a command does not exist) it is executed as a system command on the local machine.

    ! <hostnames>: <command>

runs the system/sysman command on the specified hosts/groups. See "hostames" below for more info.

### sysman commands (to be run by !<command>)

    help <topic> - displays online help 
    hosts - displays host information 
    connect - connects to hosts 
    run - runs a local script 
    set - sets a sysman environment variable 
    talk - interacts directly with a host 
    put - sends a file or a directory 
    get - receives a file or a directory 
    passwd - change the password on the hosts 
    shutdown - shutdown the remote hosts

all other commands (that begin with a !) are executed by the local shell.

```
hosts [-a] 
```

displays a list of the connected hosts. 
-a: shows additional group and translation information

```
connect <hostnames>
```
connects to the hosts and the host groups specified. 
See "hostames" below for more info. 
 
```
run <shell script filename> 
```
runs the specified script on the remote nodes hosts translation.

```
put <filespec/directory> [<target dir>]
```
sends the specified files or directory to the remote hosts into the target directory. 
To send a file to a subset of the connected hosts, prefix the put command with the host names format ( !<hostnames>: !put ... )

```
get <filespec/directory> [<target dir>] 
```
receives the specified files or directory from the remote hosts into the target directory. 
if multiple hosts are connected, it is necessary to put the files from each hosts into a separate directory. this can be accomplished by using the $host variable (See below).

to receive a file from a subset of the connected hosts, prefix the get command with the host names format ( !<hostnames>: !get ... )

```
set <environment variable> [<value>] 
```
if no value is specified, the current value of the variable is displayed. 
a value of "off" is equivalent to a value of 0, and a value of "on" is equivalent to 1.

```
talk <host> 
```
allows you to interact directly with the host in a telnet session. return to sysman by pressing ^] 
"talk local" talks to the local shell.

```
passwd [<user>] [<new password>] 
```
if user is omitted the password is changed for the current user. 
if user is NOT omitted, you can specify the new password in the command line. 
 
```
shutdown <params> 
```
sends the shutdown command to the remote hosts and disconnects them. you can shut down a subset of the hosts by using `!<hostnames>: !shutdown ...` 
 

### Sysman Environment Variables

    echo - sets echoing of the sent commands (after translation) 
    convert - sets translation on/off 
    timeout - sets the time for connection timeout 
    delay - sets file transfer speed 
    color - sets host title color (0 - disabled) 
    parallel - set parallel execution 
    comment - set the string comment. lines beginning with this string will not be sent 
 

### Sysman Command Line Variables

    $hosts - a list of all currently connected hosts 
    $host - for each host, equals the name of the host the command is running on 
    $1, $2, ... - the command line arguments 
    $* - all the command line arguments

These variables can be used in any command. The variables `$1`,`$2`,... and `$*` are the values passed to sysman when it was started and are especially useful in sysman scripts (See below).

The variable $host has a different value for each host. For example, typing `echo $host` would result in each host echoing its name (the one used by sysman). This variable is usually used in local redirection (See below).

### Sysman Files

    `hosts.dat` - host names and adresses 
    `groups.dat` - host groups definitions 
    `convert.dat` - conversion specifications

`hosts.dat` - host names adresses (and other information in future versions).

Format: 
    hostname address (other info) 
    ...

`groups.dat` - host groups definitions

The host name used is according to the hostname format (see below).

Format: 

    groupname 
    hostname/groupname 
    hostname/groupname 
    ...
    
    groupname 
    hostname/groupname 
    hostname/groupname 
    ...
    
    ...


`convert.dat` - automatic conversion specifications 
    hostname/groupname:source string:target string 
    the conversion performed is `regsub -- {<source string>} $command {<target string>}` command 
    for more information turn to the TCL documentation on the regsub command. 
 

### Host Names

Whenever a list of host names (or a single host name) is needed, it should be given as a space separated list of host or group names. 
The group names are the ones found in the groups.dat file, and the host names are the ones found in hosts.dat or (if not found there) the host's URL (or IP address). 
The list is to include a list of all the hosts in the specified hosts/groups list. 
You can exclude a host or a group from the list by prefixing its name with a tilde (~). 
If the list contains only names prefixed by a tilda, the expanded list will include all of those hosts currently connected except for those excluded.

Examples: 

Assume: group1 = { host1 host2 }, group2 = { host2 host3 }, group3 = { group1 group2 } 

if we are connected to group1, then: 

`!host1: <command>` - executes on host1 

`!group2: <command>` - executes on host2 

`!~group2: <command>` - executes on host1 

If we are connected to group3, then: 

`!group1 host3: <command>` - executes on host1, host2, host3 

`!group1 ~group2: <command>` - executes on host1

`!~group1: <command>: <command>` - executes on host3
 

### Local Redirection

With sysman it is possible to redirect output from the remote nodes to local files or pipes in a sophisticated way. 
In order to redirect output from a command or a shell script run by the !run command to a local file or program, prefix the redirection symbol (>,>>,|) with '!'. 
When redirecting output from multiple hosts to a single file, use the >> operator. 
You can redirect output from multiple hosts to multiple files by using the $host variable in the file name or path.

Examples:

    uptime !>> uptime.out 
    !run my_script.sh !> $host.my_script.out 
    cat info !| process_info 
    !run my_script.sh !| tail -5 > $host/max

### Sysman Scripts

Since sysman is a shell it can run its own scripts. To make a sysman script use `#!/.../sysman` at the script's top line. 
The variables `$1`, `$2`,... and `$*` will contain the arguments passed to the script.

## Sysman Internals (in brief)

The way sysman works is by managing multiple telnet sessions - one per each connected host, collecting the output and presenting it sensibly.

sysman has two main loops: one when running interactively and one when running a script (both in the --MAIN-section) . These loops are a good place to start.

sysman's interface makes use of `dumbsh` (dumb shell) - a readline shell that does nothing other than print the command typed (this gets intercepted by sysman with Expect's  interact command). `dumbsh` has a primitive interface which allows it to be given a list of words to be used as a completion list (with the TAB key) - at the command line type -c, then Enter and then a space seperated list of words and then Enter again. 

To build dumbsh type: `gcc -lreadline dumbsh.c -o dumbsh`

The command gets processed by the do_command proc.

The heart of sysman is the two procedures rsh and rsh_par that handle the output returned from the hosts when running in sequential or parallel mode respectively. Because rsh_par has to correctly handle input that comes from many sources simultaneously, the procedure is a rather complex state machine and is quite difficult to understand (so is the procedure "connect" which connects to the hosts in parallel). 
The main issue in the output handling is determining whether the output from all the hosts is directed to the same file (output "conflict") or to different files (in which case it is not necessary to print the hostname title at the top of the output). If we pipe the output to a program, a different instance of the program is started for each host, however there might still be an output conflict if after the pipe chain, the output flows to the screen (or to the same file), so sysman uses lsof to determine the final file the output flows to.

An environment variable "X" is simply the global variable "var_X" and the sysman command "Y" is simply the procedure "cmd_Y". All environment variables should be added to the global list `sysman_vars` and all sysman commands should be added to the list `sysman_commands`.

The sysman comman line variables are interpreted by the procedure lookupvar.

## Things to do

 - Make the program clearer. 
 - Make the ^C handler work better. 
 - Fix dumbsh bug (memory flow) to allow automatic completion of host names (the completion list will be updated whenever the host list is updated). 
 - Make sysman modular for OS's outside the Unix family. The OS can be specified in hosts.dat. 
 - Port the whole thing to SCHEME (requires porting Expect first) 


## License (MIT)

Copyright (c) 2000, 2013 Ron Pressler

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 