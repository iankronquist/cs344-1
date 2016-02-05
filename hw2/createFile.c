#include <dirent.h>
#include <math.h> //Requires linking with -lm
#include <pwd.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>


//get the name of the dir
char *get_name_dir() {
	//process id
	pid_t id = getpid();
	//length of my dir name = jiajun.room.+process_id = 12+20 = 32
	char *name = malloc(32 * sizeof(char));
	sprintf(name, "jiajun.rooms.%d", id);
	return name;

}

int main(){
	int i;
	const char *roomName[7];
	char *name = get_name_dir();
    roomName[1]= malloc(5 * sizeof(char));
    roomName[2]= malloc(5 * sizeof(char));
   	roomName[3]= malloc(5 * sizeof(char));
   	roomName[4]= malloc(5 * sizeof(char));
   	roomName[5]= malloc(5 * sizeof(char));
   	roomName[6]= malloc(5 * sizeof(char));
   	roomName[0]= malloc(5 * sizeof(char));
	roomName[0]= "0";
	roomName[1]= "1";
	roomName[2]= "2";
	roomName[3]= "3";
	roomName[4]= "4";
	roomName[5]= "5";
	roomName[6]= "6";

	//make and enter the directory
	mkdir(name, 0777);
	chdir(name);
	//make 7 files for 7 rooms
	for (i = 0; i < 7; i++) {
		FILE *fp = fopen(roomName[i], "w");
		fprintf(fp, "ROOM NAME: %s\n", roomName[i]);
		//close file
		fclose(fp);
	}
		//back to upper dir and free memory
		chdir("..");
//		free(name);
		return 0;
}
