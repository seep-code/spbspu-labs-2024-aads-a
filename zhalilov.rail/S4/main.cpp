#include <iostream>
#include <fstream>

#include "getMaps.hpp"
#include "mapMaster.hpp"

int main(int argc, char *argv[])
{
  std::map < std::string, zhalilov::primaryMap > maps;
  if (argc == 2)
  {
    std::ifstream file(argv[1]);
    zhalilov::getMaps(maps, file);
  }
  else
  {
    std::cerr << "No file argument";
    return 1;
  }

  zhalilov::MapMaster master(maps);
  std::string cmdOutput;
  while (std::cin)
  {
    try
    {
      master.doCommandLine(std::cin, cmdOutput);
      std::cout << cmdOutput << '\n';
    }
    catch (const std::invalid_argument &e)
    {
      std::cout << "<INVALID COMMAND>";
    }
    catch (const std::exception &e)
    {
      std::cerr << e.what();
      return 1;
    }
  }
}
