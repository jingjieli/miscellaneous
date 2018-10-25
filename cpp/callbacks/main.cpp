#include <string>
#include <iostream>

using namespace std;

// 1. function pointers
string encryptMsgByInc(string msg)
{
  for (auto &c : msg)
  {
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
      c++;
  }
  return msg;
}

string encryptMsgByDec(string msg)
{
  for (auto &c : msg)
  {
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
      c--;
  }
  return msg;
}

string buildMsg(string orig_msg, string (*encryptFuncPtr)(string))
{
  string new_msg = encryptFuncPtr(orig_msg);
  new_msg = "[HEADER]" + new_msg + "[FOOTER]";
  return new_msg;
}

// 2. functor
class Encryptor
{
public:
  Encryptor() : m_incremental_(false), m_count_(1) {}

  Encryptor(bool is_incremental, int cnt) : m_incremental_(is_incremental), m_count_(cnt){};

  string operator()(string msg)
  {
    for (auto &c : msg)
    {
      if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
      {
        if (m_incremental_)
        {
          c += m_count_;
        }
        else
        {
          c -= m_count_;
        }
      }
    }

    return msg;
  }

private:
  bool m_incremental_;
  int m_count_;
};

string buildMsg(string orig_msg, Encryptor encryptor)
{
  string new_msg = encryptor(orig_msg);
  new_msg = "[HEADER]" + new_msg + "[FOOTER]";
  return new_msg;
}

int main(int argc, char **argv)
{
  cout << buildMsg("old msg", &encryptMsgByInc) << endl;

  cout << buildMsg("old msg", &encryptMsgByDec) << endl;

  cout << buildMsg("old msg", Encryptor(true, 2)) << endl;

  cout << buildMsg("old msg", Encryptor(false, 2)) << endl;

  return 0;
}