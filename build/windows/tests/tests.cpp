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

      instance[3] = "three";
      instance[6] = "six";
      instance[1] = "one";

      Assert::AreEqual(instance[1], "one", L"message", LINE_INFO());
      Assert::AreEqual(instance[3], "three", L"message", LINE_INFO());
      Assert::AreEqual(instance[6], "six", L"message", LINE_INFO());
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

    TEST_METHOD(List_Test_02_PushShift) {
      my::list<my::string> instance;

      instance.push("one");
      instance.push("two");
      instance.push("three");

      Assert::AreEqual(instance.size(), (size_t)3, L"Expected 3 items in the list", LINE_INFO());

      Assert::AreEqual(instance.shift()->second, "one", L"Invalid value", LINE_INFO());
      Assert::AreEqual(instance.shift()->second, "two", L"Invalid value", LINE_INFO());
      Assert::AreEqual(instance.shift()->second, "three", L"Invalid value", LINE_INFO());
    }

    TEST_METHOD(List_Test_03_Invalid_Insert) {
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