#include <iostream>
#include <string>
#include <cstdlib>

#include "Framework.h"

#if !defined( WIN32 )
#define BOOST_TEST_DYN_LINK
#endif

#define BOOST_TEST_MODULE framework_tests
#include <boost/test/included/unit_test.hpp>

// Initialize the framework for all tests
// TODO: This didnt work, duno why ;<
//static ocls::Framework framework;

// Include tests!
#include "DataCreation_tests.h"
#include "SourceLoading_tests.h"

