#include "mylo.hpp"
#include "test.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

__TEST_NAMESPACE_BEGIN

TEST_CLASS_CATEGORY(Vector, "Vector")

  TEST_METHOD(Vector_Index) {
    my::vector<my::string> instance;

    instance[3] = "three";
    instance[6] = "six";
    instance[1] = "one";

    Assert::AreEqual(instance[1], "one", L"message", LINE_INFO());
    Assert::AreEqual(instance[3], "three", L"message", LINE_INFO());
    Assert::AreEqual(instance[6], "six", L"message", LINE_INFO());
  }

  TEST_CLASS_END_CATEGORY

  TEST_CLASS_CATEGORY(List, "List")

  TEST_METHOD(List_PushPop) {
    my::list<my::string> instance;

    instance.push("one");
    instance.push("two");
    instance.push("three");

    Assert::AreEqual(instance.size(), (size_t)3, L"Expected 3 items in the list", LINE_INFO());

    Assert::AreEqual(instance.pop()->second, "three", L"Invalid value", LINE_INFO());
    Assert::AreEqual(instance.pop()->second, "two", L"Invalid value", LINE_INFO());
    Assert::AreEqual(instance.pop()->second, "one", L"Invalid value", LINE_INFO());
  }

  TEST_METHOD(List_PushShift) {
    my::list<my::string> instance;

    instance.push("one");
    instance.push("two");
    instance.push("three");

    Assert::AreEqual(instance.size(), (size_t)3, L"Expected 3 items in the list", LINE_INFO());

    Assert::AreEqual(instance.shift()->second, "one", L"Invalid value", LINE_INFO());
    Assert::AreEqual(instance.shift()->second, "two", L"Invalid value", LINE_INFO());
    Assert::AreEqual(instance.shift()->second, "three", L"Invalid value", LINE_INFO());
  }

  TEST_METHOD(List_Invalid_Insert) {
    wchar_t message[200];
    try
    {

    }
    catch (std::exception ex)
    {

    }
    catch (...)
    {
      _swprintf(message, L"Incorrect exception for %hs", "test");
      Assert::Fail(message, LINE_INFO());
    }
  }

  TEST_CLASS_END_CATEGORY

__TEST_NAMESPACE_END