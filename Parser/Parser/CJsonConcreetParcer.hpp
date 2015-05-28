#include <vector>
#include <iostream>
#include "CJsonObject.hpp"


template <typename T>
class CJsonConcreetParcer :public CJsonObject
{
public:
   CJsonConcreetParcer(){};
   CJsonConcreetParcer(const std::string& name) : CJsonObject(name){};
   
   virtual void set(const T& value)
   {
      mValue = value;
   }
   virtual void get (T& value) const
   {
      value = mValue;
   }
   void setName(const std::string& name)
   {
      mName = "\"" + name + "\"";
   }

   virtual std::string serialize () const
   {
      std::string result (mName +": {\n");
      if (mInternalObject[0] !=0 )
      {
         result += mInternalObject[0]->serialize();
      }
      for (unsigned int i = 1; i < mInternalObject.size(); ++i)
      {
         if (mInternalObject[i] != 0 )
         {
            result += ",\n" + mInternalObject[i]->serialize();
         }
      }
     result += "\n}";
     return result;
   }

   virtual void deserialize (const std::string& inputString)
   {
      KeyValueMap stringMap = splitString(inputString);
      if (getName() == stringMap.begin()->first)
      {
         stringMap = splitString(stringMap.begin()->second);
      }
      for (unsigned int i = 0; i < mInternalObject.size(); ++i)
      {
         CJsonObject* pCurrentInternalObcject = mInternalObject[i];

         if ( pCurrentInternalObcject != 0 )
         {
            std::string currentInternalObcjectName( pCurrentInternalObcject->getName() );
            KeyValueMap::iterator curentKeyValueMapElement = stringMap.begin();
            while (curentKeyValueMapElement != stringMap.end())
            {
               std::string curentKeyValueMapElementName (curentKeyValueMapElement->first);

               if (currentInternalObcjectName == curentKeyValueMapElementName)
               {
                  pCurrentInternalObcject->deserialize( curentKeyValueMapElement->second );
                  stringMap.erase( curentKeyValueMapElement );
                  curentKeyValueMapElement = stringMap.begin();
                  break;
               }
               else
               {
                  curentKeyValueMapElement++;
               }
            }
         }
      }
   }
   T mValue;
protected:
   std::vector<CJsonObject*> mInternalObject;
private:
   CJsonConcreetParcer (const CJsonConcreetParcer& other){}
   CJsonConcreetParcer& operator = (const CJsonConcreetParcer& other){};
};

class CJsonIntParcer : public CJsonConcreetParcer<int>
{
public:
   CJsonIntParcer() {mValue = 0;}
   CJsonIntParcer (const std::string& name) : CJsonConcreetParcer<int>(name) {}

   virtual std::string serialize () const 
   {
      std::ostringstream ss;
      ss << mValue;
      return  mName +": "+ ss.str() + " ";
   }
   virtual void deserialize (const std::string& inputString)
   {
      mValue = stringToValue<int>(inputString);
   }

private:
   CJsonIntParcer (const CJsonIntParcer& other){}
   CJsonIntParcer& operator = (const CJsonIntParcer& other){};
};

class CJsonBoolParcer : public CJsonConcreetParcer<bool>
{
public:
   CJsonBoolParcer() { mValue = 0;}
   CJsonBoolParcer (const std::string& name) : CJsonConcreetParcer<bool>(name) {}

   virtual std::string serialize () const 
   {
      std::ostringstream ss;
      ss << std::boolalpha << mValue;
      return  mName +": "+ ss.str() + " ";
   }

   virtual void deserialize (const std::string& inputString)
   {
      if ( inputString.compare(0, 4, "TRUE") || inputString.compare(0, 4, "true")  == 0)
      {
         mValue = true;
      }
      else
      {
         mValue = false;
      }
   }

private:
   CJsonBoolParcer (const CJsonBoolParcer& other){}
   CJsonBoolParcer& operator = (const CJsonBoolParcer& other){};
};

class CJsonStringParcer : public CJsonConcreetParcer<std::string>
{
public:
   CJsonStringParcer(){}
   CJsonStringParcer (const std::string& name) : CJsonConcreetParcer<std::string>(name) {}
   virtual std::string serialize () const 
   {
      return mName + ": \""+ mValue + "\"";
   }
   virtual void deserialize (const std::string& inputString)
   {
      mValue = inputString;
   }
private:
   CJsonStringParcer (const CJsonStringParcer& other){}
   CJsonStringParcer& operator = (const CJsonStringParcer& other){};
};

template <typename Tvalue,typename Tparcer >

class CJsonArrayParcer : public CJsonConcreetParcer<std::vector<Tvalue> >
{
public:
	CJsonArrayParcer(){}

	CJsonArrayParcer (const std::string& name) : CJsonConcreetParcer<std::vector<Tvalue> >(name) {}
   virtual std::map<std::string, std::string> splitString(const std::string& inputString)
   {
      std::string value = inputString;
      int keyValueSpliter = 0;
      std::map<std::string, std::string> keyValueMap;
      std::string::const_iterator curentString = inputString.begin();
      //skip key of current object (array)
      int keyEnd = inputString.find('[');
      int valueStart = keyEnd;
      char fakeValue ='0';

      while (inputString.end() - curentString > 2 )
      {

         valueStart = inputString.find('{', ++valueStart);
         if (valueStart > 0)
         {
            curentString = inputString.begin()+ valueStart;
         }

         ObjectCoord objectCoord = getValue(curentString , inputString.end() );
         stringIterator start = curentString;
         stringIterator end = curentString;
         std::string key(start, end);

         start = curentString + objectCoord.start;
         end = curentString + objectCoord.end;
         std::string value(start, ++end);

         if ( !value.empty())
         {
            keyValueMap.insert(std::make_pair (&(fakeValue), value ));
         }
         fakeValue++;
         curentString += (objectCoord.end + 1);
      }

      return keyValueMap;
   }

	virtual std::string serialize () const 
	{
		std::string result("[ ");
      Tparcer parcer;

		if ( mValue.size() > 0)
		{
         parcer.set( mValue[0] );
         std::string serializeResult = parcer.serialize ();
         result += removeObjectName(serializeResult);
		}
		for (int i = 0; i < mValue.size(); ++i)
		{
         parcer.set( mValue[i] );
         std::string serializeResult = parcer.serialize ();
			result += ", " + removeObjectName(serializeResult);
		}
		result += "]"; 
		return mName + ": \""+ result + "\"";
	}
	virtual void deserialize (const std::string& inputString)
	{
		typedef std::map<std::string, std::string> mymap;
		mymap stringMap = splitString(inputString);

		mValue.clear();
		mymap::iterator curentObject = stringMap.begin();
		mymap::iterator endObject = stringMap.end();
		while (curentObject != endObject)
		{
			Tparcer parcer;
			parcer.deserialize(curentObject->second);
         Tvalue curentValue;
         parcer.get(curentValue);
			mValue.push_back(curentValue);
			curentObject++;
		}
	}
private:
   std::string& removeObjectName( std::string& serializeResult) const
   {
      int keyEnd = serializeResult.find(':');
      if (keyEnd > 0)
      {
         serializeResult.erase(0, keyEnd + 1);
      }
      return serializeResult;
   }

   CJsonArrayParcer (const CJsonArrayParcer& other){}
   CJsonArrayParcer& operator = (const CJsonArrayParcer& other){};
};

