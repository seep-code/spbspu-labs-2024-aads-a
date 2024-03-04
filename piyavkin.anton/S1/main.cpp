#include <iostream>
#include "list.hpp"
#include "inputlist.hpp"
#include "output.hpp"

int main()
{
  using namespace piyavkin;
  // size_t size = 0;
  // size_t max_size_list = 0;
  // std::pair< std::string, piyavkin::List< unsigned long long >* >* pairs = nullptr;
  // try
  // {
  //   pairs = inputList(std::cin, size, max_size_list);
  //   output(std::cout, pairs, size, max_size_list);
  // }
  // catch (const std::invalid_argument& e)
  // {
  //   std::cerr << e.what() << '\n';
  //   freeMemory(pairs, size);
  //   return 1;
  // }
  // catch (const std::logic_error& e)
  // {
  //   std::cout << 0 << '\n';
  //   return 0;
  // }
  // catch (const std::exception& e)
  // {
  //   std::cerr << e.what() << '\n';
  //   return 1;
  // }
  // std::cout << '\n';
  // freeMemory(pairs, size);
  List< int > list({ 1,2,2,3,3,3,3,3,3,4,4,4,4,4,5 });
  List< int > list2(5, 5);
  std::cout << list.size() << '\n';
  List< int > list3(list2.begin(), list2.end());
  list.unique();
  std::cout << list.size();
}
