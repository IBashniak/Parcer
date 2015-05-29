#include "CFileReader.hpp"

void main()
{
   std::string path ("D:/MyGit/Parcer/Parser/Parser/MyStruct.hp");
   CFileReader fileReader(path);
   fileReader.getClassMemberMap();
}