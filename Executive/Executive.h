#include <iostream>
#ifndef EXECUTIVE_H
#define EXECUTIVE_H
using namespace std;

template <char ch = '='>
class title
{
public:
	typedef std::string str;
	std::string operator()(const str &msg)
	{
		str underline(msg.length() + 2, ch);
		return str("\n  ") + msg + str("\n ") + underline + str("\n");
	}
};
#endif
