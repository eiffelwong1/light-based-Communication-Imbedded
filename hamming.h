#include <stdio.h>
// Copied these two functions over because I couldn't get the linking to work.
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



void fromHamming(int* ham, int len, int* data){
	//transforming data back to regular from humming code, len should be multiples of 7
	
	int c, c1,c2,c3;

	for(int i = 0; i < (len + 6)/7; i++){
		int d = i*4;
		int h = i*7;
		
		c1=ham[h+1]^ham[h+7]^ham[h+5]^ham[h+3];
		c2=ham[h+2]^ham[h+7]^ham[h+6]^ham[h+3];
		c3=ham[h+4]^ham[h+7]^ham[h+6]^ham[h+5];
		c = c3*4 + c2*2 + c1;

		if(c!=0){
			printf("error found and corrected at %d \n",c);
			ham[h+c] = !ham[h+c];	
		}
		data[d+0] = ham[h+7];
		data[d+1] = ham[h+6];
		data[d+2] = ham[h+5];
		data[d+3] = ham[h+3];
	}
}
