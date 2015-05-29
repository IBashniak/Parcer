#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>
struct CTypeName
{
   std::string type;
   std::string name;
};

class CFileReader
{
public:
   CFileReader(){};
   CFileReader(const std::string& path)
   {
      setPathToFile(path);
   };

   typedef std::map < std::string, std::string > NameTypeMap;
   typedef std::map < std::string, NameTypeMap > ClassMemberMap;
   bool setPathToFile(const std::string& path);
   ClassMemberMap getClassMemberMap();

private:
   // full path including file
   std::string mPathToFile;
   std::vector< std::string > ReadFile();
   //CTypeName splitString(const std::string& inputString);
   // only string with definition of member
   bool isImportantString(const std::string& str);
   ClassMemberMap processData(const std::vector< std::string >& memberString);

};