/**
  *  This file is part of the OpenCL-ConsLaws framework
  *  Copyright (C) 2014, 2015 Jens Kristoffer Reitan Markussen
  *
  * This program is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
  *
  */






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
