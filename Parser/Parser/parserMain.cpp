#include "CJsonMyStructParcer.hpp"
#include <vector>
#include <map>

void main()
{
   using namespace std;
   CJsonIntParcer objInt;
   objInt.set(22);
   int copyInt;
   objInt.get(copyInt);
   string serialesedString = objInt.serialize();

   MyStruct objStruct, objStruct2;
   objStruct.mInteger = 3;
   objStruct.mString="testStruct";
   objStruct.mBool = true;

   CJsonMyStructParcer structParcer, structParcer2;
   structParcer.set(objStruct);
   string serialesedMyStruc = structParcer.serialize();
   //structParcer2.deserialize( "\"MyStruct\": {\"Int\": 3, \"String\": \"testString\"}}" );
   structParcer2.deserialize( serialesedMyStruc.c_str() );

   structParcer2.get(objStruct2);

   MyStructWraper wraper, wraper2;
   wraper.mString = "Что - то";
   wraper.mstruct = objStruct;

   CJsonMyStructWraperParcer wraperParcer, wraperParcer2;
   wraperParcer.set(wraper);
   string serialesedwraperParcer = wraperParcer.serialize();
   
   wraperParcer2.deserialize(serialesedwraperParcer);
   wraperParcer2.get(wraper2);


   MyStructArrayWraper myStructArrayWraper;
   myStructArrayWraper.mString = "12345";
   myStructArrayWraper.mstructVec.push_back(objStruct);

   std::vector<MyStruct> structVector, structVector2;
   structVector.push_back(objStruct);
   structVector.push_back(objStruct);
   structVector.push_back(objStruct);



   CJsonMyStructArrayParcer myStructArrayParcer, myStructArrayParcer2;
   myStructArrayParcer.set(structVector);

   string serialesedwraperArrayParcer = myStructArrayParcer.serialize();
   myStructArrayParcer2.deserialize(serialesedwraperArrayParcer);
   myStructArrayParcer2.get(structVector2);
   int a = 0;


   return;


}