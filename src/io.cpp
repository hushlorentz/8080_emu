#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int openFile(string filePath, unsigned char *buffer, int bufferSize)
{
  ifstream input;
  input.open(filePath, ios::out | ios::binary);

  if (input) {
    input.read((char *)buffer, bufferSize);
    input.close();

    if (input.gcount() != bufferSize) {
      printf("Number of bytes read does not match number expected!\n");
      return 1;
    }
  } else {
    return 1;
  }

  return 0;
}
