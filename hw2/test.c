#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>

//define room types
enum room_type {
	start = 0,
	mid,
	end
};

//define room structure
struct room {
	 enum room_type type;
	 const char *name;
	 int con_num;
	 int con_max;
	 struct room *connection[7];
};
/*

struct room* read_rooms(){
	int i = 0;
	char dir_name[] = "room1";
//	struct room rooms[7];
    struct room *rooms = malloc(7 * sizeof(struct room));
	chdir(dir_name);
	//fix me:change names
	DIR *dp;	
	struct dirent *dir;
	dp = opendir (".");
	while (dir = readdir (dp)) {
 		rooms[i] = read_one_room(dir->d_name);
		i++;
	}
	printf("i shoudl be 7, but it is: %d\n", i);//debug
	closedir (dp);
	free(dir_name);
	chdir("..");
	return rooms;
}
*/
//read one room file to a struct
struct room read_one_room(char *file_name){
	int i, k, j=0;
	struct room room;
	char str[250];
	FILE *fp = fopen(file_name, "r");
	//read the name 
	fgets(str, 250, fp);
	char* token = strtok(str, ":");
	token = strtok(NULL, " \t\r\n");
	room.name = token;
	printf("room.name is: %s|\n", room.name);
	//read the type i dont know why do i have to call it twice here
	fgets(str,250,fp);
	token = strtok(str, ":");
	token = strtok(NULL, " \t\r\n");
	printf("room.type is: %s|\n", token);
	if (!strcmp(token, "START_ROOM"))
		room.type = start;
	else if (!strcmp(token, "END_ROOM"))
		room.type = end;
	else if (!strcmp(token, "MID_ROOM"))
		room.type = mid;
	printf ("room type is: %d\n", room.type);
	//read the connections
	for (k = 0; k < 4; k++){
		fgets(str,250,fp);
		token = strtok(str, ":");
		token = strtok(NULL, " \t\r\n");
	for (i = 0; i < 7; i++){
			if(!strcmp(token,all_room[i].name)){
				room.connection[j] = &all_room[i];
				j++;
			}
		}
	}	
	room.con_num = j;
	room.con_max = j;
//	printf("room.max is: %d\n", room.con_max);//debug
//	printf("room.connection[1] is:%d\n", room.connection[1]);
	fclose(fp);
	return room;
}


int main(){
	
	chdir("jiajun.rooms.16129");
	char str[5] = "room1";
	read_one_room(str);
	return 0;
}
