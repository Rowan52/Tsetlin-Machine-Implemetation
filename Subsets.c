#include <stdio.h>

int main() {
int Epochs = 8 , Number_Examples = 20;

int Number_Subsets = 5 ;

int LoopCounter = 0 ;  
for(int Epoch = 1 ; Epoch <= Epochs; Epoch++){
	printf("Epoch # %d \n" , Epoch);
	
	for(int i = (Number_Examples/Number_Subsets)*LoopCounter ; i < (Number_Examples/Number_Subsets)*(LoopCounter+1) ; i++){
		printf("%d \n" , i); // TM Fuction
		if(LoopCounter == Number_Subsets) LoopCounter = 0 ; 
		if(i >= Number_Examples) i = 0 ;
	}
	LoopCounter++ ;
 
}	

return 1 ; 	
}
