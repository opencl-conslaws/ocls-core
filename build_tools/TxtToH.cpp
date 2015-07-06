/**
  *  This file is part of the OpenCL-ConsLaws framework
  *  Copyright (C) 2010, 2011, 2012, 2015 SINTEF, Oslo, Norway.
  *
  * This program is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
  *
  */



#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cstring>

using namespace std;

/**
 * This app simple takes each line of an input text-file, and writes it out as a
 * header file, suitable for compilation into an app.
 */
int main(int argc, char** argv) {
	char* input_file;
	char* output_file;
	ifstream input;
	ofstream output;
	string line;

	if (argc != 3) {
		cout << "Usage: " << argv[0] << " <inputfile> <outputfile>" << endl;
		exit(-1);
	}
	else {
		input_file = argv[1];
		output_file = argv[2];
		if (strcmp(input_file, output_file) == 0) {
			cout << "Cannot use the same input and output file." << endl;
			cout << "Input was " << input_file << ", " << endl;
			cout << "Output was " << output_file << "." << endl;
			exit(-1);
		}
		else {
			cout << "Reading from " << input_file << ", writing to " << output_file << "." << endl;
		}
	}

	input.open(input_file);
	if(!input.good()) {
		cout << "Error opening " << input_file << " for reading." << endl;
		exit(-1);
	}

	output.open(output_file);
	if (!output.good()) {
		cout << "Error opening " << output_file << " for writing." << endl;
		exit(-1);
	}

	output << "#include <string>" << endl;
	output << "static std::string source = \"\\" << endl;
	while(getline(input, line)) {
		output << line << "\\n\\" << endl;
	}
	output << "\";" << endl;

	return 0;
}
