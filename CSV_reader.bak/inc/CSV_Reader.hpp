#ifndef __CSV_READER_HPP_
#define __CSV_READER_HPP_

#include <map>
#include <assert.h>
#include <iomanip>
#include <vector>
#include <string.h>
#include <iostream>
#include <fstream>




enum EAircaftLogType
{
    Time = 1,
    System_time = 2,
    gyro_x = 3,
    gyro_y = 4,
    gyro_z = 5,
    acc_x = 6,
    acc_y = 7,
    acc_z = 8,
    GPS_latitude = 17,
    GPS_longitude = 18,
    GPS_altitude = 19,
    GPS_HorizontalAccuracy = 23,
    GPS_SatelliteNum = 26,
};

class CSVReader
{
public:
    CSVReader():verbose_(false){}
    CSVReader(const char *path, bool verbose = false)
    {
        verbose_ = verbose;
        assert(LoadCSV(path));
    }
    ~CSVReader(){}
    bool LoadCSV(const char *path);
    int GetParamFromString(std::string str, std::vector<std::string> &stringVec);
    std::string* getStamp(long row, long col);
    std::map<long, float>  getColString(long row);

private:
    std::map<long, std::map<long, std::string>> m_stringMap;
    std::string m_CSVName;
    std::vector<std::string> split(std::string str, std::string pattern);
    bool verbose_;

};



#endif 
