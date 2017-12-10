//[EOPSY.A] Laboratory task 6
//Kosmas Tsiakas

//The following programs copies one file to another, either with memory mapping or with read and write instructions.
//Some parameters are given to change the way the program is called

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define BUF_SIZE 8192

//Essential functions definitions
void copy_read_write(int fd_from, int fd_to);
void copy_mmap(int fd_from, int fd_to);
void help();


int main(int argc, char **argv){
	//initializing variables
	int opt = 0;
	int fd_from, fd_to;
	
	//I have to check the number of arguments given, so that I can save the correct filename in the correct variable
	//Opening the files, both input and output and checking for errors
	//Always the last argument is the second file and the second from the end, the first file

	if (argc>2){
		fd_from = open (argv[argc-2], O_RDONLY);
    	if (fd_from == -1) {
           	perror ("Error opening source file\n");
           	return 2;
    	}
		fd_to = open(argv[argc-1], O_RDWR | O_CREAT, 0666);
    	if (fd_to == -1){
        	perror("Error opening destination file\n");
        	return 3;
    	}
	}

	//Start getting the option arguments. As long as it is not returning -1 there are arguments available to check
	//We are checking for - and -m, and if there are not arguments, there is the execution of the function out of the while loop
	//After each function's execution we exit the program, so there is no case of executing more than one function

	while ((opt = getopt(argc, argv, ":mh::")) != -1) {
		switch(opt){
			case 'h':
				help(); //we do not need to have a break, since there is exit() in the function called
			case 'm':
				copy_mmap(fd_from, fd_to);
			default:
				printf("Wrong option. Check ./copy -h for more information\n");
				exit(0);
		}
	}
	if (argc>2){
		//since we do not have any options but we have arguments, we want the copy function with read-write
		copy_read_write(fd_from, fd_to);
	}
	else{//when the program is called without any arguments, just print some information
		help();
	}
	//successful return
	return 0;
}

//Copy functions implementations
void copy_read_write(int fd_from, int fd_to){
	char buffer[BUF_SIZE];      /* Character buffer */

	ssize_t filesize_from, filesize_out;    /* Number of bytes returned by read() and write() */

	/* Copy process */
    while((filesize_from = read (fd_from, &buffer, BUF_SIZE)) > 0){ //as long as we haven't reached the end, keep reading
            filesize_out = write (fd_to, &buffer, (ssize_t) filesize_from);
            if(filesize_out != filesize_from){
                /* Write error */
                perror("write");
                exit(0);
            }
    }
 
    /* Close file descriptors */
    close (fd_from);
    close (fd_to);

    printf("File copying has been successfully complete\n");

    exit(1);
}

void copy_mmap(int fd_from, int fd_to){
	struct stat st;
	fstat(fd_from, &st); //return the status of the source file to the struct st

	char* target;
	target=mmap(0, st.st_size, PROT_READ, MAP_SHARED, fd_from, 0); //maps in the virtual address space 
	if (target==MAP_FAILED){ //system calls' error check
    	printf("Error mapping target with errno: %d.\n", errno);
    	exit(6);
	}

	char* destination;
	ftruncate(fd_to, st.st_size); //pre-size the destination file to the size of the source file
	destination=mmap(0, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_to, 0);
	if (destination == MAP_FAILED){
    	printf("Error mapping destination with errno: %d.\n", errno);
    	exit(5);
	}

	memcpy(destination, target, st.st_size); //copy the memory area between source and destination
	munmap(destination, st.st_size); //unmap files from memory since the copy has been done
	munmap(target, st.st_size);

	/* Close file descriptors */
    close (fd_from);
    close (fd_to);

    printf("File copying has been successfully complete\n");

    exit(1);
}

void help(){
	printf("\nThis program copies one file to another. \nSyntax is as follows:\n\n");
	printf("./copy -m [source_file] [destination_file] -- copy the files by memory mapping\n");
	printf("./copy [source_file] [destination_file] -- copy the files by reading and writing\n");
	printf("./copy -h -- provide help for this program\n\n");
	exit(1);
}
