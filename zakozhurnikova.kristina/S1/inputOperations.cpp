#include "inputOperations.hpp"

void zakozhurnikova::readList(List< pair >& list, std::istream& in)
{
  std::string stringName;
  while (in)
  {
    in >> stringName;
	  size_t number = 0;
	  List< size_t > numbers;
	  while (in >> number)
	  {
	    numbers.push_back(number);
	  }
	  if (!in.eof())
	  {
	    in.clear();
	  }
	  auto pair = std::make_pair(stringName, numbers);
	  list.push_back(pair);
  }
}
