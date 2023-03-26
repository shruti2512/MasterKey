//**************HEADER FILES*************************************************************

#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <string.h>
#include <cstring>
#include <limits.h>
#include <math.h>
#include "md5wrapper.h"


using namespace std;

//****************************************************************************************

char bloomfilter[8192];		//bloomfilter array that is the final code word to be sent across to locks
int counter,count,c;


//*****************Secrets for all 30 locks that we wish to open *********************************

string secret[30]={"This is a general secret","This is the secret for your car",
				  "This is the secret for your garage door","This is the secret for your main door for the house",
				  "This is your laptop log in","Door#1","Door#2","Door#3","Door#4","Door#5","Door#6",
				  "Door#7","Door#8","Door#9","Door#10","Door#11","Door#12","Door#13","Door#14","Door#15"
				  "Door#16","Door#17","Door#18","Door#19","Door#20","Door#21","Door#22","Door#23",
				  "Door#24","Door#25"};
				 
//*********************** Converts decimal number into binary number  **********************************

void convert_bin (unsigned long ccc)
		{
			string temp_loop="abc";
			unsigned int product=1,sum=0;
			int loop=0;
			int rem[100], rem_t[100];
			int c=0;
						
			
			while(ccc!=0)
			{
				rem[c]=ccc%2;  
				ccc=ccc/2;       
				c++; // c-3 is the length of the bin number that we want(truncated by 3)
			}
            int lenofbin=c-3;
			//cout<<" bin value  ";		
			for(int b=c-4;b>=0;b--) // manupulate to discard first 3 bits for the index
			{
				
				rem_t[loop]=rem[b];
				//cout<<rem_t[loop];	//to display binary value of each chunk after discarding 3 bits		
				loop++;  
							
			}
			
			for(int dd=lenofbin-1;dd>0;dd--)
			{
				product=product*2;    // to find the 2 pow exp. 
			}
			
			for(int bb=0;bb<loop-1;bb++)
			{
				sum = sum+ rem_t[bb]*product;  // calculate the decimal eqv.of binary
				product=product/2;
				if(product==1&& rem_t[bb]==1)  // last bit adjustment
				   sum=sum+1; 
			}
			
				bloomfilter[sum]='1';  // set the corresponding bit in bloomfilter to '1' 
									  // the index corresponds to chunk value
				
		}

//************************ generates random number***************************************

unsigned long int lehmer(long int s)
{
     	 static unsigned long long a = 2007, b = 4194301, c = 2147483647, z = b;
		 if ( s < 0 )
		 {
		  s = -s; a = s;
		 }
		 z = (a + b * z) % c;
		 return z % s;
}


//****************************   MAIN MODULE   *********************************************

int main( int argc, char** argv )
{
   counter =0;
   for(int v=0;v<8192;v++)  //bloomfilter initialization
      bloomfilter[v]='0';

  string combined_str[30];
  string time_variant_parameter;

  time_t rawtime;				
  time ( &rawtime );			//current raw time
  int random_number;
  srand ( time(NULL) );
  random_number=rand()%100;  //generates a random number
  char buffer[10];
  itoa(random_number,buffer,10); //converts random number to string
  time_variant_parameter = strcat(ctime(&rawtime),buffer);	// TVP =current time+random number

  for(int jjj=0;jjj<30;jjj++)
  {
	 combined_str[jjj]=time_variant_parameter+secret[jjj];
	 md5wrapper md5;

	 std::string hash1 = md5.getHashFromString(combined_str[jjj]);  // hashing TVP and the shared secret
	 
	 string hash_str1="xyz";
	 hash_str1 = (string)hash1;
	 string add_str=hash_str1+secret[c];
	 std::string hash2 = md5.getHashFromString(add_str);

	 string hash_str2="xxx";
	 hash_str2 = (string)hash2;

	 char xyz[100];
	 char chunk[100][100];
		
	 int length_of_hash;
	 const char *hash_ptr;			// length of hash
	 hash_ptr = hash_str2.c_str();
	 length_of_hash=strlen(hash_ptr);
 	
   	 for (int i=0;i<=length_of_hash;i++)
		xyz[i]=hash_str2[i];

	 int k=0;
	 int no_of_chunks;
	 no_of_chunks = length_of_hash/4;
	
 	 while(k<length_of_hash)
	 {
	  for (int p=0;p<no_of_chunks;p++)
	  {
	    //cout<<"\n The Chunk # "<<p+1<<" ";
		for(int q=0;q<4;q++)
		{
				chunk[p][q]=xyz[k++];
				//cout<<chunk[p][q];   // Display the corresponding chunk value in Hex
		}	
	  }  
	 }
	
		char chunk_oneD[100];
		char chunk1[100][100];
		
		int aa=0;
		int y=0;
		int some;
		string exp_str[50];


		//cout<<"\n**********************************\n";
		for(some =0;some<no_of_chunks;some++)
		{
			for(int x=0;x<4;x++)
			{   
				chunk1[y][x]=chunk[some][x];
				chunk_oneD[aa]=chunk1[y][x];
				//cout<<chunk1[y][x];y++;     //prints Hex values of separate chunks
				aa++;
			}
		}
			  
			   int h=0;
			   for(int hh=0;hh<8;hh++)
			   {
				   std::string s1(1,chunk_oneD[h]);
				   std::string s2(1,chunk_oneD[h+1]); 	
				   std::string s3(1,chunk_oneD[h+2]);
				   std::string s4(1,chunk_oneD[h+3]);
				   std::string total=s1+s2+s3+s4;
				   h+=4;
				   const char* total_ptr;
				   total_ptr=total.c_str();
				   unsigned long xxx1 = strtoul(total_ptr,0,16);
				   //cout<<"\nDecimal value of the chunk # "<<hh+1<<" is "<<xxx1; //prints decimal value of chunk
				   convert_bin(xxx1);		
			   }
			
		//cout<<"\n\nBloomfilter:: \n\n";  
		for(int pp=0;pp<8192;pp++)
		{
			//cout<<bloomfilter[pp];          // Prints bloomfilter after every iteration(for every secret)
			if(bloomfilter[pp]=='1')
				counter++;
		}
		count=counter;
		counter=0;
	  }
  cout<<"******************************************************\n";		
  cout<<"This is the Bloomfilter without the random bits(ITS): \n"<<bloomfilter<<"\n";
  cout<<"******************************************************\n";
  

int temp_count=0;
int a=4096-count;

//*********** Ensure that more than half bits of the bloomfilter are set to '1'***********
//*********** so that the all BFs look same **********************************************

do{	
			unsigned long int r[4096];
			for(int z=0;z<a;z++)
			 {
			  r[z]=lehmer(8192);	// Use Lehmer function to randomly set the bits in the BF to '1'
			  bloomfilter[r[z]]='1';
			 }
			
			  for(int ppp=0;ppp<8192;ppp++)  // check how many bits are set to '1'
				if(bloomfilter[ppp]=='1')
					temp_count++;
			
			  count=temp_count;
			  temp_count=0;
}while(count<=4096);

//************   To Display the bloomfilter  **********************************************
			string string_temp;	
		cout<<"\n\nBloomfilter:: \n";  
		for(int pp=0;pp<8192;pp++)
		{
			//string_temp[pp]=bloomfilter[pp];
			cout<<bloomfilter[pp];
		}
		
		cout<<"Value of 'bloomfilter' is: "<<bloomfilter<<"\n";


//******************************************************************************************		
		getchar();
}

   

