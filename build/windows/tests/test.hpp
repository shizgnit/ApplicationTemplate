#pragma once

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



