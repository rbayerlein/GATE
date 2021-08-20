#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

#define BUFFER_SIZE 65536 // = elements per buffer


using namespace std;

struct ids {
	short txID1, axID1, txID2, axID2, tof;
};

short num_rings_per_unit = 84;
short num_crys_per_ring = 840;
short num_crys_per_module_tx = 35;	//5 blocks * 7 crys per block transaxial
int StartIndex = floor(num_crys_per_module_tx/2) - (num_crys_per_module_tx+1)%2;

int main(int argc, char **argv) {

	if (argc != 3) {
		cout << "Usage: " << argv[0] << " [ucdcoin_input]" << " [lm_output] " << endl;
		return 1;
	}
	cout << "start... (may take a while)" << endl;
// read arguments
	string infile_fullpath = argv[1];
	string outfile_fullpath = argv[2];

// open input

	FILE *pInputFile = fopen(infile_fullpath.c_str(), "rb");
	if (pInputFile == NULL) { // check return value of file pointer
		cerr << infile_fullpath << " cannot be opened." << endl;
		exit(1);
	}
	ids * pids_in = new ids[BUFFER_SIZE];

// open output file
	FILE *pOutputFile = fopen(outfile_fullpath.c_str(), "wb"); 
	ids *pids_out = new ids[BUFFER_SIZE];

// read from file
	int buffer_counter = 1;
	while(!feof(pInputFile)){
		int read_ct = fread(pids_in, sizeof(ids), BUFFER_SIZE, pInputFile);	
	// -> read BUFFER_SIZE events from pInputFIle, each element having size 'sizeof(ids)' and it's read into buffer pids_in. 
	// The return value is the number of elements read from file into the buffer at a time
		for (int i= 0; i < read_ct; i++){

			// tof
			pids_out[i].tof = pids_in[i].tof * (-1); 
			// catch invalid time of flight values
			if (pids_out[i].tof >127 ){
				pids_out[i].tof = +127;
			}
			else if (pids_out[i].tof < -128){
				pids_out[i].tof = -128;
			}
			
			// add axial gap
			pids_out[i].axID1 = pids_in[i].axID1 + (pids_in[i].axID1 / num_rings_per_unit);
			pids_out[i].axID2 = pids_in[i].axID2 + (pids_in[i].axID2 / num_rings_per_unit);
			if(pids_out[i].axID1 > 679 || pids_out[i].axID2 > 679) cout << "found entry larger 679: i=" << i << ", values: " << pids_out[i].axID1 << ", " << pids_out[i].axID2 << endl;

			// transaxial adjustment

			pids_out[i].txID1=( pids_in[i].txID1-StartIndex+num_crys_per_ring/4*3)%num_crys_per_ring;
			pids_out[i].txID2=( pids_in[i].txID2-StartIndex+num_crys_per_ring/4*3)%num_crys_per_ring;  



			if(i == BUFFER_SIZE-1){
				// write data to output file
				//cout << "writing data from buffer " << buffer_counter << endl;
				fwrite(pids_out, sizeof(ids), BUFFER_SIZE, pOutputFile);
			}
		}// end of for loop
		buffer_counter++;
	}// end of while loop

	delete[] pids_in;
	delete[] pids_out;
	fclose(pInputFile);
	fclose(pOutputFile);

	cout << "done." << endl;

}			


//if(i==0) cout << pids_in[i].axID1 << "\t" << ((pids_in[i].axID1+1) / num_rings_per_unit)<< "\t"  << pids_out[i].axID1 << endl;