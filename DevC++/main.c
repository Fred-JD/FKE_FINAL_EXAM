#include <stdio.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <conio.h>  

#define user_file "user_data.dat"
#define data_file "Final_Exam_Time.txt"
#define printCourse_global printCourse2(global_user.u_corse, global_user.num_course)

#define len_course_code 9
#define max_char 64
#define max_matrix 10
#define len_date 11
#define len_time 7
#define max_course 10
#define line_data 273

struct Course {
	char code[len_course_code];
	char name[max_char];
	int section;
	char lec_name[max_char];
	char date[len_date];
	char time[len_time];
};

struct User{
	char name[max_char];
	char matrix[max_matrix];
	int num_course;
	struct Course u_corse[max_course];
};

struct datetime {
	int year;
	int month;
	int day;
	int hour;
};

struct Course global_course[line_data];
struct User global_user;

enum bool {false = 0, true};

void readData();
void createAccount();
void readUser();
void readAllUser();
void startPage();
void deleteCourse();
void addCourse();
void listMyCourse();
void listCourse();
void deleteUser();
void deleteUser2();
void terminate();
void main_menu();
void printUser();
void printUser2();
void printCourse(const struct Course*, const int);
void printCourse2(const struct Course *course, const int num_course);

enum bool checkCourse(const char*);
enum bool sameCourse(const char*);

int main() {
	readData();
	startPage();
}

void readData() 
{
	char buffer[130]; //store line info
	int i;
	
	// File Open and Error handle
	FILE* data;
	data = fopen(data_file, "r");
	if(data == NULL){ //Error handle for file
			fprintf(stderr,"Unable to open '%s'\n",data_file);
	}
	fgets(buffer, 50, data);  //Remove first line
	
	char other_data[120];
	
	for(i = 0; i < line_data; i++) {
		int it = 0; //iterator
		char *cptr = '\0'; // Initialize a empty pointer
		char *cptr1 = '\0';
		
		fscanf(data, "%s ", global_course[i].code);
		fgets(buffer, 130, data);
		while(buffer[it] != '\n'){
			int c = buffer[it];
			strcpy(global_course[i].date, "");
			if(isdigit(c)){
				global_course[i].name[it-1] = '\0'; 	//Make the last char empty set
				
				cptr = (buffer + it); 		// Assign the pointer of the place we left
				strcpy(other_data, cptr); 	// then assign it to a new string
				
				int ignored;
				strcpy(global_course[i].lec_name, "");

				sscanf(other_data, "%d %d %s %s",
					&ignored,
					&global_course[i].section,
					global_course[i].date,
					global_course[i].time);
				
				int blen = 0;
				if(ignored > 9 && global_course[i].section > 9){
					blen = 6;
				}
				else if((ignored > 9 && global_course[i].section < 10) || (ignored < 10 && global_course[i].section > 9)){
					blen = 5;
				}
				else {
					blen = 4;
				}

				blen += strlen(global_course[i].date) + 8;
				cptr1 = (buffer + it + blen);
				strcpy(global_course[i].lec_name, cptr1);
				int ch_len = strlen(global_course[i].lec_name);
				global_course[i].lec_name[ch_len - 1] = '\0'; //remove the '\n' from the last char
				
				if(strcmp(global_course[i].lec_name, "") ==0 ) {
					// Handle Error for no lecture name
					strcpy(global_course[i].lec_name, "NO_LECTURE_NAME");
				}
				
				break;
			}
			global_course[i].name[it] = c;
			it++;
		}
	}
	
	fclose(data);
}

void createAccount() {
//	struct Course *course;
	char name_input[max_char];
	printf("Enter your name (Max 64 char) : ");
	fflush(stdin);
	fgets(name_input, max_char, stdin);
	name_input[strlen(name_input) - 1] = '\0';
	//name_input[strlen(name_input)] = NULL;
	strcpy(global_user.name, name_input);
	printf("Enter your matrix (AxxEExxxx) : ");
	scanf("%9s", global_user.matrix);
	global_user.num_course = 0;
	
	printf("\n\nAccount has created\n");
	printf("Name : %s  Matrix : %s\n\n\n", global_user.name, global_user.matrix);
	
	printf("\tPress any key to continue...........>\n\n\n");
		if(getch())
			addCourse();
			
	// Save the user data
	FILE *write;
	write = fopen(user_file, "ab");
	
	fseek(write, 0, SEEK_END); // navigate to end of file
	fwrite(&global_user, sizeof(global_user), 1, write);
	
	fclose(write);
	
	main_menu();
}

void readAllUser() {
	FILE *read;
	read = fopen(user_file, "rb");
	if(read == NULL){
		printf("Do not have any account.\n");
		printf("Please crate one.\n");
	}
	if(getc(read) == EOF) {
		printf("Do not have any account.\n");
		printf("Please crate one.\n");
	}
	rewind(read);
	int i = 1;
	printf("   %-64s %s", "Name", "Matrix");
	printf("\n==================================================================================================================\n");
	while(fread(&global_user, sizeof(global_user), 1, read) == 1){
		printf("%2d %-64s %s\n", i, global_user.name, global_user.matrix);
		i++;
	}
	printf("\n\n\nTotal user : %d", --i);
	printf("\n==================================================================================================================\n");
	fclose(read);
}

void printUser(){
	printf("\n");
	printf("Name : %s\n", global_user.name);
	printf("Matrix : %s\n", global_user.matrix);
	printCourse(global_user.u_corse, global_user.num_course);
}

void printUser2() {
	printf("\n==================================================================================================================\n");
	printf("Name : %s\t\t", global_user.name);
	printf("Matrix : %s\n\n", global_user.matrix);
	printCourse2(global_user.u_corse, global_user.num_course);
	printf("\n==================================================================================================================\n");
}

void printCourse(const struct Course *course, const int num_course){
	int i = 0;
	if(num_course == 0){
		printf("No Course Register\n");
		return;
	}
	for(i = 0; i < num_course; i++){
		printf("\n============================================\n");
		printf("Course Code : %s\n", course[i].code);
		printf("Course Title : %s\n", course[i].name);
		printf("Course Section : %d\n", course[i].section);
		printf("Course Lecture : %s\n", course[i].lec_name);
		printf("============================================\n");
	}
}

void printCourse2(const struct Course *course, const int num_course){
	int i = 0;
	if(num_course == 0){
		printf("No Course Register\n");
		return;
	}
	
	char title[] = "Title", section[] = "Section", lecturer[] = "Lecturer", time_exam[] = "Exam Time";
	printf("      Code       %-10s %38s   %-50s %s\n", 
			title, section, lecturer, time_exam);
	for(i = 0; i < num_course; i++){
		printf("%3d %10s   %-42s %2d       %-50s %-10s %s\n", 
				i+1, course[i].code, course[i].name, course[i].section, course[i].lec_name, course[i].date, course[i].time);
	}
	printf("\nTotal number of section avaliable : %d\n", i);
}


enum bool checkCourse(const char *code) {
	int i;
	for(i = 0; i < line_data; i++){
		if(strcmp(global_course[i].code, code) == 0){
			return true;
		}
	}
	return false;
}

enum bool sameCourse(const char *code) {
	int i;
	for(i = 0; i < global_user.num_course; i++){
		if(strcmp(global_user.u_corse[i].code, code) == 0){
			return true;
		}
	}
	return false;
}

void addCourse() {
	int i, kf, kl, sec; //k for the first course found j is last course
	char code[len_course_code];
	
start_add_course:
	system("cls");
	printf("Do you want to register new course? (y/n) : ");
	switch(getch()) {
		case 'y':
			printf("\n");
			break;
		case 'n':
			
			printf("\nExit add course\n");
			printf("\tPress any key to continue...........>\n\n\n");
			if(getch())
				return;
		default : 
			printf("\nInvalid only(y/n)\n");
			goto start_add_course;
	}
	
	if(global_user.num_course >= max_course) {
		printf("Each person can maximum register 10 course\n");
		printf("\tPress any key to continue...........>\n\n\n");
		if(getch())
			main_menu();
	}
	
	printf("Enter your course code (SSCE1192) : ");
	fflush(stdin);
	scanf("%s", code);
	
	if(checkCourse(code) == false){
		printf("Course enter is invalid!!\n");
		printf("\tPress any key to continue...........>\n\n\n");
		if(getch())
			goto start_add_course;;
	}
	
	if(sameCourse(code) == true){
		printf("You had registered that course\n");
		printf("\tPress any key to continue...........>\n\n\n");
		if(getch())
			goto start_add_course;;
	}
	
	printf("\n\nHere is the list of the section avaliable");
	printf("\n============================================\n");
	char title[] = "Title", section[] = "Section", lecturer[] = "Lecturer";
	printf("     Code       %-10s %26s   %s\n", title, section, lecturer);
	int count = 1;
	for(i = 0; i < line_data; i++) {
		if(strcmp(global_course[i].code, code) == 0){
			kl = i;
			printf("%2d %10s   %-30s %2d       %s\n", count, global_course[i].code, global_course[i].name, global_course[i].section, global_course[i].lec_name);
			count++;
		}
	}
	kf = kl - count + 2;
	printf("\n\nTotal number of section avaliable : %d", count - 1);
	printf("\n============================================\n");
	
	printf("Enter your section : ");
	fflush(stdin);
	scanf("%d", &sec);
	enum bool sec_exit = false;
	
	while(sec_exit == false) {
		for(i = kf; i < kl + 1; i++){
			if(global_course[i].section == sec){
				printf("Course registered\n");
				global_user.u_corse[global_user.num_course] = global_course[i];
				global_user.num_course++;
				
				
				sec_exit = true;
				break;
			}
		}
		if(sec_exit == false) {
			printf("Section enter is invalid!\n");
			scanf("%d", &sec);
		}
	}
	
	printf("\tPress any key to continue...........>\n\n\n");
		if(getch())
			goto start_add_course;;
}

void addCourse2() {
	int i, kf, kl, sec; //k for the first course found j is last course
	char code[len_course_code];
	FILE *fp2;
	
start_add_course:
	system("cls");
	printf("Do you want to register new course? (y/n) : ");
	switch(getch()) {
		case 'y':
			printf("\n");
			break;
		case 'n':
			printf("\nExit add course\n");
			printf("\tPress any key to continue...........>\n\n\n");
			deleteUser2();
			fp2 = fopen(user_file, "ab+");
			fseek(fp2, ftell(fp2) - sizeof(global_user), 0);
			fwrite(&global_user, sizeof(global_user), 1, fp2);
			fclose(fp2);
			if(getch())
				main_menu();
		default : 
			printf("\nInvalid only(y/n)\n");
			goto start_add_course;
	}
	
	if(global_user.num_course >= max_course) {
		printf("Each person can maximum register 10 course\n");
		printf("\tPress any key to continue...........>\n\n\n");
		if(getch())
			main_menu();
	}
	
	printf("Enter your course code (SSCE1192) : ");
	fflush(stdin);
	scanf("%s", code);
	
	if(checkCourse(code) == false){
		printf("Course enter is invalid!!\n");
		printf("\tPress any key to continue...........>\n\n\n");
		if(getch())
			goto start_add_course;;
	}
	
	if(sameCourse(code) == true){
		printf("You had registered that course\n");
		printf("\tPress any key to continue...........>\n\n\n");
		if(getch())
			goto start_add_course;;
	}
	
	printf("\n\nHere is the list of the section avaliable");
	printf("\n============================================\n");
	char title[] = "Title", section[] = "Section", lecturer[] = "Lecturer";
	printf("     Code       %-10s %26s   %s\n", title, section, lecturer);
	int count = 1;
	for(i = 0; i < line_data; i++) {
		if(strcmp(global_course[i].code, code) == 0){
			kl = i;
			printf("%2d %10s   %-30s %2d       %s\n", count, global_course[i].code, global_course[i].name, global_course[i].section, global_course[i].lec_name);
			count++;
		}
	}
	kf = kl - count + 2;
	printf("\n\nTotal number of section avaliable : %d", count - 1);
	printf("\n============================================\n");
	
	printf("Enter your section : ");
	fflush(stdin);
	scanf("%d", &sec);
	enum bool sec_exit = false;
	
	while(sec_exit == false) {
		for(i = kf; i < kl + 1; i++){
			if(global_course[i].section == sec){
				printf("Course registered\n");
				global_user.u_corse[global_user.num_course] = global_course[i];
				global_user.num_course++;
				
				
				sec_exit = true;
				break;
			}
		}
		if(sec_exit == false) {
			printf("Section enter is invalid!\n");
			scanf("%d", &sec);
		}
	}
	
	printf("\tPress any key to continue...........>\n\n\n");
		if(getch())
			goto start_add_course;;
}

void startPage() {
	system("cls");
	printf("\n\n\xB2\xB2\xB2\xB2\xB2 Welcome\n\n");
	printf("\xB2\xB2\xB2\xB2\xB2 Start Page\n\n");
	printf("\xDB\xDB\xDB\xDB\xB2 1. NEW USER\n\n");
	printf("\xDB\xDB\xDB\xDB\xB2 2. OLD USER\n\n");
	
	printf("Enter your choice:");
	switch(getch()) {
		case '1':
			system("cls");
			createAccount();
			break;
		case '2':
			system("cls");
			readUser();
			break;
		default:
			startPage();
	}
}

void main_menu(){
	system("cls");
	
	int i, menu_len = 7, choice;
	char *menu[] = { "Add New Course", "Delete Course", "List My Course", "List All the Course", "Delete Account", "Start Page","Exit"};
	void (*menu_func[7])(void) = { addCourse2, deleteCourse, listMyCourse, listCourse, deleteUser, startPage, terminate };
	printf("\n\n\xB2\xB2\xB2\xB2\xB2 Welcome\n\n");
	printf("\n\n\xB2\xB2\xB2\xB2\xB2 Welcome\n\n");
	printf("\xB2\xB2\xB2\xB2\xB2 Start Page\n\n");
	
	for(i = 0; i < menu_len; i++){
		printf("\xDB\xDB\xDB\xDB\xB2 %2d. %s\n\n", i+1, menu[i]);
	}
	
	printf("Enter your choice:");
	fflush(stdin);
	scanf("%d", &choice);
//	choice = getch();
	//(*menu_func[choice])();
    if(choice > 0 && choice <= menu_len){
		choice--;
		(*menu_func[choice])();
	}
	else {
		printf("Choice Invalid! Please retry.\n");
		printf("\tPress any key to continue...........>\n\n\n");
		if(getch())
			main_menu();
	}
}

void deleteCourse(){
	system("cls");
	char code[len_course_code];
	int i, id_course;
	char choice;
	enum bool course_exit = false;
	
	printUser2();
	printf("\n\n");
	
	printf("Do you want to delete course? (y/n) : ");
	switch(getch()) {
		case 'y':
			printf("\n");
			break;
		case 'n':
			printf("\tPress any key to continue...........>\n\n\n");
			if(getch())
				main_menu();
		default : 
			printf("\nInvalid only(y/n) \n");
			printf("\tPress any key to continue...........>\n\n\n");
			if(getch())
				main_menu();
	}
	
	while(course_exit == false){
		printf("Enter the course code : ");
		fflush(stdin);
		scanf("%8s", code);
		for(i = 0; i < global_user.num_course; i++){
			if(strcmp(global_user.u_corse[i].code, code) == 0){
				course_exit = true;
				id_course = i;
				break;
			}
		}
		if(course_exit == false)
			printf("The code enter is invalid!\n");
	}
	
	system("cls");
	printf("\n\n==================================================================================================================\n");
	printCourse2(&global_user.u_corse[id_course], 1);
	printf("Are you sure you want to remove that course? (y/n)");
	
	while(choice != 'y' || choice != 'n'){
		fflush(stdin);
		scanf("%c", &choice);
		if(choice == 'n'){
			deleteCourse();
		}
		else if(choice == 'y'){
			break;
		}
	}
	
	for(i = id_course; i < global_user.num_course;  i++){
		global_user.u_corse[i] = global_user.u_corse[i+1];
	}
	global_user.num_course--;
	
	FILE *fp2;
	deleteUser2();
	fp2 = fopen(user_file, "ab+");
	fseek(fp2, ftell(fp2) - sizeof(global_user), 0);
	fwrite(&global_user, sizeof(global_user), 1, fp2);
	fclose(fp2);
	
	printf("Course has been deleted\n");
	printf("\tPress any key to continue...........>\n\n\n");
	if(getch())
		main_menu();
}

enum bool chechMatrix(const char* matrix) {
	return strcmp(global_user.matrix, matrix) ==  0 ? true : false;
}

void readUser() {
	char matrix[max_matrix];
	enum bool user_exit;
	system("cls");
	
	FILE *read;
	read = fopen(user_file, "rb");
	if(read == NULL){
		printf("Do not have any account.\n");
		printf("Please crate one.\n");
		printf("\tPress any key to continue...........>\n\n\n");
		if(getch())
			startPage();
	}
	if(getc(read) == EOF) {
		printf("Do not have any account.\n");
		printf("Please crate one.\n");
		printf("\tPress any key to continue...........>\n\n\n");
		if(getch())
			startPage();
	}
	fclose(read);
	
	readAllUser();
	
	read = fopen(user_file, "rb");
	printf("Enter your matrix (AxxEExxxx)\n");
	fflush(stdin);
	scanf("%9s", matrix);
	
	rewind(read);
	while(fread(&global_user, sizeof(global_user), 1, read) == 1){
		if(chechMatrix(matrix) == true){
			user_exit = true;
			break;
		}
		else{
			user_exit = false;
		}
	}
	
	if(user_exit == false){
		printf("Matrix enter is invalid\n"); 
		readUser();
	}
		
	printf("\n");
	fclose(read);
	
	main_menu();
}

void listMyCourse(){
	system("cls");
	printf("\n\n==================================================================================================================\n");
	printCourse_global;
	printf("\tPress any key to continue...........>\n\n\n");
	if(getch())
		main_menu();
}

void listCourse(){
	system("cls");
	printCourse2(global_course, line_data);
	printf("\tPress any key to continue...........>\n\n\n");
	if(getch())
		main_menu();
}

void deleteUser() {
	system("cls");
	FILE *ft, *fp;
	fp = fopen(user_file, "rb+");
	struct User deleteUser;
	printf("Do you want to delete it? (y/n):");
	if (getch() == 'y')
	{
		ft = fopen("test.dat", "wb+");  //temporary file for delete
		rewind(fp);
		while (fread(&deleteUser, sizeof(deleteUser), 1, fp) == 1)
		{
			if (strcmp(global_user.matrix, deleteUser.matrix) != 0)
			{
				fseek(ft, 0, SEEK_CUR);
				fwrite(&deleteUser, sizeof(deleteUser), 1, ft); //write all in tempory file except that
			}                              //we want to delete
		}
		fclose(ft);
		fclose(fp);
		
		remove(user_file);
		rename("test.dat", user_file); //copy all item from temporary file to fp except that
		
		printf("\nAccount has been remove.\n");
		printf("\nDo you want continue? (y/n) ");
		fflush(stdin);
		switch(getch()){
			case 'y':
				startPage();
			case 'n':
				terminate();
			default:
				terminate();
		}
	}
}

void deleteUser2() {
	struct User deleteUser;
	FILE *ft, *fp;
	fp = fopen(user_file, "rb+");
	ft = fopen("test.dat", "wb+");  //temporary file for delete
	rewind(fp);
	while (fread(&deleteUser, sizeof(deleteUser), 1, fp) == 1)
	{
		if (strcmp(global_user.matrix, deleteUser.matrix) != 0)
		{
			fseek(ft, 0, SEEK_CUR);
			fwrite(&deleteUser, sizeof(deleteUser), 1, ft); //write all in tempory file except that
		}                              //we want to delete
	}
	fclose(ft);
	fclose(fp);
	
	remove(user_file);
	rename("test.dat", user_file); //copy all item from temporary file to fp except that
}

void terminate() {
	system("cls");
	printf("\tFKE Final Exam Managemnt System\n\n");
	printf("\tMini Project in C\n\n");
	printf("\tbrought to you by Jiande\n\n");
	printf("\tPress any key to close...........>\n\n\n");
	if(getch())
		exit(0);
}