#include <sys/mount.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "supp.h"
#include <unistd.h>
#include <sstream>
#include <iostream>

std::string MountNRead()
{
  system("sh sourcedatafind.sh");
  std::fstream f;
  f.open("sourcefile");
  std::string line, file;
  std::getline(f, line);
  return line;
}


int UMount(std::string dev)
{
  if( umount(dev.c_str()))
    {
      std::cout<<"Umount error: "<<std::string(strerror(errno))<<std::endl;
      return -1;

    }
  else
    {
      std::cout<<"umount successfully"<<std::endl;
    }
  return 0;
}
