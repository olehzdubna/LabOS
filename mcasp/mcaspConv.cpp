/*
*
*
*/

#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char** argv) {

  ifstream ifs;
  ifs.open(argv[1]);
  if(!ifs.is_open()) {
    cerr << "Cannot open input file: " << argv[1] << endl;
  }
  
  while(!ifs.eof() && !ifs.fail()) {
    int32_t r;
    ifs.read((char*)&r, sizeof(r));
    cout << (double)r/0x7fffffff << endl;
  }
  
  return 0;
}