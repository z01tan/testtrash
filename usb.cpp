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
  std::cout<<"starting"<<std::endl;
  std::string filePattern("SourceData");
  std::string s = devFind(std::string("/dev/"),std::string("sdb"));
  
  if(s=="")
    {
      std::cout<<"device not found"<<std::endl;
      return std::string("-1");
    }

  std::string dev = "/dev/"+s;

  int fd;

  std::string file = devFind(std::string("/mnt/usb/"),std::string("SourceData"));
  if(file=="")
    {
      if(mount(dev.c_str(),"/mnt/usb","vfat",MS_NOATIME,NULL))
	{
	  if(errno == EBUSY)
	      std::cout<<"already mounted"<<std::endl;
	  else
	    {
	      std::cout<<"Mount error: "<<std::string(strerror(errno))<<std::endl;
	      return std::string("-1");
	    }
	}
      file = devFind(std::string("/mnt/usb/"),std::string("SourceData"));
    }
  std::string tmpfile = file;
  file=std::string("/mnt/usb/")+file;

  std::cout<<"device opened successful"<<std::endl;
  if(access(file.c_str(),F_OK)==-1){
    fd=-2;
    std::cout<<"archive file not found"<<std::endl;
    std::stringstream s;
    s<<fd;
    return s.str();
  }
  std::cout<<"Found file: "<<file<<std::endl<<"Start inflating"<<std::endl;
  std::string comm("unzip -o ");
  comm+=file;                         //comm = "unzip <filename>"
  system(comm.c_str());               //unzip file
  file = tmpfile;
  unsigned pos = file.find(".bin");
  file.replace(pos,4,".XML");            //file.bin -> file.XML
  if(access(file.c_str(),F_OK)==-1)
    {
      fd=-3;
      std::cout<<"archive not propperly inflated\n"<<std::endl;
      std::stringstream s;
      s<<fd;
      return s.str();
      
    }
  else
    {
      std::cout<<"unpack successful."<<std::endl;
      return file;
	  
    }
  return file;
}


int UMount(std::string dev)
{
  return umount(dev.c_str());
}
