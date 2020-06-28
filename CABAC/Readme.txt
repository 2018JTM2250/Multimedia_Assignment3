------------------------------------
CABAC
 
------------------------------------

This file takes as input binary data (image, text, audio etc.), reads the data bit by bit and encodes 
the data
using Context Adaptive QM Coding.

---------------------------------------------------------------------------------------------------------------------

To compile the file : g++ -g -Wall CABAC.cpp FileIO/fileIO.cpp PreProcessing/bitPlaneMap.cpp QmCoder/qmcoder.cpp -o cabac
To run the file : ./cabac

----------------------------------------------------------------------------------------------------------------------
Code works as follows :

Step 1 : After running the application, user has to enter the complete path of the file
Step 2 : After uploading the file, user has to enter the context value. 
         It can be 1,2,3 or 4 but the context value gretaer than 4 and smaller than 1 will lead to an error.
Step 3 : User has to select file preprocessing by entering either 1 or 2
Step 4 : After that, the main code runs and compress the input files and display result accordingly

Note : to find compression ratio(%) of a file,
       compression ratio(%) = (Uncompressed_file_size / compressed_file_size ) * 100 

------------------------------------------------------------------------------------------------------------
Thank You