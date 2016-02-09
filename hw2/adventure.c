#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>


//read one room, return it
struct room read_one_room(char *file_name);

//define room names
const char *room_names[10] = {
	"room0", 
	"room1", 
	"room2", 
	"room3",	
	"room4", 
	"room5", 
	"room6",
	"room7",
	"room8",
	"room9"
};

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

//initial all rooms
struct room all_room[7];

//generate all rooms attributes
struct room *generate_rooms(){
	int i,j;
	//set flags all to 0 means not taken
	int room_flag[10];
	for (i = 0; i < 10; i++){
		room_flag[i] = 0;
	}
	for (i = 0; i < 7; i++){
		all_room[i].con_num = 0;
	}
	//initial all 7 rooms
	for (i = 0; i < 7; i++) {
		//set all rooms to mid room first
		all_room[i].type = mid;
		//initial num of connection to 0
	//	all_room[i].con_num = 0;
		//set connection numbers, 3-6
		all_room[i].con_max = rand()%4;
		all_room[i].con_max += 3;
		//pick different names to rooms
		while (1){
			// from room0 to room9
			int which_room = rand()%10;
			//test the flag and set flag to used = 1
			if (room_flag[which_room] == 0){
				room_flag[which_room] = 1;
				all_room[i].name = room_names[which_room];
				break;
			}
		}
	}
	//connect all 7 rooms as the con_max
	for (i = 0; i < 7; i++) {//each one of the room
		for (j = 0; j < all_room[i].con_max; j++){
			//choose a random room to connect
			int room_will_connect = rand()%7;
			//if the room already connected, choose another room
			while (connect_room(i, room_will_connect, all_room) == 1){
				room_will_connect = rand()%7;
		//		printf("room_wiil_connect is:%d\n", room_will_connect);//debug
			}
		}
	}
	//set start and end rooms
	all_room[0].type = start;
	all_room[6].type = end;
	return all_room;
}


//get the name of the dir
char *get_name_dir() {
	//process id
	pid_t id = getpid();
	//length of my dir name = jiajun.room.+process_id = 12+20 = 32
	char *name = malloc(32 * sizeof(char));
	sprintf(name, "jiajun.rooms.%d", id);
	return name;

}

//generate room files
void create_files(struct room rooms[7]){
	int i,j;
	char *name = get_name_dir();
	//make and enter the directory
	mkdir(name, 0777);
	chdir(name);
	//make 7 files for 7 rooms
	for (i = 0; i < 7; i++) {
		//open file
		FILE *fp = fopen(rooms[i].name, "w");
		//input file name
		fprintf(fp, "ROOM NAME: %s\n", rooms[i].name);
		//input room type
		if (rooms[i].type == start)
			fprintf(fp, "ROOM TYPE: START_ROOM\n");
		else if (rooms[i].type == mid)
			fprintf(fp, "ROOM TYPE: MID_ROOM\n");
		else if (rooms[i].type == end)
			fprintf(fp, "ROOM TYPE: END_ROOM\n");
		//input all connections
		for (j = 0; j < rooms[i].con_max; j++) {
			fprintf(fp, "CONNECTION %d: %s\n", (j+1), rooms[i].connection[j]->name);
		}

		//close file
		fclose(fp);
	}
		//back to upper dir and free memory
		chdir("..");
		free(name);
}

//test function for 2 rooms
int test_connection(int a, int b){
	int i;
	//if they are the same room, return singal 1 
	if (a == b)
		return 1;
	//test each connection of room_i to see if any of them are room_will_connect
	for (i = 0; i < all_room[a].con_max; i++) {
		if ((all_room[a].connection[i] == &all_room[b]) && (all_room[a].connection[i] != NULL)) {	
			return 1;
		}
	}
	for (i = 0; i < all_room[b].con_max; i++){
		if ((all_room[b].connection[i] == &all_room[a]) && (all_room[b].connection[i] != NULL))
			return 1;
	}
	return 0;
}


//connection room function
int connect_room(int room_i, int room_will_connect, struct room all_room[7]){
	//set 2 room pointers point to two input rooms
	struct room *room1 = &all_room[room_i];
    struct room *room2 = &all_room[room_will_connect];
	//if number of connection reachers the max, return 0 represent for succeed
	if (room1->con_num == 6) 
		return 0;
	//if they are connected, return 1 represent for fail
	if (test_connection(room_i, room_will_connect) == 1)
		return 1;
	//if the room will be connected reaches the max, return 1 for fail
/*	if ((room2->con_num > room2->con_max)|| (room1->con_num > room1->con_max))
		
		return 1;//wrong*/
	// Connecting
	room1->connection[room1->con_num] = room2;
	room2->connection[room2->con_num] = room1;
	//return succeed signal 0
	//increase the number of connections
	room1->con_num++;
	room2->con_num++;
//	if (room2->con_num > room2->con_max){
//		room2->con_num--;
//		printf("2\n");//debug
//	}
//	printf("con_num for room1 is: %d\n",room1->con_num);	
//	printf("con_num for room2 is: %d\n",room2->con_num);
	return 0;
}

//debug print
void print(){
	int i;
	for (i=0; i<7; i++)
		printf("num_con is: %d\nnum_max is: %d\n",all_room[i].con_num,all_room[i].con_max);
}

//read room file to memory function
struct room* read_rooms(){
	int i = 0;
	char *dir_name = get_name_dir();
    struct room *rooms = malloc(7 * sizeof(struct room));
	chdir(dir_name);//debug
	DIR *dp;	
	struct dirent *dir;
	dp = opendir (".");
	while ((dir = readdir (dp))!= NULL) {
		if (strcmp(dir->d_name, ".") && strcmp(dir->d_name, "..")){

//		printf("file name is: %s\n", dir->d_name);
 		rooms[i] = read_one_room(dir->d_name);
			i++;
		}
	}
	closedir (dp);
	free(dir_name);
	chdir("..");
	return rooms;
}

//read one room file to a struct
struct room read_one_room(char *file_name){
	int i, k, j=0;
	struct room room;
	char str[250];
	FILE *fp = fopen(file_name, "r");
	//read the name 
	fgets(str, 250, fp);
	char* token1 = strtok(str, ":");
	token1 = strtok(NULL, " \t\r\n");
	//room.name = malloc(sizeof(char)*5);
	char *name = malloc(sizeof(char)*5);
	strcpy (name, token1);
	room.name = name;
	//read the type i dont know why do i have to call it twice here
	fgets(str,250,fp);

	char* token2 = strtok(str, ":");

	token2 = strtok(NULL, " \t\r\n");
//	printf("room.type is: %s|\n", token2);

	if (!strcmp(token2, "START_ROOM"))
		room.type = start;
	else if (!strcmp(token2, "END_ROOM"))
		room.type = end;
	else if (!strcmp(token2, "MID_ROOM"))
		room.type = mid;
//	printf ("room type is: %d\n", room.type);

	//read the connections
//	for (k = 0; k < 4; k++){
	while(1){

		fgets(str,250,fp);
		if (feof(fp))
			break;
		char* token3 = strtok(str, ":");
		token3 = strtok(NULL, " \t\r\n");
//		printf("token3 is: %s\n",token3);

		for (i = 0; i < 7; i++){
			if(!strcmp(token3 ,all_room[i].name)){
				room.connection[j] = &all_room[i];
				j++;

				//printf("connections: %s\n", room.connection[j]->name);
			}
		}
	}	

	room.con_num = j;
	room.con_max = j;
	fclose(fp);
	return room;
}

void end_of_game(int steps, struct room ** visited, int count){
	int i=0;
	
	printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");
	printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n", steps);
	for (i = 0; i < count; i++) {
		printf("%s\n", visited[i]->name);
	}
}

void game(struct room* read_rooms){
	int i;
	int flag = 1;
	//starting room is the first one at the room array
	struct room *current_room = &read_rooms[0];		
	//for visited rooms. choose a big number 30
	int size = 30;
	struct room **visited = malloc(size*sizeof(struct room*));
	//count how many rooms visited
	int count = 0;
	//how many steps user uses
	int steps = 0;
	//input of user
	char input[5];
	//game loop forever
	while(1){
		while(flag == 1){
			flag = 0;	
			//check if game is end
			if(current_room->type == 2){
				end_of_game(steps, visited, count);
				free(visited);
				return;
			}
			//print game info
			printf("\nCURRENT LOCATION: %s\n", current_room->name);
			printf("POSSIBLE CONNECTIONS: ");
			for (i = 0; i < current_room->con_max-1; i++) {
				printf("%s, ", current_room->connection[i]->name);
			}
			printf("%s.\n", current_room->connection[current_room->con_max-1]->name);
			//user input
			printf("WHERE TO? >");
			scanf("%s", &input);
			//check input
			for (i = 0; i < current_room->con_max; i++) {
				if (strncmp(input, current_room->connection[i]->name, 5) == 0){
					current_room = current_room->connection[i];
					//reallocate size
					if (count >= size){
						size = size + size;
						visited = realloc(visited, size*sizeof(struct room*));
					}
					visited[count] = current_room;
					count++;
					steps++;
					flag = 1;
				}
			}
		}
		//if cannot find match
		printf("\nHUH? I DON’T UNDERSTAND THAT ROOM. TRY AGAIN.\n");
	}

}

//free memory 
void clean (struct room *r){
	free(r);
}

//main
int main(){
	//initial random thing
	srand(time(0));
	//generate rooms
	generate_rooms();
	//create files
	create_files(all_room);
	//read rooms from a file
	struct room *read_all_rooms = read_rooms();
	//start game with the reading rooms
	game(read_all_rooms);
	//free memory
	free(read_all_rooms);
	return 0;
}


