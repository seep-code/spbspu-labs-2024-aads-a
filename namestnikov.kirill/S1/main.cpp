#include <iostream>
#include "forward_list.hpp"
#include "iterators.hpp"
#include "node.hpp"
#include "input_output_lists.hpp"

int main()
{
  using namespace namestnikov;
  //ForwardList<int> Fwd;
  //for (int i = 0; i < 10; ++i)
  //{
  //  Fwd.push_front(i);
  //}
  //std::cout << Fwd.size();
  //Fwd.reverse();
  //Fwd.print();
  ForwardList<pair_t> list;
  inputLists(std::cin, list);
  std::cout << list.size();
}
