#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <dirent.h>
#include <iostream>
#include <string.h>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <assert.h>
#include <map>
#include <CSV_Reader.hpp>

void LLtoUTM(int eId, double Lat, double Long,  double& Northing, double& Easting, int& Zone);

int main(int argc,char **argv)
{
    std::string filename = "1.csv";
    CSVReader reader(filename.c_str());
    // std::string *ss = reader.getStamp(1,2);
    // std::string st = ss->c_str();
    // std::cout<< *ss <<" dd "<<st<<std::endl;
    std::map<long, float> str = reader.getColString(Time);

}
