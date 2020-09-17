#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <unistd.h>
#include<stdlib.h>
/*
	Author: Shen Ouyang
	date 09/09/2020
	description: Read , copy and move files through user instructions.
*/




/*
	file_open function, working for Task1 and Task2. The function is to
	get file name and open it.
*/
void file_open(char fileName[]){
    int n, infile ;
    char buffer[1024];
	// Open file
    if((infile = open (fileName, O_RDONLY))  <0){
        write(1, "Usage: ./fileuilt ../../a.txt \n", 32);
        exit(1);
    }
	// Read file to buffer
    n=read(infile,buffer, sizeof(buffer));
	// Write file content to Screen
    write(2, buffer , n);
    close(infile);
}
/* 
	Copy_file function, working for user copying deafult file to destination directory
	then read the default file and write to the target directory
*/
void Copy_default_file(char fileName[], char outfile[]){
    int n, infile;
    char buffer[1024];
    int desfile;
	char defaultfile[]="logfile.txt";
	//Combine path and file name.
	strcat(outfile,defaultfile);
	// Check if there is target file exist.
	if((desfile=open(outfile,O_RDONLY)>=0)){
		write(2,"Same name file existed\n",24);
		exit(1);
	}
	// Check if deafult file open or not.
	if((infile = open (fileName, O_RDONLY))  <0){
        write(1, "invalid argument: no source file\n", 33);
        exit(1);
	}
	// Create a file which have same name as source destination file.
    if((desfile=open(outfile,O_RDWR | O_CREAT, 00777))<0){
		write(2,"invalid argument: no destination dirctory to copy the file\n", 60);
		exit(1);
	}
	// Read the file content to n then write it to destdir file 
    n=read(infile,buffer,sizeof(buffer));
	write(desfile,buffer,n);
	write(1,"Copy logfile.txt to destination directory success\n",51);
    close(infile);
    close(desfile);
     
}
/*	Copy_move_file function, working for Task3, 4 ,5. The main function is to get
	sourcefile and destination directroy, then read the sourcefile and write to
	the target directroy.
*/
void Copy_move_file(char fileName[], char outfile[]){
	int n, infile ,desfile;
	char sfilename[80], *p;
	char buffer[1024];
	//Get the Filename from user`s input path
	strcpy(sfilename,(p=strrchr(fileName,'/')) ? p+1 : fileName);
	//Combine filename and destination path	
	strcat(outfile,sfilename);
	if((desfile=open(outfile,O_RDONLY)>=0)){
		write(2,"Same name file existed\n",24);
		exit(1);
	}
	if((infile = open (fileName, O_RDONLY))  <0){
        write(1, "invalid argument: no source file\n", 33);
        exit(1);
	}
	if((desfile=open(outfile,O_RDWR | O_CREAT, 00777))<0){
		write(2,"invalid argument: no destination dirctory to copy the file\n", 60);
		exit(1);
	}
    n=read(infile,buffer,sizeof(buffer));
	write(desfile,buffer,n);
	write(1," Move or Copy success\n",23);
	close(infile);
	close(desfile);
	
}

void Force_Move_copy(char fileName[], char outfile[]){
	int n, infile ,desfile;
	char sfilename[80], *p;
	char buffer[1024];
	//Get the Filename from user`s input path
	strcpy(sfilename,(p=strrchr(fileName,'/')) ? p+1 : fileName);
	//Combine filename and destination path	
	strcat(outfile,sfilename);
	if((infile = open (fileName, O_RDONLY))  <0){
        write(1, "invalid argument: no source file\n", 33);
        exit(1);
	}
	if((desfile=open(outfile,O_RDWR | O_CREAT, 00777))<0){
		write(2,"invalid argument: no destination dirctory to copy the file\n", 60);
		exit(1);
	}
    n=read(infile,buffer,sizeof(buffer));
	write(desfile,buffer,n);
	write(1,"Force Move or Copy success",27);
	close(infile);
	close(desfile);
	
}

int main(int argc , char* argv[]){
	char buffer[1024];
    char defaultfile[]="logfile.txt";

	
	//open default file
	if(argc == 1){
        file_open(defaultfile);
    }
	//open the target file
	if (argc==2){
		file_open(argv[1]);
	}
	
    if( argc>=3) 
	{
	// if argument count =3 then use default logfile for copying.
		if(argc==3){
			if (strcmp("-d",argv[1])==0){
                Copy_default_file(defaultfile,argv[2]);
            }
			else if(strcmp("-M",argv[2])==0){
				write(1,"invalid argument, no destination to move the file\n",50);
				exit(1);
			}
			else if(strcmp("-F",argv[2])==0){
				write(1,"invalid arugment, F is redundant as nothing to force here\n",59);
			}
			else{
				write(2,"Pelase input -d after call the program.\n", 41);
				exit(1);	
			}		
		}
	    //if argument count=4 then copy file from source path to destination path
		else if (argc==4)
		{
			if (strcmp("-d",argv[2])==0){
				Copy_move_file(argv[1],argv[3]);	
			}
			//IF the argv[2] is not "-d" then show the error msg.
			else if(strcmp("-M",argv[2])==0)
			{
				if(strcmp("-F",argv[3])==0){
					write(1,"invalid argument, no destination to move file\n",47);
					exit(1);
				}
			} 
			else
			{
				write(2, "Please input -d after call the program.\n", 41);
				exit(1);	
			}	
		}
		// If argument count =5, There are 2 possibilities.  
		// 1: Move file 
		// 2: Force Copy file
		else if (argc==5)
		{
			//Move function. Check if the second argument is "-d", and fourth argument is "-M"
			//if so then move function executed.
			if (strcmp("-d",argv[2])==0){
			    if (strcmp("-M",argv[4])==0){
				    Copy_move_file(argv[1],argv[3]);
					//Once move Success, delete sourcefile.
                    unlink(argv[1]);
			    }
                
            }
			//Force copy function. Check if argv[2] is "-d" and argv[4] is "-F"
			//if so then Force copy function executed.
            else if(strcmp("-d",argv[2])==0)
			{
				if(strcmp("-F",argv[4])==0){
					Force_Move_copy(argv[1],argv[3]);
				}   
            }
			else
			{
				write(2, "invalid argument, immediately after -d, a directroy path was expected ", 71);
				exit(1);
			}
            
		}
		else if (argc==6)
		{	//There are Four possiblities to make force move success.
			//Force Move 
			if(strcmp("-d",argv[2])==0)
			{
				if(strcmp("-M",argv[4])==0){
					if(strcmp("-F",argv[5])==0){
						Force_Move_copy(argv[1],argv[3]);
						write(1,"Force Move success"
					}
				}
			}
			else if(strcmp("-F",argv[2])==0)
			{
				if(strcmp("-M",argv[3])==0){
					if(strcmp("-d",argv[4])==0){
						Force_Move_copy(argv[1],argv[5]);
					}
				}
			}
			else if(strcmp("-M",argv[2])==0)
			{
				if(strcmp("-F",argv[3])==0){
					if(strcmp("-d",argv[4])==0){
						Force_Move_copy(argv[1],argv[5]);
					}
				}
			}
			else if(strcmp("-d",argv[2])==0)
			{
				if(strcmp("-F",argv[4])==0)
				{
					if(strcmp("-M",argv[5])==0){
						Force_Move_copy(argv[1],argv[3]);
					}

				}
			}
			else
			{
				write(1,"invalid argument\n",18);
				exit(1);
			}
		}
		else
		{
			write(1, "Too many arguments!",20);
			exit(1);
		}
			
	}
	exit(0);
}
