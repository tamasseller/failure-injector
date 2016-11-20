all: test

TEST_BINARY_NAME = FailureInjectorTest

TEST_SOURCES += FailureSourceTest.cpp
TEST_SOURCES += FailureInjectorTest.cpp
TEST_SOURCES += RegistryTest.cpp

TEST_SOURCES += ../FailureInjectorPlugin.cpp
TEST_SOURCES += ../FailureInjectorShell.cpp
TEST_SOURCES += ../SimpleStringHelper.cpp
TEST_SOURCES += ../FailureSource.cpp

TEST_INCLUDE_DIRS += .
TEST_INCLUDE_DIRS += ..

TEST_FLAGS = -fno-exceptions -m32 -std=c++11

CPPUTEST_FLAGS = -c
	
include ../ultimate-makefile/Makefile.test
