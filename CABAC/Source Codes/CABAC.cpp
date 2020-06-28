//============================================================================
// Name        : CABAC.cpp
// Description : CABAC main code 
//============================================================================


// including libraries
#include <iostream>
#include <math.h>
#include "QmCoder/qmcoder.h"
#include "FileIO/fileIO.h"
#include "PreProcessing/bitPlaneMap.h"


// defining functions
void printFileSize(int,char[], char[], int, int,int);
void shiftBit(bool *array, bool nextBit);
void printBoolArray(bool *array);
int getContext(bool *array);

using namespace std;

//Global Variables initialization
char *memblock;
int n;
int preprocess;

// Main program //
int main() {

	int flag1 = 0;
	int flag2 = 0;
	
	//User Interaction to enter file_name
	cout << "-----------------------------" << endl;
	cout << "WELCOME" << endl;
	cout << "-----------------------------" << endl;
	cout << "Enter the file to be encoded " << endl;
	char filename[1024];
	cin.getline(filename,1024);
    cout << "File has successfully uploaded" << endl;
	
	//User Interaction to enter the context
	while(flag1 == 0){
		cout << "-----------------------------" << endl;
		cout << "Enter the number of context" << endl;
		cout << "You can select only 1,2,3,4" << endl;
		cin >> n;
		//Enter the # of context dependency
		if(n == 1 || n == 2 || n == 3 || n == 4 )
		{
			flag1 = 1;
		}
		else{
			cout << "Invalid Entry..!!!!" << endl;
			cout << "Enter either 1,2,3 or 4" << endl;
			cout << "--------------------------" << endl;
		}
	}

	//User interaction to do preprocessing
	
	while(flag2 == 0){
		cout << "--------------------------" << endl;
		cout << "Select the option by entering 1 or 2: "<<endl;
		cout <<	"1. Do bitMap Preprocessing " << endl;
		cout << "2. Don't do Preprocessing " << endl;
		cin >> preprocess;
		
		if(preprocess == 1 || preprocess == 2)
		{
			cout << "You have chosen option : " << preprocess << endl;
			flag2 = 1; 
		}
		else{
			cout << "Invalid Entry..!!!!" << endl;
			cout << "Enter either 1 or 2" << endl;
			cout << "--------------------------" << endl;
		}
	}
	
	
//------------------------------------------------------------------------------//
//-----Main code begins--------------------------//

	//Array to store the previous (memory) for context bits
	bool *array = new bool[n];
	int i=0;
	while (i < n)
	{
		array[i] = 0;
		i++;
	}

	// Read the Input file
	//	char filename[] = "resources/image.dat";
	//read the contents of the file and store it in memblock
	memblock = readFileByBytes(filename);
	int originalFileSize = FileSizeinBytes;

	//Do PreProcessing
	//This will store the preprocessed file in *PreProcessed.dat* file
	if(preprocess == 1){
		bitmapProcessing(memblock,originalFileSize);
		//Pre-processed Input filename
		char filename1[] = "PreProcessed.dat";
		//read the contents of the file and store it in memblock
		memblock = readFileByBytes(filename1);
	}

	//output the data to this file
	FILE *fp;
	fp=fopen("CABACencoded.dat", "w+");
	//calling the parameterized constructor for CABAC
	QM obj(fp);
	//Initialize the encoder parameters
	obj.StartQM("encode");

	//Read the file till EOF
	while (!checkEOF()) {
		if (ReadBit() == 0){
			int gc =  getContext(array);
			obj.encode(0,getContext(array));
			shiftBit(array,0);
		}
		else{
			int gc = getContext(array);
			obj.encode(1,getContext(array));
			shiftBit(array,1);
		}
	}

	//Flush the remaining contents
	obj.Flush();
	cout << "Done Encoding :)";
	fclose(fp);

	//Output file Size
	char outputFilename[] = "CABACencoded.dat";
	char *opFilename = outputFilename;
	int encodedFileSize = getFileSize(opFilename);

	//Print the result
	printFileSize(n,filename,outputFilename,encodedFileSize,originalFileSize, preprocess);
//
//	cout << "Done/!!";
	//return 0;
}






/**
 * This function changes the context when a
 * new symbol is read.
 * Basically it shifts the array to the right.
 * In the bool array, position 0 is the most recent position.
 */
 
 
void shiftBit(bool *array, bool nextBit){
	int i = n-1;
	while (i >= 0)
	{
		if(i == 0){
			array[i] = nextBit;
		}
		
		else{
			array[i] = array[i-1];
		}
		
	i = i-1;
	}
}
 
 
void printBoolArray(bool *array){
	int i = 0;
	while (i < n)
	{
		cout << array[i] << " ";
		i++ ;
	}
	cout << endl;
}


/**
 * THis function computes the context based
 * on the bool array.
 * e.g. if the bool array has entries {0,1,1},
 * then the context is 2^(0)*0 + 2^(1)*1 + 2^(2)*1 = 5
 */
int getContext(bool *array){
	int context = 0;
	int i = 0;
	
	while (i < n)
	{
		context += pow(2,i)*array[i];
		i++;
	}
	
	return context;
}

/**
 * This function prints the input and output info.
 */
void printFileSize(int n, char inputFilename[], char outputFilename[], int encodedFileSize, int originalFileSize, int preprocess){

	// Displaying input information	
	cout << "--------------------------------" <<  endl;
	cout << "INPUT Information" << endl ;
	cout << "--------------------------------" << endl ;

	
	cout << "Input File Name      : " <<inputFilename << endl;
	cout << "Context Dependency   : " << n << endl;
	
	if(preprocess == 1){
		cout << "PreProcessing :  YES" << endl;
	}
	else{
		cout << "PreProcessing : NO " << endl;
	}
	
	cout << "Original File size   : " << originalFileSize << " bytes" << endl;
	
	
	// Displaying output information
	cout << "--------------------------------" <<  endl;
	cout << "OUTPUT Information" << endl ;
	cout << "--------------------------------" << endl;
	
	
	cout << "Output File Name     : " << outputFilename << endl;
	cout << "Compressed File size : " << encodedFileSize << " bytes" << endl;
	cout << "Compression Ratio    : " << (float)encodedFileSize/originalFileSize*100 << "%" << endl << endl;
	cout << "---------------------------------" << endl;
}
