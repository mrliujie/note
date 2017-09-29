#ifndef __READER_H__
#define __READER_H__

#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

class DataMap
{
public:
	long int IMG_timestamp;
	long int IMU_timestamp;
	double Northing;
	double Easting;
	int distance;
	DataMap()
	{
		IMG_timestamp = -1;
		IMU_timestamp = -1;
		Northing = -1;
		Easting = -1;
		distance = -1;
	}
};

class Reader
{
public:
	Reader(std::string filename)
	{
		manifest_name = filename;
		read_manifest(manifest_name);
	}

	DataMap match(long int input_timestamp);
	long int get_IMG_timestamp(std::string filename);
	float cal_distance(std::string src_filename, std::string input_file);


private:
	std::string manifest_name;
	std::vector<DataMap> list_data;

	void read_manifest(std::string filename);
	std::vector<std::string> split(std::string str, std::string pattern);

};
#endif