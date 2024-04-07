#ifndef FORWARD_LIST_HPP
#define FORWARD_LIST_HPP
#include <stdexcept>
#include <algorithm>
#include <type_traits>
#include <cstddef>

namespace zaitsev
{
  template< typename T >
  class ForwardList
  {
    struct Node
    {
      T value_;
      Node* next_;
      Node(const Node& other):
        value_(other.value_),
        next_(nullptr)
      {}
      Node(const T& value):
        value_(value),
        next_(nullptr)
      {}
      Node(T&& value):
        value_(value),
        next_(nullptr)
      {}
    };
    static void freeNodes(Node* head_node)
    {
      while (head_node)
      {
        Node* temp = head_node->next_;
        delete head_node;
        head_node = temp;
      }
    }

    template< bool IsConst >
    class BaseIterator
    {
      using prt_t = std::conditional_t< IsConst, const T*, T* >;
      using ref_t = std::conditional_t< IsConst, const T&, T& >;
      using node_t = std::conditional_t< IsConst, const Node*, Node* >;
    private:
      node_t node_;

    public:
      BaseIterator():
        node_(nullptr)
      {}
      BaseIterator(node_t node):
        node_(node)
      {}
      BaseIterator& operator++()
      {
        node_ = node_->next_;
        return *this;
      }
      BaseIterator operator++(int)
      {
        BaseIterator copy = *this;
        node_ = node_->next_;
        return copy;
      }
      ref_t operator*() const
      {
        return node_->value_;
      }
      prt_t operator->() const
      {
        return std::addressof(node_->value_);
      }
      bool operator!=(const BaseIterator& other) const
      {
        return node_ != other.node_;
      }
      bool operator==(const BaseIterator& other) const
      {
        return node_ == other.node_;
      }
    };

  public:
    using iterator = BaseIterator< false >;
    using const_iterator = BaseIterator< true >;

    ForwardList():
      head_(nullptr)
    {}
    ForwardList(const ForwardList& other)
    {
      if (!other.head_)
      {
        head_ = nullptr;
        return;
      }
      head_ = new Node(other.head_->value_);
      Node* head = head_;
      Node* cur = other.head_->next_;
      try
      {
        while (cur)
        {
          head->next_ = new Node(cur->value_);
          head = head->next_;
          cur = cur->next_;
        }
      }
      catch (const std::bad_alloc&)
      {
        freeNodes(head_);
        throw;
      }
    }
    ForwardList(ForwardList&& other):
      head_(other.head_)
    {
      other.head_ = nullptr;
    }
    ForwardList(size_t count, const T& value):
      head_(new Node(value))
    {
      Node* head = head_;
      try
      {
        for (size_t i = 1; i < count; ++i)
        {
          head->next_ = new Node(value);
          head = head->next_;
        }
      }
      catch (const std::bad_alloc&)
      {
        freeNodes(head_);
        throw;
      }
    }
    ~ForwardList()
    {
      freeNodes(head_);
    }

    bool operator==(const ForwardList& other)
    {
      return std::equal(cbegin(), cend(), other.cbegin(), other.cend());
    }
    bool operator!=(const ForwardList& other)
    {
      return !std::equal(cbegin(), cend(), other.cbegin(), other.cend());
    }
    bool operator<(const ForwardList& other)
    {
      return std::lexicographical_compare(cbegin(), cend(), other.cbegin(), other.cend());
    }
    bool operator>=(const ForwardList& other)
    {
      return !std::lexicographical_compare(cbegin(), cend(), other.cbegin(), other.cend());
    }
    bool operator>(const ForwardList& other)
    {
      return std::lexicographical_compare(other.cbegin(), other.cend(), cbegin(), cend());
    }
    bool operator<=(const ForwardList& other)
    {
      return !std::lexicographical_compare(other.cbegin(), other.cend(), cbegin(), cend());
    }


    iterator begin()
    {
      return iterator(head_);
    }
    iterator end()
    {
      return iterator();
    }
    const_iterator cbegin() const
    {
      return const_iterator(head_);
    }
    const_iterator cend() const
    {
      return const_iterator();
    }
    bool empty() const
    {
      return !head_;
    }
    T& front()
    {
      return head_->value_;
    }
    const T& front() const
    {
      return head_->value_;
    }
    void push_front(const T& value)
    {
      Node* new_head = new Node(value);
      new_head->next_ = head_;
      head_ = new_head;
    }
    void push_front(T&& value)
    {
      Node* new_head = new Node(value);
      new_head->next_ = head_;
      head_ = new_head;
    }
    void pop_front()
    {
      if (!head_)
      {
        throw std::runtime_error("List is empty");
      }
      Node* temp = head_;
      head_ = head_->next;
      delete head_;
    }
    void assign(size_t count, const T& value)
    {
      Node* new_head = nullptr;
      try
      {
        for (size_t i = 0; i < count; ++i)
        {
          Node* temp = new Node(value);
          temp->next_ = new_head;
          new_head = temp;
        }
      }
      catch (const std::bad_alloc&)
      {
        freeNodes(new_head);
      }
      freeNodes(head_);
      head_ = new_head;
    }
    void clear()
    {
      freeNodes(head_);
    }
    void swap(ForwardList& other)
    {
      Node* temp = other.head_;
      other.head_ = head_;
      head_ = temp;
    }

    size_t remove(const T& value)
    {
      size_t removed = 0;
      Node* prev = nullptr;
      Node* cur = head_;
      while (cur)
      {
        if (cur->value_ == value)
        {
          Node* temp = cur;
          if (prev)
          {
            prev->next_ = cur->next_;
          }
          else
          {
            head_ = cur->next_;
          }
          cur = cur->next_;
          delete temp;
          ++removed;
        }
        else
        {
          prev = cur;
          cur = cur->next_;
        }
      }
      return removed;
    }
    template< class UnaryPredicate >
    size_t remove_if(UnaryPredicate p)
    {
      size_t removed = 0;
      Node* prev = nullptr;
      Node* cur = head_;
      while (cur)
      {
        if (p(cur->value_))
        {
          Node* temp = cur;
          if (prev)
          {
            prev->next_ = cur->next_;
          }
          else
          {
            head_ = cur->next_;
          }
          cur = cur->next_;
          delete temp;
          ++removed;
        }
        else
        {
          prev = cur;
          cur = cur->next_;
        }
      }
      return removed;
    }

    iterator insert_after(const_iterator pos, const T& value)
    {
      if (pos == cend())
      {
        throw std::out_of_range("Iterator is out of range");
      }
      Node* new_node(value);
      new_node->next_ = pos.node_->next_;
      pos.node_->next_ = new_node;
      return ++pos;
    }
    iterator insert_after(const_iterator pos, size_t count, const T& value)
    {
      if (pos == cend())
      {
        throw std::out_of_range("Iterator is out of range");
      }
      if (!count)
      {
        return pos;
      }
      Node* next = pos.node_->next_;
      Node* new_range(value);
      iterator new_end(new_range);

      try
      {
        for (size_t i = 0; i < count - 1; ++i)
        {
          Node* new_node(value);
          new_end.node_->next_ = new_node;
          ++new_end;
        }
        pos.node_->next_ = new_range;
        new_end.node_->next_ = next;
      }
      catch (const std::bad_alloc&)
      {
        freeNodes(new_range);
        throw;
      }

      return new_end;
    }
    iterator insert_after(const_iterator pos, T&& value)
    {
      if (pos == cend())
      {
        throw std::out_of_range("Iterator is out of range");
      }
      Node* new_node(value);
      new_node->next_ = pos.node_->next_;
      pos.node_->next_ = new_node;

      return ++pos;
    }
    iterator insert_after(const_iterator pos, const_iterator first, const_iterator last)
    {
      if (pos == cend())
      {
        throw std::out_of_range("Iterator is out of range");
      }
      if (begin == end)
      {
        return pos;
      }
      Node* next = pos.node_->next_;
      Node* new_range(*first.node_);
      iterator new_end(new_range);
      ++first;
      try
      {
        for (; first != last; ++first)
        {
          Node* new_node(*first.node_);
          new_end.node_->next_ = new_node;
          ++new_end;
        }
        pos.node_->next_ = new_range;
        new_end.node_->next_ = next;
      }
      catch (const std::bad_alloc&)
      {
        freeNodes(new_range);
        throw;
      }

      return new_end;
    }

    iterator erase_after(const_iterator pos)
    {
      if (pos == cend())
      {
        throw std::out_of_range("Iterator is out of range");
      }
      if (pos.node_->next_)
      {
        iterator ret(pos.node_->next_->next_);
        delete pos.node_->next_;
        pos.node_->next_ = ret.node_;
        return ret;
      }
      else
        return end();
    }
    void splice_after(const_iterator pos, ForwardList& other)
    {
      if (pos == cend())
      {
        throw std::out_of_range("Iterator is out of range");
      }
      if (!other.head_)
      {
        return;
      }

      Node* temp = pos.node_->next_;
      pos.node_->next_ = other.head_;
      while (other.head_->next_)
      {
        other.head_ = other.head_->next_;
      }
      other.head_->next_ = temp;
      other.head_ = nullptr;
    }
    void reverse()
    {
      if (!head_)
      {
        return;
      }
      Node* new_head = head_;
      Node* tail = head_->next_;
      new_head->next_ = nullptr;
      while (tail)
      {
        Node* temp = tail->next_;
        tail->next_ = new_head;
        new_head = tail;
        tail = temp;
      }
      head_ = new_head;
    }

    void merge(ForwardList& other)
    {
      if (!other.head_)
      {
        return;
      }
      if (!head_)
      {
        head_ = other.head_;
        other.head_ = nullptr;
      }
      Node* res = nullptr;
      Node* list1_cur = head_;
      Node* list2_cur = other.head_;
      if (list1_cur->value_ < list2_cur->value_)
      {
        res = list2_cur;
        list2_cur = list2_cur->next_;
      }
      else
      {
        res = list1_cur;
        list1_cur = list1_cur->next_;
      }

      while (list1_cur || list2_cur)
      {
        if (list1_cur && list2_cur)
        {
          if (list1_cur->value_ < list2_cur->value_)
          {
            res->next_ = list2_cur;
            list2_cur = list2_cur->next_;
          }
          else
          {
            res->next_ = list1_cur;
            list1_cur = list1_cur->next_;
          }
        }
        else if (list1_cur)
        {
          res->next_ = list1_cur;
          list1_cur = nullptr;
        }
        else
        {
          res->next_ = list2_cur;
          list2_cur = nullptr;
        }
      }
      head_ = res;
      other.head_ = nullptr;
    }
    size_t unique()
    {
      if (!head_)
      {
        return 0;
      }
      size_t removed = 0;
      Node* cur = head_;
      while (cur->next_)
      {
        if (cur->value_ == cur->next_->value_)
        {
          Node* temp = cur->next_;
          cur->next_ = cur->next_->next_;
          delete temp;
          ++removed;
        }
        cur = cur->next_;
      }
      return removed;
    }
  private:
    Node* head_;
  };
}
#endif
