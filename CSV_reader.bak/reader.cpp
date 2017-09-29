#include "reader.h"

DataMap Reader::match(long int input_timestamp)
{
	for( size_t i = 0; i < list_data.size(); i++)
	{
		// std::cout << "input_timestamp " << input_timestamp 
		// << "\t list " << list_data[i].IMG_timestamp << std::endl;
		if (input_timestamp == list_data[i].IMG_timestamp)
		{
			return list_data[i];
		}
	}
	return DataMap();
}

float Reader::cal_distance(std::string src_filename, std::string input_file)
{
  long int src_IMG_timestamp = get_IMG_timestamp( src_filename);
  long int input_IMG_timestamp = get_IMG_timestamp( input_file);
  DataMap src_data = match(src_IMG_timestamp);
  DataMap input_data = match(input_IMG_timestamp);
  if ( src_data.distance == -1 || input_data.distance == -1)
  {
    return -1;
  }
  float dis = sqrt(pow(src_data.Northing - input_data.Northing, 2) 
    + pow(src_data.Easting - input_data.Easting, 2));
  return dis;
}

std::vector<std::string> Reader::split(std::string str, std::string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str += pattern;
    int size = str.size();

    for (int i = 0; i < size; i++)
    {
        pos = str.find(pattern, i);
        if (pos < size)
        {
            std::string s = str.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return result;
}

void Reader::read_manifest(std::string filename)
{
    std::ifstream ifile(filename);
    if (!ifile.is_open())
    {
    	std::cerr << "open  " << filename << " error!"<<std::endl;
    	exit(0);
    }
    ifile.close();
    std::fstream in;
    in.open(filename.c_str(), std::ios::in); 

    while( !in.eof())  
    {
        std::string strBuff;
        getline(in,strBuff); 

        if (!strBuff.empty())
        {
    	    std::vector<std::string> str = split(strBuff, "\t");
      		if (str.size() > 3)
      		{
      			DataMap data;
      			data.IMG_timestamp = std::stol(str[0]);
      			data.IMU_timestamp = std::stol(str[1]);

      			std::vector<std::string > str_utm = split(str[2], ",");
      			if (str_utm.size() > 1)
      			{
      				data.Easting = std::stod(str_utm[0]);
      				data.Northing = std::stod(str_utm[1]);
      			}
      			data.distance = std::stoi(str[3]);
      			list_data.push_back(data);
      		}
        }
    }
}

long int Reader::get_IMG_timestamp(std::string filename)
{
    std::vector<std::string> str = split(filename, "/");
    std::string image_name = str.back();
    std::vector<std::string> img_name_num = split(image_name, ".");
    long int result = std::stol(image_name);
    return result;
}
