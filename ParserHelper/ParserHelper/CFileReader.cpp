#include "CFileReader.hpp"
#include <algorithm>
#define MAX_STRING_LEN 512
#include <iostream>

bool CFileReader::setPathToFile( const std::string& path )
{
   //To Do check if file exist
   mPathToFile = path;
   return true;
}

std::vector< std::string > CFileReader::ReadFile()
{
   std::vector< std::string > result;
   if (mPathToFile.empty())
   {
      return result;
   }

   std::ifstream ifs;

   ifs.open (mPathToFile.c_str(), std::ifstream::in);
   if (!ifs.is_open())
   {
      return result;
   }

   char buffer[MAX_STRING_LEN];

   while (!ifs.eof()) 
   {
      ifs.getline (buffer, MAX_STRING_LEN);
      std::string curentString(buffer);
      if (isImportantString(curentString))
      {
         result.push_back(curentString);
      }
   }

   ifs.close();
   return result;

}
struct CFunctor
{
   CFunctor(CFileReader::ClassMemberMap& classMemberMap): mClassMemberMap(classMemberMap){}

   CTypeName CFunctor::splitString( const std::string& inputString );
   void operator() (const std::string& curentString)
   {
      CTypeName typeName = splitString(curentString);
      if (typeName.type.compare("struct") == 0|| typeName.type.compare("class") == 0)
      {
         if (!mCurentTypeMap.empty() && !mCurentClass.empty())
         {
            mClassMemberMap.insert(std::make_pair(mCurentClass,mCurentTypeMap));
            mClassMemberMap[mCurentClass] = mCurentTypeMap;
            int s = mClassMemberMap.size();
            int a =0;
         }
         mCurentClass = typeName.name;
         mCurentTypeMap.clear();
         //mClassMemberMap[mCurentClass][typeName.type] = typeName.name;


      }
      else
      {
         mClassMemberMap[mCurentClass][typeName.type] = typeName.name;
         //mCurentTypeMap.insert(std::make_pair(typeName.type,typeName.name));
      }
   }

   std::string mCurentClass;
   CFileReader::NameTypeMap mCurentTypeMap;
   CFileReader::ClassMemberMap& mClassMemberMap;
};

struct CPrint
{
   void operator() (const  std::pair< std::string, CFileReader::NameTypeMap >& curentMapString)
   {
      CFileReader::NameTypeMap::const_iterator start = curentMapString.second.begin();
      CFileReader::NameTypeMap::const_iterator end = curentMapString.second.end();
      CFileReader::NameTypeMap::const_iterator curString = start;

      std::cout <<" _  " << curentMapString.first<< "_ \n";

      for (;curString != end; curString++)
      {
         std::cout << curString->first<< " + " << curString->second<< "!\n";

         //this->operator()(curString->second);
      }

   }
   //void operator() (const  std::pair< std::string, CFileReader::NameTypeMap >& curentMapString)
   //{
   //   CFileReader::NameTypeMap::const_iterator start = curentMapString.begin();
   //   CFileReader::NameTypeMap::const_iterator end = curentMapString.end();
   //   CFileReader::NameTypeMap::const_iterator curString = start;
   //   for (;curString != end; curString++)
   //   {
   //      std::cout << curString->first<< "  " << curString->second;
   //   }

   //}
};

CFileReader::ClassMemberMap CFileReader::getClassMemberMap()
{
   std::vector< std::string >stringVector  = ReadFile();
   CFileReader::ClassMemberMap classMemberMap;
   CFunctor functor (classMemberMap);
   std::for_each(stringVector.begin(), stringVector.end(), functor );
   CPrint print;
   int s = functor.mClassMemberMap.size();

   std::for_each(functor.mClassMemberMap.begin(), functor.mClassMemberMap.end(), print );

   return CFileReader::ClassMemberMap();
}

bool CFileReader::isImportantString( const std::string& str )
{
   bool isImportant = false;
   int r = str.find('{');
   int r3 = str.find('}');
   bool em =  str.empty();
   if ( r >= 0 || r3 >= 0 || str.empty() )
   {
      return isImportant;
   }
   isImportant = true;

   return isImportant;
   
}

CTypeName CFunctor::splitString( const std::string& inputString )
{
// remove space
   int start = inputString.find_first_not_of(" ");
   int end = inputString.find_last_not_of(" ");
   std::string::const_iterator pStart = inputString.begin() + start;
   std::string::const_iterator pEnd = inputString.begin() + end;
   if (isalpha(*pEnd))
   {
      ++pEnd;
   }
   int space = inputString.find_last_of(' ');
   std::string::const_iterator pSpace = inputString.begin() + space;

   CTypeName typeName;
   typeName.type = std::string(pStart, pSpace);
   typeName.name = std::string(pSpace + 1 , pEnd);

   return typeName;
}