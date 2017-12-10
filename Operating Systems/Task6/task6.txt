
  Operations on files, mmap() and getopt()


Write a program which copies one file to another. Syntax:

  copy [-m] <file_name> <new_file_name>
  copy [-h]


Without option -m use read() and write() functions to copy file contents. If
the option -m is given, do not use neither read() nor write() but map files
to memory regions with mmap() and copy the file with memcpy() instead.

If the option -h is given or the program is called without arguments print
out some help information.  

Important remarks: 

- use getopt() function to process command line options and arguments,

- the skeleton of the code for both versions (with read/write and with mmap)
  should be the same, but in some place either copy_read_write(int fd_from,
  int fd_to) or copy_mmap(int fd_from, int fd_to) should be called,

- check error codes after each system call.


Manuals (mostly from section 2, i.e. "man -s 2 open"):

man -s 3C getopt
man open
man close
man read
man write
man lseek
man fstat
man mmap
man memcpy 

List of necessary include files:

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>




