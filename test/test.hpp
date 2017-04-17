#pragma once

#if defined _WIN32

  //#include "stdafx.h"
  #include "CppUnitTest.h"

#else
  
  // Fabricate the namespace just in case something cares
  namespace Microsoft {
    namespace VisualStudio {
      namespace CppUnitTestFramework {}
    }
  }

  #include <list>
  #include <iostream>

  namespace _vs_test_adapter {

    class method {
    public:
      virtual void run(void *) = 0;
    };

    static std::list<method *> methods;

    template<typename T, typename S> class method_spec : public method {
    public:
      method_spec() {
        methods.push_back(this);
      }

      void run(void* inst) {
        ((T*)inst)->caller((S*)inst);
      }
    };

    class tester {};

    class tester_factory {
    public:
      virtual tester *create() = 0;
    };

    static std::list<tester_factory *> testers;

    template<class T> class tester_spec : public tester_factory {
    public:
      tester_spec(std::string name) {
        m_name = name;
        testers.push_back(this);
      }
      tester *create() {
        methods.clear();
        return new T();
      }
      std::string m_name;
    };

  }

  #define TEST_CLASS(className) \
	  namespace className { \
		  class test_class; \
	  } \
	  static _vs_test_adapter::tester_spec<##className##::test_class> className##_test_class_instance("##className##"); \
	  class className::test_class : public _vs_test_adapter::tester

  #define TEST_METHOD(methodName) \
	  struct methodName##_signature {}; \
	  void caller(methodName##_signature *t) { \
		  this->methodName(); \
	  } \
	  _vs_test_adapter::method_spec<test_class, methodName##_signature> methodName##_instance; \
	  void methodName()

  #define BEGIN_TEST_METHOD_ATTRIBUTE(var)
  #define TEST_OWNER(var)
  #define TEST_PRIORITY(var)
  #define END_TEST_METHOD_ATTRIBUTE()

#endif


#define __TEST_NAMESPACE_BEGIN namespace test {
#define __TEST_NAMESPACE_END }

// http://www.ademiller.com/blogs/tech/2014/04/test-categories-for-visual-c/
// Note, VS2015 community does not support test case categories

#define TEST_CLASS_CATEGORY(className, category)                    \
        TEST_CLASS(className)                                       \
        {                                                           \
            BEGIN_TEST_CLASS_ATTRIBUTE()                            \
                TEST_CLASS_ATTRIBUTE(L"TestCategory", L#category)   \
            END_TEST_CLASS_ATTRIBUTE()

#define TEST_CLASS_END_CATEGORY };


#define TEST_METHOD_CATEGORY(methodName, category)                  \
        BEGIN_TEST_METHOD_ATTRIBUTE(methodName)                     \
            TEST_METHOD_ATTRIBUTE(L"TestCategory", L#category)      \
        END_TEST_METHOD_ATTRIBUTE()                                 \
        TEST_METHOD(methodName)



