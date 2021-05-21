#include <stdio.h>

int main() {
int Epochs = 5 ;	
int loop = 0 , Number_Examples = 20;
for(int Epoch = 1 ; Epoch <= Epochs; Epoch++){
	printf("Epoch # %d \n" , Epoch);
	loop = Epoch ;
	int LoopCounter = 0 ;  
	for(int i = 0 ; i <= Number_Examples ; i = i + Epoch){
		printf("%d \n" , i); //TM Function
		if(i > Number_Examples - Epoch ){
			loop = loop - 1;
			LoopCounter ++ ; 
			i = LoopCounter ; 
		}
		
		if (loop <= 0) i = Number_Examples + 1 ;
	}
 
}	

return 1 ; 	
}
