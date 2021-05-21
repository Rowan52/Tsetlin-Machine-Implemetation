#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>

#include "TM4.h"

int TM[Number_Possible_values][Number_Clauses][Number_Automata]; 

//output data
int Exclude_To_Include = 0 ;
int Include_To_Exclude = 0 ;
int Num_Type_2 = 0 ;
int Num_Type_1_Rewards = 0 ; 
int Num_Type_1_Penalty = 0 ;

using namespace std ; 

void Debug(int Option){
fstream DEBUG ;
DEBUG.open("DEBUG.csv" , ios::app);
if(!DEBUG.good()){
	cerr << "Error opening DEBUG.csv" << endl;
		exit(EXIT_FAILURE);
	}
	
	if(Option == 1){
		DEBUG << endl ; 
		for(int Pos_Value = 0 ; Pos_Value < Number_Possible_values ; ++Pos_Value){
			DEBUG << Pos_Value << ",," ;
			for( int Clause = 0 ; Clause < Number_Clauses ; ++Clause){
				for(int Automata = 0 ; Automata < Number_Automata ; ++ Automata){
					DEBUG << TM[Pos_Value][Clause][Automata] << ","  ;
				}
				DEBUG << ",";
			}	
			DEBUG << endl;
		}
	}else if(Option == 2){
		
	} else {
		cout << "Debug Option out of range " << endl ; 
	}
DEBUG.close();
}

void initialise(){
cout << "starting Initialisation" << endl ;

//randomly puts each automata either side of the reward punish switching line

	srand(time(0));	
	for(int Pos_Value = 0 ; Pos_Value < Number_Possible_values ; ++Pos_Value){
		for( int Clause = 0 ; Clause < Number_Clauses ; ++Clause){
			for(int Automata = 0 ; Automata < Number_Features ; ++ Automata){
				if((1.0*((float)rand())/RAND_MAX) <= 0.5){
					TM[Pos_Value][Clause][Automata] = Mid_State + 1 ; //include
					TM[Pos_Value][Clause][Automata+Number_Features] = Mid_State;  //exclude 
				} else {
					TM[Pos_Value][Clause][Automata] = Mid_State ;
					TM[Pos_Value][Clause][Automata+Number_Features] = Mid_State + 1 ;
				}
			}
		}	
		//cout << "number : " << Pos_Value <<  endl;
		//cout << TM[Pos_Value][rand()%51][rand()%51];
	}		
	Debug(1); 
cout << "initialisation complete" << endl;	
}

void Change_States(int New_Mid_State){
	int Old_Mid_State = Mid_State ;
	
	float Scale_Factor = New_Mid_State/Old_Mid_State;
	
	for(int Pos_Value = 0 ; Pos_Value < Number_Possible_values ; ++Pos_Value){
		for( int Clause = 0 ; Clause < Number_Clauses ; ++Clause){
			for(int Automata = 0 ; Automata < Number_Features ; ++ Automata){
				
				TM[Pos_Value][Clause][Automata] = (int)( TM[Pos_Value][Clause][Automata] * Scale_Factor);
				TM[Pos_Value][Clause][Automata+Number_Features] = (int)(TM[Pos_Value][Clause][Automata+Number_Features] * Scale_Factor);
			}
		}
	}
		
	Mid_State = New_Mid_State ; 
	Number_States = Mid_State*2 ;
	
}

int Action(int state){
	return state > Mid_State ; 
	//return 1 if include, 0 if exclude 
}	

void Feedback1(int Pos_Value, int Clause , int Image[Number_Features] , int Clause_Vote){
	//rewards correct automata when the overall clause was incorrect 
	//penalty for incorrect automata when overall clause was correct
	
	//cout << "type 1 " << endl ; 
	if(Clause_Vote == 0){
		//cout << "Clause vote = 0 " << endl ; 
		for(int k = 0 ; k < Number_Features ; k ++){
			
			int Flag1 = 0 , Flag2 = 0 , Prev_State_1 , Prev_State_2 ; 
			TM[Pos_Value][Clause][k] == (Mid_State +1) ? Flag1 = 1 : Flag1 = 0;
			TM[Pos_Value][Clause][k+Number_Features] == (Mid_State + 1) ? Flag2 = 1 : Flag2 = 0;
			Prev_State_1 = TM[Pos_Value][Clause][k];
			Prev_State_2 = TM[Pos_Value][Clause][k + Number_Features];
			
			TM[Pos_Value][Clause][k] -= (TM[Pos_Value][Clause][k] > 1 ) && (1.0*rand()/RAND_MAX <= 1.0/S);
			TM[Pos_Value][Clause][k+Number_Features] -= (TM[Pos_Value][Clause][k] > 1 ) && (1.0*rand()/RAND_MAX <= 1.0/S);
			
			if(Flag1 == 1 && TM[Pos_Value][Clause][k] == (Mid_State)) Include_To_Exclude++ ; 
			if(Flag2 == 1 && TM[Pos_Value][Clause][k+Number_Features] == (Mid_State)) Include_To_Exclude++ ;
			if(Prev_State_1 == TM[Pos_Value][Clause][k] && TM[Pos_Value][Clause][k] > Mid_State ) Num_Type_1_Penalty++ ;
			if(Prev_State_1 == TM[Pos_Value][Clause][k] && TM[Pos_Value][Clause][k] <= Mid_State ) Num_Type_1_Rewards++ ;
			if(Prev_State_2 == TM[Pos_Value][Clause][k + Number_Features] && TM[Pos_Value][Clause][k + Number_Features] > Mid_State ) Num_Type_1_Penalty++ ;
			if(Prev_State_2 == TM[Pos_Value][Clause][k + Number_Features] && TM[Pos_Value][Clause][k + Number_Features] <= Mid_State ) Num_Type_1_Rewards++ ;
			
		}
	}else if(Clause_Vote == 1){
		//cout << "Clause Vote = 1 " << endl ;
		for(int k = 0 ; k < Number_Features ; k ++){
			if(Image[k] == 1){
				
				int Flag1 = 0 , Flag2 = 0 , Prev_State_1 , Prev_State_2  ; 
				TM[Pos_Value][Clause][k] == (Mid_State) ? Flag1 = 1 : Flag1 = 0;
				TM[Pos_Value][Clause][k+Number_Features] == (Mid_State + 1) ? Flag2 = 1 : Flag2 = 0;
				Prev_State_1 = TM[Pos_Value][Clause][k];
				Prev_State_2 = TM[Pos_Value][Clause][k + Number_Features];
				
				TM[Pos_Value][Clause][k] += (TM[Pos_Value][Clause][k] < Number_States ) && (BOOST_POSITIVE == 1 || 1.0*rand()/RAND_MAX <= (S-1)/S);
				TM[Pos_Value][Clause][k+Number_Features] -= (TM[Pos_Value][Clause][k+Number_Features] > 1 ) && (1.0*rand()/RAND_MAX <= 1.0/S);
				
				if(Flag1 == 1 && TM[Pos_Value][Clause][k] == (Mid_State + 1)) Exclude_To_Include++ ; 
				if(Flag2 == 1 && TM[Pos_Value][Clause][k+Number_Features] == (Mid_State)) Include_To_Exclude++ ;
				if(Prev_State_1 != TM[Pos_Value][Clause][k] && TM[Pos_Value][Clause][k] > Mid_State ) Num_Type_1_Rewards++ ;
				if(Prev_State_1 != TM[Pos_Value][Clause][k] && TM[Pos_Value][Clause][k] <= Mid_State ) Num_Type_1_Penalty++ ;
				if(Prev_State_2 != TM[Pos_Value][Clause][k + Number_Features] && TM[Pos_Value][Clause][k + Number_Features] > Mid_State ) Num_Type_1_Penalty++ ;
				if(Prev_State_2 != TM[Pos_Value][Clause][k + Number_Features] && TM[Pos_Value][Clause][k + Number_Features] <= Mid_State ) Num_Type_1_Rewards++ ;
				
			}else if(Image[k] == 0){
				
				int Flag1 = 0 , Flag2 = 0  , Prev_State_1 , Prev_State_2 ; 
				TM[Pos_Value][Clause][k] == (Mid_State + 1) ? Flag1 = 1 : Flag1 = 0;
				TM[Pos_Value][Clause][k+Number_Features] == (Mid_State) ? Flag2 = 1 : Flag2 = 0;
				Prev_State_1 = TM[Pos_Value][Clause][k];
				Prev_State_2 = TM[Pos_Value][Clause][k + Number_Features];
				
				TM[Pos_Value][Clause][k+Number_Features] += (TM[Pos_Value][Clause][k+Number_Features]  < Number_States ) && (BOOST_POSITIVE == 1 || 1.0*rand()/RAND_MAX <= (S-1)/S);
				TM[Pos_Value][Clause][k] -= (TM[Pos_Value][Clause][k] > 1 ) && (1.0*rand()/RAND_MAX <= 1.0/S);
		
				if(Flag1 == 1 && TM[Pos_Value][Clause][k] == (Mid_State)) Include_To_Exclude++ ; 
				if(Flag2 == 1 && TM[Pos_Value][Clause][k+Number_Features] == (Mid_State + 1)) Exclude_To_Include++ ;
				if(Prev_State_1 != TM[Pos_Value][Clause][k] && TM[Pos_Value][Clause][k] > Mid_State ) Num_Type_1_Penalty++ ;
				if(Prev_State_1 != TM[Pos_Value][Clause][k] && TM[Pos_Value][Clause][k] <= Mid_State ) Num_Type_1_Rewards++ ;
				if(Prev_State_2 != TM[Pos_Value][Clause][k + Number_Features] && TM[Pos_Value][Clause][k + Number_Features] > Mid_State ) Num_Type_1_Rewards++ ;
				if(Prev_State_2 != TM[Pos_Value][Clause][k + Number_Features] && TM[Pos_Value][Clause][k + Number_Features] <= Mid_State ) Num_Type_1_Penalty++ ;
			
			}
		}
	} 
	//keep in range 
	for(int k = 0 ; k < Number_Automata ; k++){
		if(TM[Pos_Value][Clause][k] < 0) TM[Pos_Value][Clause][k] = 0 ;
		if(TM[Pos_Value][Clause][k] > Number_States) TM[Pos_Value][Clause][k] = Number_States ;
	}
}

void Feedback2(int Pos_Value, int Clause, int Image[Number_Features] , int Clause_Vote) { 

	int Action_include , Action_include_negated ;
	if(Clause_Vote == 1){
	cout << "type 2 feedback "<< endl; 
		for(int k = 0 ; k < Number_Features ; k ++){
			Action_include = Action(TM[Pos_Value][Clause][k]);
			Action_include_negated = Action(TM[Pos_Value][Clause][k+Number_Features]);
			
			int Flag1 = 0 , Flag2 = 0 , Prev_State_1 , Prev_State_2 ; 
			TM[Pos_Value][Clause][k] == Mid_State ? Flag1 = 1 : Flag1 = 0;
			TM[Pos_Value][Clause][k+Number_Features] == Mid_State ? Flag2 = 1 : Flag2 = 0;
			Prev_State_1 = TM[Pos_Value][Clause][k];
			Prev_State_2 = TM[Pos_Value][Clause][k + Number_Features];
			
			TM[Pos_Value][Clause][k] += (Action_include == 0 && TM[Pos_Value][Clause][k] < Number_States) && (Image[k] == 0 );			
			TM[Pos_Value][Clause][k+Number_Features] += (Action_include_negated == 0 && TM[Pos_Value][Clause][k] < Number_States) && (Image[k] == 1 );
		
			if(Flag1 == 1 && TM[Pos_Value][Clause][k] == (Mid_State + 1)) Exclude_To_Include ++ ; 
			if(Flag2 == 1 && TM[Pos_Value][Clause][k+Number_Features] == (Mid_State + 1)) Exclude_To_Include ++ ;
			if(Prev_State_1 != TM[Pos_Value][Clause][k]) Num_Type_2++ ;
			if(Prev_State_2 != TM[Pos_Value][Clause][k + Number_Features]) Num_Type_2++ ;
		}
		
			//keep in range 
		for(int k = 0 ; k < Number_Automata ; k++){
			if(TM[Pos_Value][Clause][k] < 0) TM[Pos_Value][Clause][k] = 0 ;
			if(TM[Pos_Value][Clause][k] > Number_States) TM[Pos_Value][Clause][k] = Number_States ;
		}
		
	}
	
	
}

void Update( int Pos_Value ,int Answer , int Sum_Pos[Number_Possible_values], int Vote_Clauses[][Number_Clauses] , int Image[Number_Features]){
srand(time(0));

		float T = 0 ; 
		Sum_Pos[Pos_Value] >= THRESHOLD ? T = THRESHOLD : T = Sum_Pos[Pos_Value] ; //clamp t between +- Threshold
		Sum_Pos[Pos_Value] <= -THRESHOLD ? T = -THRESHOLD : T = Sum_Pos[Pos_Value] ; 
		int target = /*(Pos_Value && Answer) || (!Pos_Value && !Answer)*/ (Pos_Value == Answer) ; 
		for(int Clause = 0 ; Clause < Number_Clauses; ++Clause){ 
			float feedback  = (2*target -1)*(1.0*rand()/RAND_MAX <= ((1.0/(THRESHOLD*2))*(THRESHOLD +(1-2*target)*Sum_Pos[Pos_Value])));
			if(feedback > 0 ){ //type 1 feedback 
				Feedback1(Pos_Value , Clause , Image , Vote_Clauses[Pos_Value][Clause]);
			
			}else if(feedback < 0){ //type 2 
				Feedback2(Pos_Value, Clause , Image , Vote_Clauses[Pos_Value][Clause]);
			}else{ //no feedback
			 //cout << "NO FEEDBACK" << endl ; 
			}
		}
}

int Machine(int Image[Number_Features] , int Answer ,  bool Method, int debug){	
	//method = 0 training , method = 1 testing. 
	int Vote_Clauses[Number_Possible_values][Number_Clauses] = {0};
	int Sum_Pos[Number_Possible_values]= {0};
	int Action_include , Action_include_negated;
	int Guess = 0;
	int Ran_Pos = Answer;
	

	while(Ran_Pos == Answer){
		Ran_Pos = rand()%Number_Possible_values ;
		//cout << "Ran_Pos = " << Ran_Pos << endl ;
	}
	for(int Pos_Value = 0 ; Pos_Value < Number_Possible_values ; ++Pos_Value){
		
	
		for(int Clause = 0 ; Clause < Number_Clauses ; ++Clause){
		//cout << "Clause = " << Clause << endl; 
			Vote_Clauses[Pos_Value][Clause] = 1 ; //default vote for 
			int Exclude_all = 1 ;
			
			int Wrong = 0 ; 
			for(int Automata = 0 ; Automata < Number_Features ; Automata++){
			//cout << "Automata = " << Automata << endl ; 
	
				Action_include = Action(TM[Pos_Value][Clause][Automata]); //Action() returns 1 if include
				Action_include_negated = Action(TM[Pos_Value][Clause][Automata+Number_Features]);	
				
				Exclude_all = Exclude_all && !(Action_include == 1 || Action_include_negated == 1); 
				
				if ((Action_include == 1 && Image[Automata] == 0) || (Action_include_negated == 1 && Image[Automata] == 1)) {
					Wrong++ ;
					if(Wrong > Leniancy*Number_Features){
						Vote_Clauses[Pos_Value][Clause] = 0; 
						//cout << "break occuring " << endl; 
						break;
					}
				}
				
				Vote_Clauses[Pos_Value][Clause] = Vote_Clauses[Pos_Value][Clause] && !(Method == true && Exclude_all == 1);
			}//for automata close ;
		}//for clause end
		
		//sum class votes 
		for(int i = 0 ; i < Number_Clauses  ; ++i){ 
			//if(i <= Mid_State && Vote_Clauses[Pos_Value][i] == 1) Sum_Pos[Pos_Value]++ ; //vote for
			if(Vote_Clauses[Pos_Value][i] == 1) Sum_Pos[Pos_Value]++;
			//if(i > Mid_State && Vote_Clauses[Pos_Value][i] == 1) Sum_Pos[Pos_Value]-- ; //vote against
		}//for clause 2 end
		//cout << "Class Sum Complete, Next Pos, Vote = " << Sum_Pos[Pos_Value] << endl ; 
		
		//feedback
		//if(Method == false) cout << "feedback \n " ;
		
		if(Method == false && (Pos_Value == Answer || Pos_Value == Ran_Pos )) Update( Pos_Value , Answer , Sum_Pos , Vote_Clauses , Image) ; 
		//if(Method == false) cout << "end of feedback \n" ; 
	}//for pos end
	
	
	
	//Sum Class votes 
	int Guess_Value = Sum_Pos[0];
	for(int i = 0 ; i < Number_Possible_values ; i++){
	if(Sum_Pos[i] > Guess_Value){
		Guess_Value = Sum_Pos[i];
		Guess = i ; 
		}
	}


	
	if(debug == 1 ){
		
			fstream DEBUG ;
	DEBUG.open("DEBUG.csv" , ios::app);
	if(!DEBUG.good()){
	cerr << "Error opening DEBUG.csv" << endl;
		exit(EXIT_FAILURE);
	}
	
		for(int k = 0 ; k < Number_Possible_values ; ++k){
					DEBUG << k << ", Weighting , " << Sum_Pos[k] <<endl;
					
				}
				DEBUG << "Guess," << Guess << ", Answer , " << Answer << endl ;
				DEBUG << "Vote_Clauses" << endl << endl; 
				for(int l = 0 ; l < Number_Possible_values ; ++l){
					for(int j = 0 ; j < Number_Clauses ; ++j){
						
					DEBUG << Vote_Clauses[l][j]<< ", ";
					}
					DEBUG << endl ; 
				}
			
			DEBUG << endl ; 
			DEBUG.close();
		}			
	//end of debug

if(Guess == Answer){
	cout << "Guess = "<< Guess << " - CORRECT" << endl ; 
	return 1 ; 
}else{
	cout << "Guess = "<< Guess << " - WRONG" << endl ; 
	return 0 ; 
}

}

int TM_Image(int Size , int Data[][Number_Features] , int Answers[] , bool Method){
int Number_Correct = 0 , Answer_Truth , debug;
	
	fstream Switching_Results;
	Switching_Results.open("Switching_Results_" + to_string(Number_States) + "_" + to_string(Number_Clauses) + ".csv" , ios::app);
	if(!Switching_Results.good()){			
		cerr << "Error opening Switching_States.csv" << endl;
		exit(EXIT_FAILURE);
	}		
	Exclude_To_Include = 0 ;
	Include_To_Exclude = 0 ;
	Num_Type_2 = 0;
	Num_Type_1_Rewards = 0 ; 
	Num_Type_1_Penalty = 0 ;

		for(int Image = 0 ; Image < Size; Image ++){
		
		//cout << endl << Data[Image][0] << " " << Data[Image][1] << " " << Answers[Image] << endl ; //XOR DEBUG
		//add shuffling here
		
		Image%10000 == 0 ? debug = 1 : debug = 0 ;
		int X[Number_Features];
		for(int i = 0 ; i < Number_Features ; i++){ //copy 2d array into 1d array. 
			X[i] = Data[Image][i] ;
		}
		int Answer = Answers[Image] ;
			//if(Answer > Number_Possible_values -1) cout << "Image number" << Image << " Answer out of range" << endl ;
		Answer_Truth = Machine(X , Answer , Method, debug); //Method 0 = train, 1 = test ;
			if(Answer_Truth == 1){
				Number_Correct++ ;
				Answer_Truth = 0 ; 
			}

		
		//print debug TM Dump
			if(Image%10000 == 0 && Method == false){
				Debug(1);
				//cout << "Image Number " << Image << endl ;
			}
			
			
		
		}
		
	if(Method == 0){
		Switching_Results << Exclude_To_Include << "," << Include_To_Exclude << "," ;
		Switching_Results << Num_Type_1_Rewards << "," <<Num_Type_1_Penalty << "," << Num_Type_2 << endl; 
	}
	Switching_Results.close();
	
	
	
	return Number_Correct ; 
}


