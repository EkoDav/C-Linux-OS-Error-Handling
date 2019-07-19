/* 
 * mycopy.c written by Erik Davis
 * March 27, 2019
 * 
 * A simple program to perform a copy of data from
 * a source file to a destination file
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	
	/* Check number of arguments */
	if(!(argc == 3)) {
		printf("mycopy: Error - invalid number of arguments\n" 
			   "usage: mycopy <sourcefile> <destinationfile>\n");
		exit(1);
	}
	
	/* Make sure arguments are not the same */
	if(0 == strcmp(argv[1], argv[2])) {
		printf("mycopy: Error - source and destination files must be different\n");
		exit(1);
	}
	
	/* Open the source file from argv[1] */
	int fds = open(argv[1], O_RDONLY, 0);
	
	/* Check source file was opened successfully */
	if(fds < 0) {
		printf("Unable to open \"%s\": %s\n", argv[1], strerror(errno));
		exit(1);
	}
	
	/* Create destination file from argv[2] with full access permissions */
	int fdd = creat(argv[2], S_IRWXU);
	
	/* Check destination file was created successfully */
	if(fdd < 0) {
		printf("Unable to create \"%s\": %s\n", argv[2], strerror(errno));
		exit(1);
	}
	
	/* Copy data from source file to destination file in 64-byte chunks*/
	static int BUFF_SIZE = 64;
	char buff[BUFF_SIZE];
	int totalCount = 0;
	int readBytes = 0;
	int wroteBytes = 0;
	
	/* While there are more bytes to read from source file, read up to 64 */
	while((readBytes = read(fds, buff, BUFF_SIZE)) > 0) {
	/* Write to destination file and check success */
		wroteBytes = write(fdd, buff, readBytes);
		if(wroteBytes < 0) {
			printf("Error: Write to destination file failed: %s\n",
				strerror(errno));
			exit(1);
		}
	/* Update write success counter */
		totalCount += wroteBytes;
	}
	
	/* Check if there was a read error */
	if(readBytes < 0) {
		printf("Error: Read of source file failed: %s\n", strerror(errno));
		exit(1);
	}
	
	/* Close the source file and check success */
	int success = close(fds);
	if(success < 0) {
		printf("Error: The program was unable to close source file: %s\n", 
			strerror(errno));
		exit(1);
	}
	
	/* Close the destination file and check success */
	success = close(fdd);
	if(success < 0) {
		printf("Error: The program was unable to close destination file: %s\n", 
			strerror(errno));
		exit(1);
	}
	
	/* Display message indicating success with bytes copied */
	printf("Copied %d bytes from file %s to %s\n", totalCount, argv[1], argv[2]);
	
	/* Voluntarily exit program */
	return 0;
}
