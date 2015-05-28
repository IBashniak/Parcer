#include <string>
#include <vector>
struct MyStruct
{
   int mInteger;
   bool mBool;
   std::string mString;

   MyStruct& operator= (const MyStruct& other)
   {
      if (this != &other)
      {
         mInteger = other.mInteger;
         mBool = other.mBool;
         mString = other.mString;
      }
      return *this;
   }
};


struct MyStructWraper
{
   MyStructWraper& operator = (const MyStructWraper& other)
   {
      if (this != &other)
      {
         mstruct = other.mstruct;
         mString = other.mString;
      }
      return *this;
   }

   MyStruct mstruct;
   std::string mString;
};


struct MyStructArrayWraper
{
	MyStructArrayWraper& operator = (const MyStructArrayWraper& other)
	{
		if (this != &other)
		{
			mstructVec = other.mstructVec;
			mString = other.mString;
		}
		return *this;
	}

	std::vector< MyStruct > mstructVec;
	std::string mString;
};
