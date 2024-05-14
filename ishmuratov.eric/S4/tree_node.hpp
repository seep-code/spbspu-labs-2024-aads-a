#ifndef TREENODE_HPP
#define TREENODE_HPP

namespace ishmuratov::detail
{
  template< class Key, class Value >
  struct TNode
  {
    std::pair< Key, Value > data;
    int height;
    TNode * parent;
    TNode * left;
    TNode * right;

    TNode(const Key key, const Value value):
      data(std::make_pair(key, value)),
      parent(nullptr),
      left(nullptr),
      right(nullptr),
      height(0)
    {}
  };
}

#endif