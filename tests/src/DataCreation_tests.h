
struct DataFixture{
    ocls::CLSSource* src;
    ocls::Framework* framework;

    DataFixture(){
        framework = new ocls::Framework();
        src = framework->getSourceLoader()->loadFromString(""
                "float func_1d(void){\n"
                "\t\treturn pos.x;\n"
                "}\n"
                "float func_2d(void){\n"
                "\t\treturn pos.x+pos.y;\n"
                "}\n"
                "float func_3d(void){\n"
                "\t\treturn pos.x+pos.y*pos.z;\n"
                "}");
    }
    ~DataFixture(){
        delete framework;
    }
};

BOOST_FIXTURE_TEST_SUITE( data_creation_tests, DataFixture )

BOOST_AUTO_TEST_CASE (createDataWithSource1d_test){
    // Create domain
    ocls::Domain domain(1);
    domain.setBounds(-1.0f,1.0f);
    domain.setMesh(5);
    domain.setGhostCells(2);
    ocls::Data* Q =
         framework->createData(domain, "Q", src->getFunction("func_1d"));

    // Check ghost cells
    BOOST_CHECK(Q->at(0) <= -1.0f);
    BOOST_CHECK(Q->at(1) <= -1.0f);
    BOOST_CHECK(Q->at(8) >= 1.0f);
    BOOST_CHECK(Q->at(7) >= 1.0f);

    // Check center node
    BOOST_CHECK(Q->at(4) == 0.0f);
}

BOOST_AUTO_TEST_CASE (createDataWithSource2d_test){
    // Create domain
    ocls::Domain domain(2);
    domain.setBounds(-1.0f,1.0f);
    domain.setMesh(5,5);
    domain.setGhostCells(2);
    ocls::Data* Q =
            framework->createData(domain, "Q", src->getFunction("func_2d"));

    // Check ghost cells
    // TODO

    // Check center node
    BOOST_CHECK(Q->at(4,4) == 0.0f);
}

BOOST_AUTO_TEST_CASE (createDataWithSource3d_test){
    // Create domain
    ocls::Domain domain(3);
    domain.setBounds(-1.0f,1.0f);
    domain.setMesh(5,5,5);
    domain.setGhostCells(2);
    ocls::Data* Q =
            framework->createData(domain, "Q", src->getFunction("func_3d"));

    // Check ghost cells
    // TODO

    // Check center node
    BOOST_CHECK(Q->at(4,4,4) == 0.0f);
}

BOOST_AUTO_TEST_CASE (createDataEmpty_test){
    // Create domain
    ocls::Domain domain(1);
    domain.setMesh(5);

    ocls::Data* Q = framework->createData(domain, "Q");
    for(int i = 0; i < 5; i++) {
        BOOST_CHECK(Q->at(i) == 0.0f);
    }
}

BOOST_AUTO_TEST_SUITE_END()