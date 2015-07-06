
struct SourceFixture{
    ocls::CLSSource* src;
    ocls::Framework* framework;

    SourceFixture(){
        framework = new ocls::Framework();
        src = framework->getSourceLoader()->loadFromString(""
                "float func(void){\n"
                "\t\treturn pos.x;\n"
                "}");
    }
    ~SourceFixture(){
        delete framework;
    }
};

BOOST_FIXTURE_TEST_SUITE( source_loading_tests, SourceFixture )

BOOST_AUTO_TEST_CASE (getFunction_test){
    BOOST_REQUIRE(src->getFunction("func") != NULL);
    BOOST_CHECK_EQUAL(src->getFunction("func")->name, "func");
    BOOST_CHECK_EQUAL(src->getFunction("func")->signature, "float func(void)");
    BOOST_CHECK_EQUAL(src->getFunction("func")->source, src);
}

BOOST_AUTO_TEST_SUITE_END()