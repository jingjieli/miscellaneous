#include <iostream>
#include <vector>

using namespace std;

// Reference: https://thispointer.com/c11-move-contsructor-rvalue-references/

class Container
{
  public:
    Container()
    {
      m_data_ = new int[20];

      cout << "Constructor: allocate 20 int" << endl;
    }

    Container(const Container& container)
    {
      m_data_ = new int[20];

      for (int i = 0; i < 20; ++i)
      {
        m_data_[i] = container.m_data_[i];
      }

      cout << "Copy Constructor: allocate 20 int" << endl;
    }

    Container& operator=(const Container& container)
    {
      if (this != &container)
      {
        m_data_ = new int[20];

        for (int i = 0; i < 20; ++i)
        {
          m_data_[i] = container.m_data_[i];
        }

        cout << "Copy Assignment operator: allocate 20 int" << endl;
      }

      return *this;
    }

    Container(Container&& container)
    {
      m_data_ = container.m_data_;

      container.m_data_ = nullptr;

      cout << "Move Constructor ..." << endl;
    }

    Container& operator=(Container&& container)
    {
      if (this != &container)
      {
        m_data_ = container.m_data_;
        container.m_data_ = nullptr;
        cout << "Move Assignment operator ..." << endl;
      }

      return *this;
    }

    ~Container()
    {
      if (m_data_)
      {
        delete[] m_data_;
        m_data_ = nullptr;

        cout << "Destructor ..." << endl;
      }
    }

    static Container getContainer()
    {
      cout << "getContainer ..." << endl; 
      Container obj;
      return obj;
    }

  private:
    int* m_data_;
};

// Reference: https://msdn.microsoft.com/en-us/library/dd293665.aspx

class MemoryBlock
{
  public:
    explicit MemoryBlock(size_t length)
    : m_length_(length), m_data_(new int[m_length_])
    {
      cout << "Constructor: length = " << m_length_ << endl; 
    }

    ~MemoryBlock()
    {
      if (m_data_)
      {
        delete[] m_data_;
        m_data_ = nullptr;

        cout << "Destructor: m_length = " << m_length_ << endl;
      }
    }

    MemoryBlock(const MemoryBlock& mem_blk)
    : m_length_(mem_blk.m_length_), m_data_(new int[mem_blk.m_length_])
    {
      copy(mem_blk.m_data_, mem_blk.m_data_ + m_length_, m_data_);

      cout << "Copy Constructor, m_length = " << m_length_ << endl;
    }

    MemoryBlock& operator=(const MemoryBlock& mem_blk)
    {
      if (this != &mem_blk)
      {
        delete[] m_data_;

        m_length_ = mem_blk.m_length_;
        m_data_ = new int[mem_blk.m_length_];

        copy(mem_blk.m_data_, mem_blk.m_data_ + m_length_, m_data_);

        cout << "Copy Assignment operator: m_length = " << m_length_ << endl;
      }

      return *this;
    }

    MemoryBlock(MemoryBlock&& mem_blk) noexcept
    : m_length_(0), m_data_(nullptr)
    {
      m_data_ = mem_blk.m_data_;
      m_length_ = mem_blk.m_length_;

      mem_blk.m_data_ = nullptr;
      mem_blk.m_length_ = 0;

      cout << "Move Constructor: m_length = " << m_length_ << endl; 
    }


    MemoryBlock& operator=(MemoryBlock&& mem_blk)
    {
      if (this != &mem_blk)
      {
        delete[] m_data_;

        m_data_ = mem_blk.m_data_;
        m_length_ = mem_blk.m_length_;

        mem_blk.m_data_ = nullptr;
        mem_blk.m_length_ = 0;

        cout << "Move Assignment operator: m_length = " << m_length_ << endl; 
      }

      return *this;
    }

  private:
    size_t m_length_;
    int* m_data_;
};

int main1(int argc, char** argv)
{
  vector<Container> containers;

  containers.push_back(Container::getContainer());

  Container* obj = new Container();
  *obj = Container::getContainer();

  return 0;
}

int main(int argc, char** argv)
{
  vector<MemoryBlock> mem_blks;

  mem_blks.push_back(MemoryBlock(25));
  mem_blks.push_back(MemoryBlock(75));
  mem_blks.insert(mem_blks.begin() + 1, MemoryBlock(50));

  return 0;
}