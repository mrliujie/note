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


int customFilter(const struct dirent *pDir)
{
	if (pDir->d_type == DT_DIR && strstr( pDir->d_name,".") == NULL)
    {
        return 1;
    }
    return 0;
}

std::vector<std::string> split(std::string str, std::string pattern)
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


std::vector<std::string> readImagePaths(std::string folder_path)
{   
    char& refStr = folder_path.back();
    if ( refStr != '/')
    {
        folder_path += '/';
    }

    std::vector<std::string > file_paths;
    struct dirent **namelist;
    int ret = scandir(folder_path.c_str(), &namelist, customFilter, versionsort);       
    if (ret < 0)
    {
        perror("scandir");
    }
    else
    {
        for (int i = 0; i < ret; i++)
        {
            std::string file_name = namelist[i]->d_name;
            std::cout << "file_name " << file_name << std::endl;
            file_paths.push_back(folder_path + file_name);
            free(namelist[i]);
        }
        free(namelist);
    }
    return file_paths;
}

void get_data(std::string filename, std::vector<float > & sorce, std::vector<float > & timestamp, std::vector<float > & distance)
{
    std::fstream in;
	in.open(filename.c_str(), std::ios::in); 
	while( !in.eof())
	{      
		std::string strBuff;
        getline(in,strBuff); 
        if (!strBuff.empty())
        {
            std::vector<std::string> str = split(strBuff, "\t");
            
           	if ( str.size() > 3)
           	{
           		float single_score = std::stof(str[1]);
           		std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(5) << "score " << single_score << std::endl;
    			// sleep(1);
           		float single_timestamp = std::stof(str[2]);
           		// std::cout << "distance " << str[3] << std::endl;
           		float single_distance = std::stof(str[3]);
           		sorce.push_back(single_score);
           		timestamp.push_back(single_timestamp);
           		distance.push_back(single_distance);
           		float test = distance.back();


            }
        }
	}

}


int main(int argc,char **argv) 
{
	std::string read_path = "/home/jumei/dataset/75mm/resize";
	std::vector<std::string > dir = readImagePaths(read_path);

	std::string oScoreFileName = read_path + "/socre.txt";
    std::ofstream oScorefile(oScoreFileName);
    if (!oScorefile.is_open())
    {
        std::cerr << "could not open output " << oScoreFileName << std::endl;
        exit(0);
    }

	std::string oDisFileName = read_path + "/distance.txt";
    std::ofstream oDisfile(oDisFileName);
    if (!oDisfile.is_open())
    {
        std::cerr << "could not open output " << oDisFileName << std::endl;
        exit(0);
    }

	std::string oTimeFileName = read_path + "/timestamp.txt";
    std::ofstream oTimefile(oTimeFileName);
    if (!oTimefile.is_open())
    {
        std::cerr << "could not open output " << oTimeFileName << std::endl;
        exit(0);
    }

    for (int i = 0; i < dir.size(); i++)
    {
        std::cout << "file_name " << dir[i] << std::endl;
        char& refStr = dir[i].back();
   		if ( refStr != '/')
    	{
   	    	dir[i] += '/';
    	}
    	std::string filename = dir[i] + "log.txt";
	    std::ifstream iTestfile(filename);
	    if (!iTestfile.is_open())
  	    {
	        std::cerr<<"ERROR::: log File  " << filename <<" dont exist!!!!"<< std::endl;
   	        continue;
 	    }
    	std::cout << "reading " << filename << std::endl;
    	std::vector<float > score;
    	std::vector<float > timestamp;
    	std::vector<float > distance;
    	get_data(filename, score, timestamp, distance);
    	for (int j = 0; j < score.size(); j++)
    	{
    		std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(5) << "score " << score[j] << std::endl;
    		// sleep(0.5);
    		oScorefile << std::setiosflags(std::ios::fixed) << std::setprecision(5) << score[j];

    		if (j != score.size() - 1)
    		{
    			oScorefile << ",";
    		}
    		else
    		{
    			oScorefile << std::endl;
    		}
    	}

   		for (int k = 0; k < distance.size(); k++)
   		{
   			oDisfile << std::setiosflags(std::ios::fixed) << std::setprecision(5) << distance[k] ;
   		
   			if ( k != distance.size() - 1)
   			{
   				oDisfile << ",";
   			}
   			else
   			{
   				oDisfile << std::endl;
   			}
   		}

   		for (int z = 0; z < timestamp.size(); z++)
   		{
   			oTimefile << std::setiosflags(std::ios::fixed) << std::setprecision(5) << timestamp[z];

   			if ( z != timestamp.size() - 1)
   			{
   				oTimefile << ",";
   			}
   			else
   			{
   				oTimefile << std::endl;
   			}
   		}

    }
    oTimefile.close();
    oDisfile.close();
    oScorefile.close();
    return 0;
}