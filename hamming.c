#include<stdio.h>
#include<stdbool.h>

void toHamming(int* data, int len, int* ham){
	//transforming data to humming code, len should be multiples of 4
	for(int i = 0; i < (len + 3) / 4; i++){
		int d = i*4;
		int h = i*7;

		ham[h + 7] = data[d + 0];
		ham[h + 6] = data[d + 1];
		ham[h + 5] = data[d + 2];
		ham[h + 3] = data[d + 3];

		ham[h+1] = ham[h+7] ^ ham[h+5] ^ ham[h+3];
		ham[h+2] = ham[h+7] ^ ham[h+6] ^ ham[h+3];
		ham[h+4] = ham[h+7] ^ ham[h+6] ^ ham[h+5];

	}
}



bool fromHamming(int* ham, int len, int* data){
	//transforming data back to regular from humming code, len should be multiples of 7
	
	int c, c1,c2,c3;

	for(int i = 0; i < (len + 6)/7; i++){
		int d = i*4;
		int h = i*7;
		
		//for(int j =0; j < 7; j++){
		//	printf("%d",ham[h+j]);
		//}
		//printf("\n");

		c1=ham[h+1]^ham[h+7]^ham[h+5]^ham[h+3];
		c2=ham[h+2]^ham[h+7]^ham[h+6]^ham[h+3];
		c3=ham[h+4]^ham[h+7]^ham[h+6]^ham[h+5];
		c = c3*4 + c2*2 + c1;

		if(c!=0){
			printf("error found and corrected at %d \n",c);
			ham[h+c] = !ham[h+c];	
		}
		//for(int j =0; j < 7; j++){
		//	printf("%d",ham[h+j]);
		//}
		printf("\n");
		data[d+0] = ham[h+7];
		data[d+1] = ham[h+6];
		data[d+2] = ham[h+5];
		data[d+3] = ham[h+3];
	}
}


void main(){
	int test[] = {1,1,1,1,0,0,0,0,1,0,1,0};
	int outtest[21] = {0} ;
	int test2[100] = {0};
	int len = 12;
	
	for(int i =0; i < len; i++){
		printf("%d,",test[i]);
	}
	printf("\n");
	
	
	toHamming(test, len, outtest);


	outtest[5] = !outtest[5];

	fromHamming(outtest, (len*7)/4, test2);

	for(int i =0; i < len; i++){
		printf("%d,",test2[i]);
	}
	printf("\n");
	return;
}
