
#include <iostream>
#include <vector>

// OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc.hpp>

#ifdef USE_CONTRIB
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <opencv2/xfeatures2d.hpp>
#endif

#include <dirent.h>
#include "string.h"
#include <unistd.h>
#include "sys/time.h"

using namespace std;
using namespace cv;

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

int customFilter(const struct dirent *pDir)
{
    if (pDir->d_type == DT_REG && strstr( pDir->d_name,".bmp") != NULL)
    {
        return 1;
    }
    return 0;
}

std::vector<string> readImagePaths(std::string folder_path)
{
    char& refStr = folder_path.back();
    if ( refStr != '/')
    {
        folder_path += '/';
    }

    std::vector<string > file_paths;
    struct dirent **namelist;
    int ret = scandir(folder_path.c_str(), &namelist, customFilter, alphasort);
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
std::string get_IMG_timestamp(std::string filename)
{
    std::vector<std::string> str = split(filename, "/");
    std::string image_name = str.back();
    std::vector<std::string> img_name_num = split(image_name, ".");
    return img_name_num[0];
}

int main(int argc,char **argv) {
    if (argc < 2)
    {
        std::cerr << " folder name " << std::endl;
        return 0;
    }

    std::string file_name(argv[1]);
    vector<string> all_paths = readImagePaths(file_name);

    for ( size_t i = 0; i < all_paths.size(); i++)
    {
        string sPrefix = get_IMG_timestamp(all_paths[i]);
        Mat img = imread(all_paths[i]);
        string realname = "/home/jumei/convert/" + sPrefix + ".jpg";
        imwrite(realname.c_str(), img);
    }

}
