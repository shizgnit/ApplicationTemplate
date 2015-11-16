//#include "stdafx.h"
#include "CppUnitTest.h"

#include "mylo.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace tests
{		
	TEST_CLASS(vector) { 
  public:
		
		TEST_METHOD(Vector_Test_01_Index)	{
      my::vector<my::string> instance;

      instance[0] = "value";

      Assert::AreEqual(instance[0], "value", L"message", LINE_INFO());
		}

	};

  TEST_CLASS(list) {
  public:

    TEST_METHOD(List_Test_01_PushPop) {
      my::list<my::string> instance;

      instance.push("one");
      instance.push("two");
      instance.push("three");

      Assert::AreEqual(instance.size(), (size_t)3, L"Expected 3 items in the list", LINE_INFO());

      Assert::AreEqual(instance.pop()->second, "three", L"Invalid value", LINE_INFO());
      Assert::AreEqual(instance.pop()->second, "two", L"Invalid value", LINE_INFO());
      Assert::AreEqual(instance.pop()->second, "one", L"Invalid value", LINE_INFO());
    }

    TEST_METHOD(List_Test_02_Invalid_Insert) {
      wchar_t message[200];
      try
      {
      }
      catch (std::exception ex)
      {
      
      }
      catch (...)
      {
        _swprintf(message, L"Incorrect exception for %s", "test");
        Assert::Fail(message, LINE_INFO());
      }
    }
  };

}