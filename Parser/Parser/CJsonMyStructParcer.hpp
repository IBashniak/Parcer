#include "CJsonConcreetParcer.hpp"
#include "MyStruct.hpp"

class CJsonMyStructParcer : public CJsonConcreetParcer<MyStruct>
{
public:
   CJsonMyStructParcer ()
   {
      mIntValue.setName("Int");
      mStringValue.setName("String");
      mBoolValue.setName("IsX");
      setName("MyStruct");

      mInternalObject.push_back(&mIntValue);
      mInternalObject.push_back(&mStringValue);
      mInternalObject.push_back(&mBoolValue);
   }

   virtual void set(const MyStruct& value)
   {
      mValue = value;
      mIntValue.set(value.mInteger);
      mStringValue.set(value.mString);
      mBoolValue.set(value.mBool);
   }

   virtual void deserialize(const std::string& inputString)
   {
      CJsonConcreetParcer<MyStruct>::deserialize(inputString);
      mIntValue.get(mValue.mInteger);
      mStringValue.get(mValue.mString);
      mBoolValue.get(mValue.mBool);
   }

   CJsonIntParcer mIntValue;
   CJsonStringParcer mStringValue;
   CJsonBoolParcer mBoolValue;

};


class CJsonMyStructWraperParcer : public CJsonConcreetParcer<MyStructWraper>
{
public:

   CJsonMyStructWraperParcer()
   {
      mStructValue.setName("Struct");
      mStringValue.setName("String");
      setName("MyStructWraper");

      mInternalObject.push_back(&mStringValue);
      mInternalObject.push_back(&mStructValue);

   }
   virtual void set(const MyStructWraper& value)
   {
      mValue = value;
      mStringValue.set(value.mString);
      mStructValue.set(value.mstruct);
   }

   virtual void deserialize(const std::string& inputString)
   {
      CJsonConcreetParcer<MyStructWraper>::deserialize(inputString);
      mStringValue.get(mValue.mString);
      mStructValue.get(mValue.mstruct);
   }

   CJsonStringParcer mStringValue;
   CJsonMyStructParcer mStructValue;
};



class CJsonMyStructArrayParcer : public CJsonArrayParcer<MyStruct ,CJsonMyStructParcer>
{
public:
	CJsonMyStructArrayParcer ()
	{
		mArrayValue.setName("MyStructArray");
		setName("MyStructArray");
		mInternalObject.push_back(&mArrayValue);
	}

	CJsonArrayParcer<MyStruct ,CJsonMyStructParcer > mArrayValue;

};


