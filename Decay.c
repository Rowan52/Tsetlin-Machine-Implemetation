#include <stdio.h>
#include <math.h>

int main(){
	int number_of_examples = 60000 ;
	int epochs = 100 ;
	
	
	float decay_rate = 30 ;
	float training_minimum = 5000;
	int number_training = number_of_examples ;
	//int number_training = 0 ;
	for (float epoch = 0; epoch < epochs; epoch++) {
	
		//number_training = exp(-((epoch/decay_rate)-log(number_of_examples-training_minimum)))+training_minimum ;
		
		float temp = epoch/decay_rate ;
		number_training = exp(-((temp)-log(number_of_examples-training_minimum)))+training_minimum ;
		printf("Number_training = %d \n" , number_training);
	}
}