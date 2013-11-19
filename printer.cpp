#include <unistd.h>
#include <iostream>
#include <dirent.h>
#include "supp.h"

bool CheckPrinter()
{
  std::string dev("/dev/");
  std::string str = devFind(dev,std::string("lp")).c_str();
  if(str=="") return false;
  dev+=str;  std::cout<<"Found printer: "<<dev<<std::endl;
  return true;
}
