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

#define NUM_ROOMS 7

const char *room_names[NUM_ROOMS] = {
	"room1", "room2", "room3", "room4",	"room5", "room6", "room7"
};

enum room_type {
	START = 0,
	MID,
	END
};

struct room {
	 enum room_type type;
	 const char *name;
};

int main(){
	FILE *testFile;
	testFile = fopen("test.txt", "w+");
	printf("testing\n");	
	fclose(testFile);
	//struct room testRoom;
	return 0;
}
