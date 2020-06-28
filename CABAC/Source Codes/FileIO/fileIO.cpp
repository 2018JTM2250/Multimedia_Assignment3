//============================================================================
// Name        : fileIO.cpp
// Description : Reading and Writing files  
//============================================================================


// including header file "fileIO.h"
#include "fileIO.h"

// declaring a global variable for storing the size of the file in Bytes
int FileSizeinBytes;

// Stream class to write on files
std::ofstream myFile;

using namespace std;

// function to get size of the file
int getFileSize(char *filename){
	
	streampos size;  // stream position type
	
	//Stream class to read from files
	// ios::in - open a file for reading
	// ios::ate-Open a file for output and move the read/write control to the end of the file.
	ifstream file (filename, ios::in|ios::binary|ios::ate);
	
	if(file.is_open()) {
		//cout << "Reading size of the file" << endl;
	    size = file.tellg();
	    file.close();
	}
	else{
		cout << "opening desired file for getting size of the file is unsuccessful" << endl;
	}
	return size;
}

// function to reaf content of the file
char* readFileByBytes(char* fileName){
		
		streampos size;   // stream position type
		char * memblock;
		
		//Stream class to read from files
		// ios::in - open a file for reading
		// ios::ate-Open a file for output and move the read/write control to the end of the file.
		 ifstream file (fileName, ios::in|ios::binary|ios::ate);
		 
		if (file.is_open())
		  {
			//cout << "Reading data of the file" << endl;
		    size = file.tellg();   // returns the current “get” position of the pointer in the stream
		    memblock = new char [size];
		    file.seekg (0, ios::beg);
		    file.read (memblock, size);
		    file.close();

		    cout << "File content has successfully placed in memory" <<endl;
		    FileSizeinBytes = size;

		  }
		  
		  else {
			  cout << "Opening file for reading content is unsuccessful" << endl ;
		  }

		  return memblock;
}

void WriteByte(unsigned char byte){
	cout << " " << (int)byte ;
}


/**
 * write the file.
 * @param -
 * 1. char data : data you want to write in the file.
 */
void writeFileByBytes(unsigned char data){
	//cout << "data written in file" << endl;
	myFile << data;
}

namespace Wr{
	unsigned char b;
	int s;
}

/**
 * This function write the bits given from MSB-> LSB
 * The first bit is written to the MSB, and so on the
 * 8th bit written to the LSB.
 */

void WriteBit(bool x)
{
    Wr::b |= (x ? 1 : 0) << (7-Wr::s);
    Wr::s++;

    if (Wr::s == 8)
    {
    	writeFileByBytes(Wr::b);
        Wr::b = 0;
        Wr::s = 0;
    }
}


void writeSingleCode(unsigned long code, char size)	{

	for(int i = 0; i < size; i++){
				bool x = ((code & (1 << i))?1:0);
					WriteBit(x);
		}
}

/**
 * Check the last bits status
 */
void checkStatusOfLastBit(){
	if(Wr::s <= 8){
		for (int i = 0; i < Wr::s; i++)
			Wr::b |= 0 << Wr::s;
		writeFileByBytes(Wr::b);
	}
}

/**
 * Open the file once.
 * @param-
 * 1. fileName : name of the file in which you
 * want to write
 */
void writePrepare(char *fileName){
	  //ios::app-> append to end of file.//No need to append
	  //ios::binary-> file is binary not text.
	  //ios::out -> write to the file
	  myFile.open(fileName, ios::out|ios::binary);
}

//for read bits
namespace RB {
	int pointer;
	unsigned char b1;
	int s1;
}

/**
 * This function reads the file bit by bit (starting from MSB)
 * with the help of 3 global variables.
 * pointer -> stores the current location in the memory block.
 * b1 -> stores the current symbol.
 * s1 -> stores the current count from the 8 bits.
 */
bool ReadBit() {
	if (RB::s1 == 0) {
		RB::b1 = memblock[RB::pointer++];
//		cout << (char)b1 << endl;
		RB::s1 = 8;
	}
//	cout << "s: " << RB::s1 << endl;

	bool bit = (RB::b1 >> (RB::s1-1)) & 1;
	RB::s1--;
	return bit;
}

/**
 * This function supports the read function and helps
 * find out the EOF
 */
bool checkEOF() {
	bool bit = 0;
	//IF the count is equal to FileSize and s1-> points to LSB.
	if (RB::pointer == (FileSizeinBytes) && RB::s1 == 0)
		bit = 1;
//	cout << "pointer: " << RB::pointer << endl;
	return bit;
}




/**
 * Close the file once all write operations done.
 */
void closeFile(){
	myFile.close();
}


