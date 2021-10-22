#include "rea.h"
#include "gtest/gtest.h"

// The fixture for testing class Foo.

class FooTest : public ::testing::Test

{

protected:

FooTest()

{

// You can do set-up work for each test here.

}

virtual ~FooTest()

{

// You can do clean-up work that doesn't throw exceptions here.

}

virtual void SetUp()

{

// Code here will be called immediately after the constructor (right

// before each test).

}

virtual void TearDown()

{
    std::cout << "ff" << std::endl;
    rea2::pipeline::instance()->remove("gTestCase0");
    rea2::pipeline::instance()->remove("gTestCase1");
// Code here will be called immediately after each test (right

// before the destructor).

}

// Objects declared here can be used by all tests in the test case for Foo.

};



// Tests that the Foo::Bar() method does Abc.

TEST_F(FooTest, ZeroEqual)

{
    rea2::pipeline::instance()->add<double>([](rea2::stream<double>* aInput){
        EXPECT_EQ(aInput->data(), 5);
        aInput->setData(5)->out();
    }, rea2::Json("name", "gTestCase0"))
    ->nextF<double>([](rea2::stream<double>* aInput){
        EXPECT_EQ(aInput->data(), 8);
        std::cout << "lala" << std::endl;
    }, "", rea2::Json("name", "gTestCase1"));
    rea2::pipeline::instance()->run<double>("gTestCase0", 4);

   // EXPECT_EQ(0,5);
}



// Tests that Foo HiHiHi.

TEST_F(FooTest, OneEqual)

{

EXPECT_EQ(1,1);

}

static rea2::regPip<double> test_gtest([](rea2::stream<double>* aInput){
    testing::GTEST_FLAG(output) = "xml:report.xml";
    testing::InitGoogleTest();
    RUN_ALL_TESTS();
    rea2::pipeline::instance()->call<QJsonObject>("sendReport", rea2::Json("type", "test",
                                                                         "files", rea2::JArray("report.xml")));
}, rea2::Json("name", "gTest", "external", "qml"));
