#include <unistd.h>
#include <stdio.h>
#include <stdlib.h> 
#include <errno.h>
#include <dirent.h>
#include <iostream>
#include <string.h>
#include <vector>
#include <fstream>

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


std::vector<std::string> read(std::string filename)
{
    std::fstream in;
    in.open(filename.c_str(), std::ios::in); 
    std::vector<std::string> result;
    while( !in.eof())
    {
        std::string strBuff;
        getline(in,strBuff); 
        if (!strBuff.empty())
        {
            std::vector<std::string> str = split(strBuff, "\t");
            if (str.size() > 1)
            {   
                int id = atof(str[1].c_str());       
                std::cout << id << " name : " << str[0] << std::endl;
                result.push_back(str[0]);
            }
        }
    }
    in.close();
    return result;
}


int main(int argc,char **argv) 
{
    std::string filename = "./ValidData.txt";
    std::vector<std::string> validDataPath = read(filename);
    for ( int i = 0; i < validDataPath.size(); i++)
    {
        std::cout << validDataPath[i] << std::endl;
    }
}
