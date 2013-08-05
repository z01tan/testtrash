#include <fstream>
#include <dirent.h>

std::string devFind(const std::string direct,const std::string name)
{
  DIR *d;
  struct dirent *dir;
  d=opendir(direct.c_str());
  
  if(d!=NULL)
    {
      while((dir = readdir(d))!=NULL)
	{
	  std::size_t found = std::string(dir->d_name).find(name);
	  if(found!=std::string::npos)
	    return std::string(dir->d_name);
	}
      closedir(d);
    }
  return "";
}
