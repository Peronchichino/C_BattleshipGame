/** \file
 * \author Lukas.Buchmayer
 * \brief
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <stddef.h>

char lbl[13] = {"ABCDEFGHIJKLM"};

/**
 * \brief Game statistics of player
 */
typedef struct stats{
	unsigned int hits;
	unsigned int shots;
	unsigned int win;
	unsigned int loss;
	unsigned int game;
}Stats;

/**
 * \brief Ship specifications
 */
typedef struct ships {
	char symbol;
	char *name;
	int amount;
	int length;
}Ships;

/**
 * \brief Open a File and print start screen\n
 * If File path does not exist exit program
 */
void startScreen(){
	FILE *fptr;
	char filename[] = "firstscreen.txt";
	char c;
	fptr = fopen(filename, "r");
	if(fptr == NULL){
		exit(0);
	}
	c = fgetc(fptr);
	while(c != EOF){
		printf("%c", c);
		c = fgetc(fptr);
	}
	fclose(fptr);
}

/**
 * \brief Set profile name depending on choice to create or load an existing profile\n
 *
 * \param profile name of textfile for game statistics
 */
void loadProfile(char profile[CHAR_MAX]){
	while(1){
		getchar();
		char name[CHAR_MAX], crlo;
		printf("Would you like to load an existing profile or create a new one?\n");
		printf("Load (L/l) / Create (C/c):");
		crlo = getchar();
		if(crlo == 76 || crlo == 108){
			printf("Profile:");
			scanf("%s", name);
			strcpy(profile, name);
			strcat(profile, ".txt");
			FILE *fptr;
			fptr = fopen(profile, "r");
			if(fptr == NULL){
				printf("Cannot open profile.\n");
				continue;
			} else {
				printf("Chosen profile: %s\n", name);
				char c = fgetc(fptr);
				while(c != EOF){
					printf("%c", c);
					c = fgetc(fptr);
				}
				printf("\n");
				break;
			}
		} else if(crlo == 67 || crlo == 99){
			printf("New profile name:");
			scanf("%s", name);
			strcpy(profile, name);
			strcat(profile, ".txt");
			break;
		} else {
			printf("\nCheck input.\n");
			continue;
		}
	}
}

/**
 * \brief Open a File to save game statistics onto new profile or existing profile
 *
 * \param profile name of textfile for game statistics
 * \param player game/player statistics
 */
void saveProfile(char profile[CHAR_MAX], Stats* player){
	FILE *fptr;
	fptr = fopen(profile, "r");
	if(fptr == NULL){
		fptr = fopen(profile, "w");
		if(fptr == NULL){
			printf("Cant create profile");
			exit(1);
		}
		fclose(fptr);
		fptr = fopen(profile, "r");
	}
	char rep[CHAR_MAX];
	strcpy(rep, profile);
	char games[]="Games", won[]="Wins", lost[]="Losses", shot[]="Shots", hit[]="Hits", ratio[]="Percentage";
	double rat = (double)player->hits/player->shots;
	char c = 0;
	int count = 0;
	while (c != EOF) {
		if (c == '\n') {
			count++;
		}
		c = (signed char) fgetc(fptr);
	}
	fclose(fptr);
	if(count == 0){
		fptr = fopen(profile, "w");
		fprintf(fptr, "%s %d\n", games, player->game);
		fprintf(fptr, "%s %d\n", won, player->win);
		fprintf(fptr, "%s %d\n", lost, player->loss);
		fprintf(fptr, "%s %d\n", shot, player->shots);
		fprintf(fptr, "%s %d\n", hit, player->hits);
		fprintf(fptr, "%s %lf\n", ratio, rat);
		fclose(fptr);
	} else {
		FILE *ptr;
		ptr = fopen("repos.txt", "w");
		fptr = fopen(profile, "r");
		int x, sum, s, h;
		double calc;
		char buffer[CHAR_MAX], line[5];
		while(fgets(line, sizeof(line), fptr)){
			fscanf(fptr, "%s %d", buffer, &x);
			if(strcmp(buffer, "s") == 0){
				sum = x + player->game;
				fprintf(ptr, "%s %d\n", games, sum);
			} else if(strcmp(buffer, "Wins") == 0){
				sum = x + player->win;
				fprintf(ptr, "%s %d\n", won, sum);
			} else if(strcmp(buffer, "Losses") == 0){
				sum = x + player->loss;
				fprintf(ptr, "%s %d\n", lost, sum);
			}else if(strcmp(buffer, "Shots") == 0){
				sum = x + player->shots;
				s = sum;
				fprintf(ptr, "%s %d\n", shot, sum);
			}else if(strcmp(buffer, "Hits") == 0){
				sum = x + player->hits;
				h = sum;
				fprintf(ptr, "%s %d\n", hit, sum);
			}
		}
		calc = (double)h/s;
		fprintf(ptr, "%s %lf", ratio, calc);
		fclose(fptr);
		fclose(ptr);
		rename(profile, "delete.txt");
		rename("repos.txt", rep);
		remove("delete.txt");
	}
}

/**
 * \brief Player firing on PC ships/map
 *
 * \param buf game field size
 * \param mapP PC map seen by player
 * \param grid PC map with pc ships, not seen by player
 * \param pcShips PC ship count
 * \param player game/player statistics
 */
void playerTurn(int buf, char mapP[][buf], char grid[][buf], int *pcShips, Stats* player){
	printf("\nYour turn!\n");
	char x, y, cord[3];
	int row, col;
	do{
		printf("Cords for shot (x,y):");
		scanf("%s", cord);
		x = cord[0];
		y = cord[2];
		row = y - 97;
		col = x - 97;
		if((row >= 0 || row <= buf) && (col >= 0 && col <= buf)){
			if(grid[row][col] == 'S'){
				printf("Congrats! You hit a ship!\n");
				mapP[row][col] = 'X';
				(*pcShips)--;
				player->hits++;
				player->shots++;
				break;
			} else if(grid[row][col] == '-' || grid[row][col] == '.'){
				printf("You missed...\n");
				mapP[row][col] = 'O';
				player->shots++;
				break;
			} else if(grid[row][col] == 'X' || grid[row][col] == 'O'){
				printf("Cant shoot at a place you have shot at previously... try again\n");
				continue;
			}
		} else if((row < 0 || row > buf) || (col < 0 || col > buf)){
			printf("You cant fire outside of the map, try again\n");
			continue;
		}
	} while(1);
}

/**
 * \brief PC firing on player ships/map
 *
 * \param buf game field size
 * \param map player map
 * \param playerShips player ship count
 */
void pcTurn(int buf, char map[][buf], int *playerShips){
	printf("\nPCs turn!\n");
	int row, col, up = buf;
	int lo = 0;
	srand(time(NULL));
	do{
		row = (rand() % (up - lo + 1) + lo);
		col = (rand() % (up - lo + 1) + lo);
		if(map[row][col] == 'S'){
			printf("PC has hit one of your ships!\n");
			map[row][col] = 'X';
			(*playerShips)--;
			break;
		} else if(map[row][col] == '-'){
			printf("PC has missed!\n");
			map[row][col] = 'O';
			break;
		} else if(map[row][col] == 'X'){
			continue;
		} else if(map[row][col] == 'O'){
			continue;
		}
	} while(1);
}

/**
 * \brief Player ship deployment
 *
 * \param buf game field size
 * \param map player map
 * \param playerShips player ship count
 * \param carr carrier ship type, 1x5
 * \param batt battleship ship type, 1x4
 * \param subm submarine ship type, 1x3
 * \param dest destroyer ship type, 1x2
 */
void deployPlayer(int buf, char map[][buf], int playerShips, Ships carr, Ships batt, Ships subm, Ships dest){
	char x, y, cord[3], symb;
	int row, col, dir, i = 0, j = 0, h = 0, k = 0 , l = 0;

	while(i < playerShips){
		getchar();
		printf("\nEnter ship symbol to place:");
		symb = getchar();
		if((symb == carr.symbol && carr.amount == 0) || (symb == batt.symbol && batt.amount == 0)){
			printf("You cant place this ship in this difficulty.\n");
		}
		if(symb == carr.symbol && carr.amount != 0){
			if(j < carr.amount){
				while(1){
					printf("Chosen ship: %s\n", carr.name);
					printf("Start coordinate(x,y):");
					scanf("%s", cord);
					x = cord[0];
					y = cord[2];
					row = y - 97;
					col = x - 97;
					printf("Ship direction(1-4):");
					scanf("%d", &dir);
					if((row >= 0 && row <= buf) && (col >= 0 && col <= buf) && map[row][col] == '-'){
						if(dir == 1 && (map[row][col + 1] == '-' && map[row][col + 2] == '-') && (map[row][col + 3] == '-' && map[row][col + 4] == '-')){
							for(int count = 0; count < carr.length; count++){
								map[row][col + count] = 'S';
							}
							j++;
							break;
						} else if(dir == 2 && (map[row + 1][col] == '-' && map[row + 2][col] == '-') && (map[row + 3][col] == '-' && map[row + 4][col] == '-')){
							for(int count = 0; count < carr.length; count++){
								map[row + count][col] = 'S';
							}
							j++;
							break;
						} else if(dir == 3 && (map[row][col - 1] == '-' && map[row][col - 2] == '-') && (map[row][col - 3] == '-' && map[row][col - 4] == '-')){
							for(int count = 0; count < carr.length; count++){
								map[row][col - count] = 'S';
							}
							j++;
							break;
						} else if(dir == 4 && (map[row - 1][col] == '-' && map[row - 2][col] == '-') && (map[row - 3][col] == '-' && map[row - 4][col] == '-')){
							for(int count = 0; count < carr.length; count++){
								map[row - count][col] = 'S';
							}
							j++;
							break;
						} else if(dir < 1 || dir > 4){
							printf("Incorrect direction, please try again.\n");
							continue;
						} else{
							printf("Your ship is going outside of the map bounds or intercepting another ship, try again.\n");
							continue;
						}
					} else if(map[row][col] == 'S'){
						printf("You cant place a ship ontop of a ship... try again\n");
						continue;
					} else if((row < 0 || row > buf) || (col < 0 || col > buf)){
						printf("You cant place ships outside of the map, try again.\n");
						continue;
					}
				}
			} else {
				printf("You cant place any more of this ship.");
			} i = i + 5;
		} else if(symb == batt.symbol && batt.amount != 0){
			if(h < batt.amount){
				while(1){
					printf("Chosen ship: %s\n", batt.name);
					printf("Start coordinate(x,y):");
					scanf("%s", cord);
					x = cord[0];
					y = cord[2];
					row = y - 97;
					col = x - 97;
					printf("Ship direction(1-4):");
					scanf("%d", &dir);
					scanf("%d", &dir);
					if((row >= 0 && row <= buf) && (col >= 0 && col <= buf) && map[row][col] == '-'){
						if(dir == 1 && (map[row][col + 1] == '-' && map[row][col + 2] == '-') && map[row][col + 3] == '-'){
							for(int count = 0; count < batt.length; count++){
								map[row][col + count] = 'S';
							}
							h++;
							break;
						} else if(dir == 2 && (map[row + 1][col] == '-' && map[row + 2][col] == '-') && map[row + 3][col] == '-'){
							for(int count = 0; count < batt.length; count++){
								map[row + count][col] = 'S';
							}
							h++;
							break;
						} else if(dir == 3 && (map[row][col - 1] == '-' && map[row][col - 2] == '-') && map[row][col - 3] == '-'){
							for(int count = 0; count < batt.length; count++){
								map[row][col - count] = 'S';
							}
							h++;
							break;
						} else if(dir == 4 && (map[row - 1][col] == '-' && map[row - 2][col] == '-') && map[row - 3][col] == '-'){
							for(int count = 0; count < batt.length; count++){
								map[row - count][col] = 'S';
							}
							h++;
							break;
						} else if(dir < 1 || dir > 4){
							printf("Incorrect direction, please try again.\n");
							continue;
						} else{
							printf("Your ship is going outside of the map bounds or intercepting another ship, try again.\n");
							continue;
						}
					} else if(map[row][col] == 'S'){
						printf("You cant place a ship ontop of a ship... try again\n");
						continue;
					} else if((row < 0 || row > buf) || (col < 0 || col > buf)){
						printf("You cant place ships outside of the map, try again.\n");
						continue;
					}
				}
			} else {
				printf("You cant place any more of this ship.");
			}i = i + 4;
		} else if(symb == subm.symbol && subm.amount != 0){
			if(k < subm.amount){
				while(1){
					printf("Chosen ship: %s\n", subm.name);
					printf("Start coordinate(x,y):");
					scanf("%s", cord);
					x = cord[0];
					y = cord[2];
					row = y - 97;
					col = x - 97;
					printf("Ship direction(1-4):");
					scanf("%d", &dir);
					if((row >= 0 && row <= buf) && (col >= 0 && col <= buf) && map[row][col] == '-'){
						if(dir == 1 && (map[row][col + 1] == '-' && map[row][col + 2] == '-')){
							for(int count = 0; count < subm.length; count++){
								map[row][col + count] = 'S';
							}
							k++;
							break;
						} else if(dir == 2 && (map[row + 1][col] == '-' && map[row + 2][col] == '-')){
							for(int count = 0; count < subm.length; count++){
								map[row + count][col] = 'S';
							}
							k++;
							break;
						} else if(dir == 3 && (map[row][col - 1] == '-' && map[row][col - 2] == '-')){
							for(int count = 0; count < subm.length; count++){
								map[row][col - count] = 'S';
							}
							k++;
							break;
						} else if(dir == 4 && (map[row - 1][col] == '-' && map[row - 2][col] == '-')){
							for(int count = 0; count < subm.length; count++){
								map[row - count][col] = 'S';
							}
							k++;
							break;
						} else if(dir < 1 || dir > 4){
							printf("Incorrect direction, please try again.\n");
							continue;
						} else{
							printf("Your ship is going outside of the map bounds or intercepting another ship, try again.\n");
							continue;
						}
					} else if(map[row][col] == 'S'){
						printf("You cant place a ship ontop of a ship... try again\n");
						continue;
					} else if((row < 0 || row > buf) || (col < 0 || col > buf)){
						printf("You cant place ships outside of the map, try again.\n");
						continue;
					}
				}
			} else {
				printf("You cant place any more of this ship.");
			} i = i + 3;
		} else if(symb == dest.symbol && dest.amount != 0){
			if(l < dest.amount){
				while(1){
					printf("Chosen ship: %s\n", dest.name);
					printf("Start coordinate(x,y):");
					scanf("%s", cord);
					x = cord[0];
					y = cord[2];
					row = y - 97;
					col = x - 97;
					printf("Ship direction(1-4):");
					scanf("%d", &dir);
					if((row >= 0 && row <= buf) && (col >= 0 && col <= buf) && map[row][col] == '-'){
						if(dir == 1 && map[row][col + 1] == '-'){
							for(int count = 0; count < dest.length; count++){
								map[row][col + count] = 'S';
							}
							l++;
							break;
						} else if(dir == 2 && map[row + 1][col] == '-'){
							for(int count = 0; count < dest.length; count++){
								map[row + count][col] = 'S';
							}
							l++;
							break;
						} else if(dir == 3 && map[row][col - 1] == '-'){
							for(int count = 0; count < dest.length; count++){
								map[row][col - count] = 'S';
							}
							l++;
							break;
						} else if(dir == 4 && map[row - 1][col] == '-'){
							for(int count = 0; count < dest.length; count++){
								map[row - count][col] = 'S';
							}
							l++;
							break;
						} else if(dir < 1 || dir > 4){
							printf("Incorrect direction, please try again.\n");
							continue;
						} else{
							printf("Your ship is going outside of the map bounds or intercepting another ship, try again.\n");
							continue;
						}
					} else if(map[row][col] == 'S'){
						printf("You cant place a ship ontop of a ship... try again\n");
						continue;
					} else if((row < 0 || row > buf) || (col < 0 || col > buf)){
						printf("You cant place ships outside of the map, try again.\n");
						continue;
					}
				}
			} else{
				printf("You cant place any more of this ship.");
			} i = i + 2;
		}
	}
}

/**
 * \brief PC ship deployment
 *
 * \param buf game field size
 * \param grid PC ship map, not seen by player
 * \param carr carrier ship type, 1x5
 * \param batt battleship ship type, 1x4
 * \param subm submarine ship type, 1x3
 * \param dest destroyer ship type, 1x2
 */
void deployPC(int buf, char grid[][buf], int pcShips, Ships carr, Ships batt, Ships subm, Ships dest){
	int row, col, dir, i = 0, j = 0, h = 0, k = 0 , l = 0, low = 0, upd = 2, lowd = 1;
	srand(time(NULL));
	while(i < pcShips){
		if(carr.amount != 0 && j < carr.amount){
			while(1){
				row = (rand() % (buf - low + 1) + low);
				col = (rand() % (buf - low + 1) + low);
				dir = (rand() % (upd - lowd + 1) + lowd);
				if((row >= 0 && row <= (buf - carr.length)) && (col >= 0 && col <= (buf - carr.length)) && grid[row][col] == '-'){
					if(dir == 1 && (grid[row][col + 1] == '-' && grid[row][col + 2] == '-') && (grid[row][col + 3] == '-' && grid[row][col + 4] == '-')){
						for(int count = 0; count < carr.length; count++){
							grid[row][col + count] = 'S';
						}
						j++;
						break;
					} else if(dir == 2 && (grid[row + 1][col] == '-' && grid[row + 2][col] == '-') && (grid[row + 3][col] == '-' && grid[row + 4][col] == '-')){
						for(int count = 0; count < carr.length; count++){
							grid[row + count][col] = 'S';
						}
						j++;
						break;
					} else{ continue;}
				} else if(grid[row][col] == 'S'){ continue;}
			} i = i + 5;
		} else if(batt.amount != 0 && h < batt.amount){
			while(1){
				row = (rand() % (buf - low + 1) + low);
				col = (rand() % (buf - low + 1) + low);
				dir = (rand() % (upd - lowd + 1) + lowd);
				if((row >= 0 && row <= (buf - batt.length)) && (col >= 0 && col <= (buf - batt.length)) && grid[row][col] == '-'){
					if(dir == 1 && (grid[row][col + 1] == '-' && grid[row][col + 2] == '-') && grid[row][col + 3] == '-'){
						for(int count = 0; count < batt.length; count++){
							grid[row][col + count] = 'S';
						}
						h++;
						break;
					} else if(dir == 2 && (grid[row + 1][col] == '-' && grid[row + 2][col] == '-') && grid[row + 3][col] == '-'){
						for(int count = 0; count < batt.length; count++){
							grid[row + count][col] = 'S';
						}
						h++;
						break;
					} else{ continue;}
				} else if(grid[row][col] == 'S'){ continue;}
			} i = i + 4;
		} else if(subm.amount != 0 && k < subm.amount){
			while(1){
				row = (rand() % (buf - low + 1) + low);
				col = (rand() % (buf - low + 1) + low);
				dir = (rand() % (upd - lowd + 1) + lowd);
				if((row >= 0 && row <= (buf - subm.length)) && (col >= 0 && col <= (buf - subm.length)) && grid[row][col] == '-'){
					if(dir == 1 && (grid[row][col + 1] == '-' && grid[row][col + 2] == '-')){
						for(int count = 0; count < subm.length; count++){
							grid[row][col + count] = 'S';
						}
						k++;
						break;
					} else if(dir == 2 && (grid[row + 1][col] == '-' && grid[row + 2][col] == '-')){
						for(int count = 0; count < subm.length; count++){
							grid[row + count][col] = 'S';
						}
						k++;
						break;
					} else{ continue;}
				} else if(grid[row][col] == 'S'){ continue;}
			} i = i + 3;
		} else if(dest.amount != 0 && l < dest.amount){
			while(1){
				row = (rand() % (buf - low + 1) + low);
				col = (rand() % (buf - low + 1) + low);
				dir = (rand() % (upd - lowd + 1) + lowd);
				if((row >= 0 && row <= (buf - dest.length)) && (col >= 0 && col <= (buf - dest.length)) && grid[row][col] == '-'){
					if(dir == 1 && grid[row][col + 1] == '-'){
						for(int count = 0; count < dest.length; count++){
							grid[row][col + count] = 'S';
						}
						l++;
						break;
					} else if(dir == 2 && grid[row + 1][col] == '-'){
						for(int count = 0; count < dest.length; count++){
							grid[row + count][col] = 'S';
						}
						l++;
						break;
					} else{continue;}
				} else if(grid[row][col] == 'S'){ continue;}
			} i = i + 2;
		}
	}
}
/**
 * \brief PC map and player map initialisation
 *
 * \param buf game field size
 * \param map player map
 * \param grid PC ship map, not seen by player
 * \param mapP PC ship map without ship locations, seen by player
 */
void createMap(int buf, char map[][buf], char mapP[][buf], char grid[][buf]){
	for(int i = 0; i < buf; i++){
		for(int j = 0; j < buf; j++){
			map[i][j] = '-';
			mapP[i][j] = '-';
			grid[i][j] = '-';
		}
	}
}
/**
 * \brief Print player map with ship location and PC shot location
 *
 * \param dif game difficulty for labels
 * \param buf game field size
 * \param map player map
 */
void printMap(int dif, int buf, char map[][buf]){
	if(dif == 1){
		printf("+ A B C D E\n");
	} else if(dif == 2){
		printf("+ A B C D E F G\n");
	} else if(dif == 3){
		printf("+ A B C D E F G H I J\n");
	} else {
		printf("+ A B C D E F G H I J K L M\n");
	}
	for(int i = 0; i < buf; i++){
		printf("%c", lbl[i]);
		for(int j = 0; j < buf; j++){
			printf(" %c", map[i][j]);
		}
		printf("\n");
	}
}

/**
 * \brief Print PC map, not grid, doesnt show shit locations only player hits/misses
 *
 * \param dif game difficulty for labels
 * \param buf game field size
 * \param mapP PC ship map without ship locations, seen by player
 */
void printPC(int dif, int buf, char mapP[][buf]){
	if(dif == 1){
		printf("+ A B C D E\n");
	} else if(dif == 2){
		printf("+ A B C D E F G\n");
	} else if(dif == 3){
		printf("+ A B C D E F G H I J\n");
	} else {
		printf("+ A B C D E F G H I J K L M\n");
	}
	for(int i = 0; i < buf; i++){
		printf("%c", lbl[i]);
		for(int j = 0; j < buf; j++){
			printf(" %c", mapP[i][j]);
		}
		printf("\n");
	}
}

/**
 * \brief Message that tells player whether they lost or won
 *
 * \param player game statistics
 * \param playerShips player ship count
 * \param pcShips PC ship count
 */
void gameOver(Stats* player, int playerShips, int pcShips){
	if(playerShips > 0 && pcShips == 0){
		printf("Congratulations! You won!\n");
		player->win++;
	} else if(playerShips == 0 && pcShips > 0){
		printf("Sorry captain, it seems you lost this battle...\n");
		player->loss++;
	}
	player->game++;
	printf("The statistics of this game will be saved to your profile\n");
}

/**
 * \brief Main, play battleship game
 *
 * \return 0 if no error else 1
 */
int main(void){
	Stats player = {.game = 0, .hits = 0, .loss = 0, .shots = 0, .win = 0};
	Ships carr, batt, subm, dest;
	startScreen();
	char profile[CHAR_MAX];
	while(1){
		char loadProfileYN; //yes no
		printf("Would you like to load a profile to save/load your stats? \n");
		printf("Y/N:");
		loadProfileYN = getchar();
		if(loadProfileYN == 89 || loadProfileYN == 121){ //ascii for Y and y
			printf("\n");
			loadProfile(profile);
			break;
		} else if(loadProfileYN == 78 || loadProfileYN == 110){ //ascii for N and n
			printf("No profile chosen, continuing as a guest player\n\n");
			break;
		} else {
			printf("Please check input\n");
			continue;
		}
	}

	int diff, pcShips, playerShips, buff;

	carr = (Ships){.name = "Carrier", .symbol = 'c', .length = 5};
	batt = (Ships){.name = "Battleship", .symbol = 'b', .length = 4};
	subm = (Ships){.name = "Submarine", .symbol = 's', .length = 3};
	dest = (Ships){.name = "Destroyer", .symbol = 'd', .length = 2, .amount = 2};

	printf("Please choose a difficulty to get started!\n");
	printf("Easy 5x5: 1 | Medium 7x7: 2 | Hard 10x10: 3 | Ultra 13x13: 4\n");
	printf("Your Choice:");
	scanf(" %d", &diff);
	if(diff == 1){
		buff = 5;
		carr.amount = 0;
		batt.amount = 0;
		subm.amount = 1;
	} else if(diff == 2){
		buff = 7;
		carr.amount = 0;
		batt.amount = 1;
		subm.amount = 2;
	} else if(diff == 3){
		buff = 10;
		carr.amount = 1;
		batt.amount = 1;
		subm.amount = 2;
	} else if(diff == 4){
		buff = 13;
		carr.amount = 1;
		batt.amount = 2;
		subm.amount = 2;
	}

	playerShips = ((batt.amount * batt.length) + (batt.amount * batt.length) + (subm.amount * subm.length) + (dest.amount * dest.length));
	pcShips = ((batt.amount * batt.length) + (batt.amount * batt.length) + (subm.amount * subm.length) + (dest.amount * dest.length));

	char map[buff][buff], grid[buff][buff], mapP[buff][buff];

	createMap(buff, map, mapP, grid);

	printf("\nThe sea is currently empty... lets get started by placing some ships.");
	deployPlayer(buff, map, playerShips, carr, batt, subm, dest);
	printf("Your ships:\n");
	printMap(diff, buff, map);
	printf("\n");
	deployPC(buff, grid, pcShips, carr, batt, subm, dest);

	srand(time(NULL));
	int up = 10, low = 1;
	int turn = (rand() % (up - low + 1) + low);
	if(turn >= 1 && turn < 6){
		printf("\n-------------------------------");
		printf("\nYou get to go first!\n");
		do {
			playerTurn(buff, mapP, grid, &pcShips, &player);
			printf("\nPC Board:\n");
			printPC(diff, buff, mapP);

			pcTurn(buff, map, &playerShips);
			printf("\nPlayer Board:\n");
			printMap(diff, buff, map);

			printf("\nYour Ships: %d | PC Ships: %d\n", playerShips, pcShips);
			printf("-------------------------------\n");
		} while(playerShips != 0 && pcShips != 0);
	} else if(turn >= 6 && turn < 11){
		printf("\n-------------------------------");
		printf("\nPC gets to go first\n");
		do {
			pcTurn(buff, map, &playerShips);
			printf("\nPlayer Board:\n");
			printMap(diff, buff, map);

			playerTurn(buff, mapP, grid, &pcShips, &player);
			printf("\nPC Board:\n");
			printPC(diff, buff, mapP);

			printf("\nYour Ships: %d | PC Ships: %d\n", playerShips, pcShips);
			printf("-------------------------------\n");
		} while(playerShips != 0 && pcShips != 0);
	}
	gameOver(&player, playerShips, pcShips);
	saveProfile(profile, &player);

	return 0;
}
