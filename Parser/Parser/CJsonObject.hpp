#include <string>
#include <map>
#include <algorithm>
#include <sstream>
#include <cctype>

enum ObectType { E_NULL, E_OBJECT, E_STRING, E_NUMBER, E_BOOL, E_ARRAY };


class CJsonObject
{
public:
   CJsonObject(){};
   CJsonObject(std::string valueString): mName(valueString) {};

   typedef std::string::const_iterator stringIterator;

   virtual std::string serialize () const = 0;

   virtual void deserialize (const std::string& inputString)  = 0;

   // Objects place in string
   struct ObjectCoord
   {
      ObjectCoord() : start(0), end(0){}
      int start;
      int end;
   };

   const std::string& getName() const {return mName;};

protected:
   std::string mName;
   
   template <typename Type>
   Type stringToValue(const std::string& inputString)
   {
      std::stringstream ss;
      Type result;
      ss << inputString;
      ss  >> result;
      return result;
   }
   typedef std::map<std::string, std::string> KeyValueMap;
   virtual KeyValueMap splitString(const std::string& inputString)
   {
      std::string value = inputString;
      int keyValueSpliter = 0;
      KeyValueMap keyValueMap;
      std::string::const_iterator curentString = inputString.begin();
      stringIterator start;
      stringIterator end;
      const int MINIMAL_KEYVALUE_STRING_LENGHT = 5; // "key":value example "i":1
      while ( inputString.end() - curentString > MINIMAL_KEYVALUE_STRING_LENGHT )
      {
         ObjectCoord keyCoord = getKey(curentString, inputString.end());
         ObjectCoord objectCoord = getValue(curentString + keyCoord.end, inputString.end() );
         objectCoord.start += keyCoord.end;
         objectCoord.end += keyCoord.end;

         start = curentString + keyCoord.start;
         end = curentString + keyCoord.end;
         std::string key(start, end);

         start = curentString + objectCoord.start;
         end = curentString + objectCoord.end;
         std::string value(start, ++end);

         if ( !key.empty())
         {
            keyValueMap.insert(std::make_pair (key, value ));
         }

         curentString += (objectCoord.end + 1);

      }
      
      return keyValueMap;
   }

   ObjectCoord getValue( stringIterator startString, const stringIterator& endString )
   {
      ObjectCoord value;
      const ObectType type = getType(startString, endString);
      switch (type)
      {
      case E_OBJECT:
      case E_ARRAY:
         value = extractObjectValue(startString, endString, type);
         break;
      case E_STRING:
         value = extractStringValue(startString, endString);
         break;
      case E_NUMBER:
         value = extractNumberValue(startString, endString);
         break;
      case E_BOOL:
         value = extractBoolValue(startString, endString);
         break;
      }
      return value;
   }

private:

   ObectType getType( stringIterator startString, const stringIterator& endString )
   {
      ObectType objectType = E_NULL;
      skipWhitespace( startString, endString );

      switch ( *startString )
      {
         case '{':
            objectType = E_OBJECT;
            break;

         case '"':
            objectType = E_STRING;
            break;

         case '[':
            objectType = E_ARRAY;
            break;

         case 't':
         case 'T':
         case 'f':
         case 'F':
            objectType = E_BOOL;
            break;
      }

      if ( objectType == E_NULL && isdigit( *startString ) != 0 )
      {
         objectType = E_NUMBER;
      }

      return objectType;
   }

   ObjectCoord getKey( stringIterator startString, const stringIterator& endString )
   {
      stringIterator startKey = startString;
      ObjectCoord keyPlace;

      while ( startKey != endString && *startKey != '"' )
      {
         startKey++;
      }

      keyPlace.start = startKey - startString;

      stringIterator endKey = startKey;
      while ( endKey != endString && *endKey != ':' )
      {
         endKey++;
      }

      keyPlace.end = endKey - startString;
      return keyPlace;
   }

   ObjectCoord extractStringValue( const stringIterator& startString, const stringIterator& endString )
   {
      ObjectCoord objectPlace;
      stringIterator startValue = startString;
      while ( startValue != endString && *startValue != '"' )
      {
         ++startValue;
      }

      objectPlace.start = int (startValue - startString);

      ++startValue;

      while ( startValue != endString && *startValue !='"')
      {
         ++startValue;
      }
      objectPlace.end = int (startValue - startString);
      return objectPlace;
   }

   ObjectCoord extractBoolValue( stringIterator startString, const stringIterator& endString )
   {
      ObjectCoord objectPlace;
      stringIterator startObj = startString;
      skipWhitespace( startObj, endString );

      objectPlace.start = int(startObj - startString);
      if (*startObj == 't' || *startObj == 'T')
      {
         objectPlace.end = objectPlace.start + 5;
      }
      else
      {
         objectPlace.end = objectPlace.start + 6;
      }
      return objectPlace;
   }

   ObjectCoord extractNumberValue( stringIterator startString, const stringIterator& endString )
   {
      ObjectCoord objectPlace;
      stringIterator startObj = startString;

      skipWhitespace( startObj, endString );

      objectPlace.start = int(startObj - startString);
      
      while ( isdigit(*startObj) || *startObj =='.' )
      {
         startObj++;
      }

      objectPlace.end = int(startObj - startString);

      return objectPlace;
   }

   ObjectCoord extractObjectValue( stringIterator startString, const stringIterator& endString, ObectType type )
   {
      int countOpenBraces;
      stringIterator startObj = startString;
      ObjectCoord objectPlace;
      char openObject	= '{';
      char closeObject	= '}';
      if (type == E_ARRAY)
      {
         openObject	= '[';
         closeObject	= ']';
      }

      countOpenBraces = 0;
      while ( startObj != endString && ( countOpenBraces != 0 || objectPlace.start == 0))
      {
         if (*startObj ==  openObject)
         {
            ++countOpenBraces;
            if (countOpenBraces == 1)
            {
               objectPlace.start = int (startObj - startString);
            }
         } 
         else if (*startObj ==  closeObject)
         {
            --countOpenBraces;
         }
         ++startObj;
      }
      objectPlace.end = int (startObj - 1 - startString);

      return objectPlace;
   }

   void skipWhitespace( stringIterator& startString, const stringIterator& endString )
   {
      while ( startString != endString && ( *startString == ':' || *startString == ' ' ) )
      {
         startString++;
      }
   }
};