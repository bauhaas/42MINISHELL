# 42MINISHELL
## List of content
...
[![norminette](https://github.com/Korkrane/42MINISHELL/actions/workflows/norminette.yaml/badge.svg)](https://github.com/Korkrane/42MINISHELL/actions/workflows/norminette.yaml)
## Git structure
### Commit mssg
- `ADD: ...` to implement new function/files
- `FIX: ...`  to correct bugs
- `NORM: ...` to correct norm error
- `RFCT: ...` to refacto  

## To do
  - [ ] 1. Trouver un symbole unicode pr remplacer les flèches de couleurs dans le prompt
  - [x] 2. ; seul doit retourner une erreur de syntaxe avec une valeur de 2 (pr le moment on a command not found & 127)
  - [x] 3. "" seul doit print un long message d'info
  - [x] 4. echo -nnnnnnnnnnnna doit print sur une nouvelle line
  - [x] 5. ls | exit a le bon comportement mais affiche exit alors qu'il ne devrait pas
  - [ ] 6. export a="echo toto"  pr le moment on tente d'execute une commande "echo toto" qui ne trouve pas il faut resplit le echo et toto en 2elem
  - [x] 7. Quand on lance une mauvaise cmd puis on exit, cela doit  return 127 (on return 0)
  - [x] 8. cat only + ctrl C (comportement chelou avec le prompt malgré le fait qu'on est le bon retour et le bon nombre de process créer/kill)
  - [ ] 9. > fgfegfge | ls redirection en premier doit pvr marcher et créer le fichier
  - [ ] 10. env -i ./minishell  pas d'erreur normalement., nous on a : minishell: Error termcaps -1
  - [x] 11. unset PATH puis ls, on segfault car on va se servir de PATH pr search prog alors qu'il est vide
  - [x] 12. gérer les cas commencant et ayant juste en cmd une redir ou un pipe
  - [x] 13. Qd on écrit dans minishell, l'input rame et peut sauter des touches qui ont étés tapés (j'avais ce pb depuis un petit moment déjà)

## External functions allowed
| Function name | Description                    |
| ------------- | ------------------------------ |
| `fork`      | fork() creates a new process by duplicating the calling process.  The new process is referred to as the child process.  The calling process is referred to as the parent process.<br /><br />RETURN: On success, the PID of the child process is returned in the parent, and 0 is returned in the child.  On failure, -1 is returned in the parent, no child process is created, and errno is set appropriately.     |
| `wait`   | wait() system call suspends execution of the calling process until one of its children terminates.<br /><br />RETURN: On success, returns the process ID of the terminated child; on error, -1 is returned. |
| `waitpid`      |  waitpid()  system  call  suspends execution of the calling process until a child specified by pid argument has changed state.  By default, waitpid() waits only for terminated children, but this behavior is modifiable via the options argument, as described below.<br /><br />RETURN: On success, returns the process ID of the child whose state has changed; if WNOHANG was specified and one or more child(ren) specified by pid exist, but have not yet changed state, then 0 is returned.  On error, -1 is returned.     |
| `wait3` `wait4`      | The wait3() and wait4() system calls are similar to waitpid(2), but additionally return resource usage information about the child in the structure pointed to by rusage.<br /><br />RETURN: On success, returns the process ID of the child whose state has changed; if WNOHANG was specified and one or more child(ren) specified by pid exist, but have not yet changed state, then 0 is returned.  On error, -1 is returned.     |
| `signal`      | signal() sets the disposition of the signal signum to handler, which is either SIG_IGN, SIG_DFL, or the address of a programmer-defined function (a "signal handler").<br /><br />RETURN: On succes, returns the previous value of the signal handler, or SIG_ERR on error.  In the event of an error, errno is set to indicate the cause.   |
| `kill`      | ...       |
| `exit`      | ...      |
| `getcwd`      | getcwd() function copies an absolute pathname of the current working directory to the array pointed to by buf, which is of length size.<br /><br />RETURN: On success, these functions return a pointer to a string containing the pathname of the current working directory.  In the case getcwd() and getwd() this is the same value as buf. On failure, these functions return NULL, and errno is set to indicate the error.  The contents of the array pointed to by buf are undefined on error.       |
| `chdir`      |chdir() changes the current working directory of the calling process to the directory specified in path.<br /><br />RETURN:  On success, zero is returned.  On error, -1 is returned, and errno is set appropriately.      |
| `stat` `lstat` `fstat`     | These  functions  return  information  about  a  file, in the buffer pointed to by statbuf.  No permissions are required on the file itself, but—in the case of stat(), fstatat(), and lstat()—execute(search) permission is required on all of the directories in pathname that lead to the file.<br /><br />stat() and fstatat() retrieve information about the file pointed to by pathname; the differences for fstatat() are described below.<br /><br />lstat() is identical to stat(), except that if pathname is a symbolic link, then it returns information about the link itself, not the file that it refers to.<br /><br />fstat() is identical to stat(), except that the file about which information is to be retrieved is specified by the file descriptor fd.<br /><br />RETURN: On success, zero is returned.  On error, -1 is returned, and errno is set appropriately.    |
| `execve`      | execve() executes the program pointed to by filename.  filename must be either a binary executable, or a script.<br /><br />RETURN: On success, execve() does not return, on error -1 is returned, and errno is set appropriately.      |
| `dup`      | dup() system call creates a copy of the file descriptor oldfd, using the lowest-numbered unused file descriptor for the new descriptor.<br /><br />RETURN:  On success, these system calls return the new file descriptor.  On error, -1 is returned, and errno is set appropriately.    |
| `dup2`      | dup2() system call performs the same task as dup(), but instead of using the lowest-numbered unused file descriptor, it uses the file descriptor number specified in newfd.  If the file  descriptor newfd was previously open, it is silently closed before being reused.<br /><br />RETURN: On success, these system calls return the new file descriptor.  On error, -1 is returned, and errno is set appropriately.     |
| `pipe`      | pipe()  creates  a  pipe,  a  unidirectional data channel that can be used for interprocess communication.  The array pipefd is used to return two file descriptors referring to the ends of the pipe. pipefd[0] refers to the read end of the pipe.  pipefd[1] refers to the write end of the pipe.  Data written to the write end of the pipe is buffered by the kernel until it is read from the read  end of the pipe.  For further details, see pipe(7).<br /><br />RETURN: On success, zero is returned.  On error, -1 is returned, and errno is set appropriately.       |
| `opendir`      | opendir() function opens a directory stream corresponding to the directory name, and returns a pointer to the directory stream.  The stream is positioned at the first entry in the directory.<br /><br />RETURN: On succes, return a pointer to the directory stream.  On error, NULL is returned, and errno is set appropriately.     |
| `readdir`      | readdir() function returns a pointer to a dirent structure representing the next directory entry in the directory stream pointed to by dirp.  It returns NULL on reaching the end of the directory stream or if an error occurred.<br /><br />RETURN: On success, readdir() returns a pointer to a dirent structure.  (This structure may be statically allocated; do not attempt to free(3) it.) If the end of the directory stream is reached, NULL is returned and errno is not changed.  If an error occurs, NULL is returned and errno is set appropriately.  To distinguish end of stream and from an error, set errno to zero before calling readdir() and then check the value of errno if NULL is returned.      |
| `closedir`      | closedir()  function  closes  the  directory stream associated with dirp.  A successful call to closedir() also closes the underlying file descriptor associated with dirp.  The directory stream descriptor dirp is not available after this call.<br /><br />RETURN: On succes, returns 0 on success.  On error, -1 is returned, and errno is set appropriately.       |
| `strerror`      | strerror() function returns a pointer to a string that describes the error code passed in the argument errnum, possibly using the LC_MESSAGES part of the current locale to select the appropriate language. This string must not be modified by the application, but may be modified by a subsequent call  to strerror() or strerror_l(). <br /><br />RETURN: The strerror(), strerror_l(), and the GNU-specific strerror_r() functions return the appropriate error description string, or an "Unknown error nnn" message if the error number is unknown.      |
| `errno`      | ...      |
| `isatty`      | isatty() function tests whether fd is an open file descriptor referring to a terminal.<br /><br />RETURN: On succes, returns 1 if fd is an open file descriptor referring to a terminal; otherwise 0 is returned, and errno is set to indicate the error.      |
| `ttyname`      | ttyname()  returns  a pointer to the null-terminated pathname of the terminal device that is open on the file descriptor fd, or NULL on error (for example, if fd is not connected to a terminal).  The return value may point to static data, possibly overwritten by the next call.  The function ttyname_r() stores this pathname in the buffer buf of length buflen.<br /><br />RETURN: ttyname() returns a pointer to a pathname on success.  On error, NULL is returned, and errno is set appropriately.  The function ttyname_r() returns 0 on success, and  an  error  number upon error.     |
| `ttyslot`      | ttyslot() returns the index of the current user's entry in some file.<br /><br />RETURN: On succes, returns the slot number. On error (e.g., if none of the file descriptors 0, 1 or 2 is associated with a terminal that occurs in this data base) it returns 0 on UNIX V6 and V7 and BSD-like systems, but -1 on System V-like systems.|
| `ioctl`      | ioctl()  system call manipulates the underlying device parameters of special files.  In particular, many operating characteristics of character special files (e.g., terminals) may be controlled with ioctl() requests.  The argument fd must be an open file descriptor.<br /><br />RETURN: On success zero is returned.  A few ioctl() requests use the return value as an output parameter and return a nonnegative value on success.  On error, -1  is  returned,  and  errno  is  set appropriately.   |
| `getenv`      | getenv() function searches the environment list to find the environment variable name, and returns a pointer to the corresponding value string.<br /><br />RETURN: On succes, returns a pointer to the value in the environment, or NULL if there is no match.Q |
| `tcsetattr`      | ...      |
| `tcgetattr`      | ...       |
| `tgetent`      | ...       |
| `tgetflag`      | ...       |
| `tgetnum`      | ...      |
| `tgetstr`      | ...       |
| `tgoto`      | ...      |
| `tputs`      | ...       |

## Ressources
