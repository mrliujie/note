#include <unistd.h>
#include <stdio.h>
#include <stdlib.h> 
#include <errno.h>
#include <dirent.h>
#include <iostream>
#include <string.h>

int customFilter(const struct dirent *pDir)
{
    if (pDir->d_type == DT_REG )
    {
        return 1;
    }
    return 0;
}

int main(int argc,char **argv) {
	if (argc < 2)
	{
		std::cerr << " folder name " << std::endl;
		return 0;
	}
   
    std::string file_name(argv[1]);
    struct dirent **namelist;
    int n;
    n = scandir(file_name.c_str(), &namelist, customFilter, alphasort);
    if (n < 0)
    {
        perror("scandir");
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            printf("%s\n", namelist[i]->d_name);
            free(namelist[i]);
        }
        free(namelist);
    }
}
