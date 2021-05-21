#include "TM4.h"


#include <fstream>
//#include <stdio.h>
//#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <iostream>

#define Size_Training 8 
#define Size_Test 8 
#define Size_Image 3

int Data_Training[Size_Training][Size_Image] ;
int Data_Training_Answers[Size_Training] ;
int Data_Test[Size_Test][Size_Image] ;
int Data_Test_Answers[Size_Test] ;

using namespace std ; 

void read_Data(){
	 
	FILE * fp;
	char * line = NULL;
	size_t len = 0;

	const char *s = ",";
	char *token = NULL;

	fp = fopen("Extracted_Train.csv", "r");
	if (fp == NULL) {
		printf("Error opening Train\n");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < Size_Training; i++) {
		getline(&line, &len, fp);

		token = strtok(line, s);
		for (int j = 0; j < Size_Image; j++) {
			Data_Training[i][j] = atoi(token);
			token=strtok(NULL,s);
		}
		Data_Training_Answers[i] = atoi(token);
	}

	fp = fopen("Extracted_Test.csv", "r");
	if (fp == NULL) {
		printf("Error opening test \n");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < Size_Test; i++) {
		getline(&line, &len, fp);

		token = strtok(line, s);
		for (int j = 0; j < Size_Image; j++) {
			Data_Test[i][j] = atoi(token);
			token=strtok(NULL,s);
		}
		Data_Test_Answers[i] = atoi(token);
	}
	
	/*print example of digit.
	srand(time(NULL)*clock());	
	int random = rand()%Size_Training ;
	//cout << "random = " << random << endl;
	cout << "Example of digit " << Data_Training[random][Size_Image+1] << "." << endl; 
	for (int y = 0; y < 7; y++) {
		for (int x = 0; x < 7; x++) {
			if(Data_Training[random][x+7*y] == 1) {
				printf("@");
			} else {
				printf(".");
			}
		}
		printf("\n");
	}
	*/
cout << "Complete" << endl; 	
}

int main(){
	
fstream DEBUG;
remove("DEBUG.csv");
DEBUG.open("DEBUG.csv" , ios::app);
if(!DEBUG.good()){
	cerr << "Error opening DEBUG.csv" << endl;
		exit(EXIT_FAILURE);
	}
DEBUG.close();

fstream Results;
remove("results.csv") ; 
Results.open("results.csv" , ios::app);
	if(!Results.good()){
	cerr << "Error opening Results.csv" << endl;
		exit(EXIT_FAILURE);
	}
	
fstream Switching_Results;
remove("Switching_Results.csv");
	Switching_Results.open("Switching_Results.csv" , ios::app);
	if(!Switching_Results.good()){			
		cerr << "Error opening DEBUG.csv" << endl;
		exit(EXIT_FAILURE);
	}
Switching_Results << "Exclude to Include,Include to Exclude, Num type 1 rewards, Num type 1 penalty , num type 2" << endl;
Switching_Results.close();

Results << " Iterations , Clauses , States , Threshold , S , Leniancy , BOOST_POSITIVE" << endl ; 
Results << Number_Iterations << "," << Number_Clauses << "," << Number_States << "," << THRESHOLD << "," << S << "," << Leniancy << "," << BOOST_POSITIVE << endl ; 


	clock_t start_total = clock();
	
	read_Data();
	
	clock_t end_total = clock();
	double time_used = ((double) (end_total - start_total)) / CLOCKS_PER_SEC;
	printf("Reading Data: %.1f s\n", time_used);
	
	start_total = clock();
	cout << endl;
	
	initialise();
	
	end_total = clock();
	time_used = ((double) (end_total - start_total)) / CLOCKS_PER_SEC;
	printf("Initialisation time: %.1f s\n\n", time_used);
/*	//start of brute force search
	for(Mid_State = 10 ; Mid_State <70 ; Mid_State+=10){
			Number_States = Mid_State*2 ;
		for(S = 1.5 ; S < 5.0 ; S+=0.5){
			for(THRESHOLD = 1 ; THRESHOLD < 10 ; THRESHOLD++){
				for(BOOST_POSITIVE = 0 ; BOOST_POSITIVE <= 1 ; BOOST_POSITIVE++){

	initialise();
	
	Results << " Iterations , Clauses , States , Threshold , S , Leniancy , BOOST_POSITIVE" << endl ; 
	Results << Number_Iterations << "," << Number_Clauses << "," << Number_States << "," << THRESHOLD << "," << S << "," << Leniancy << "," << BOOST_POSITIVE << endl ; 
*/	//end of brute for search
	
	Results << "Epoch , Testing time, Testing Accuracy , Training Time , Training Accuracy" << endl ; 	
	
	for(int Iteration = 1 ; Iteration <= Number_Iterations ; ++Iteration ){
	int Correct = 0 ; 
	start_total = clock();
	cout << "EPOCH #" << Iteration << endl; 
	Correct = TM_Image(Size_Training, Data_Training , Data_Training_Answers , 0);
	cout << Correct << " correct." ;
	end_total = clock();
	time_used = ((double) (end_total - start_total)) / CLOCKS_PER_SEC;
	printf("Training time: %.1f s\n", time_used);
	
	Results << Iteration << "," <<  time_used << "," << (float)Correct/Size_Training << "," ;
	
	start_total = clock();
	Correct = TM_Image(Size_Test, Data_Test , Data_Test_Answers , 1);
	cout << Correct << " correct." ;
	if(Correct == Size_Test) cout << "yayyyyyyyyyyyyy" << endl ;
	end_total = clock();
	time_used = ((double) (end_total - start_total)) / CLOCKS_PER_SEC;
	printf("Testing time: %.1f s\n", time_used);
	
	Results << time_used << "," << (float)Correct/Size_Test << endl ;
	
	}
	
//	}}}} //brute force search clsoing brackets.
Results.close();
return 0; 
}





