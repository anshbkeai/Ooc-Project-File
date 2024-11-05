# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(TestProject VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify the source files
set(SOURCES

    aes_file.h
    aes.h
    Token_Service.h
    Timer.h
    Time_Convert.h
    Final_Service.h
    Token.h
    User.h
    Logger.h
    Vector.h
    Console.cpp

)
if(NOT WIN32)
    find_package(Threads REQUIRED)
    target_link_libraries(TestProject PRIVATE Threads::Threads)
endif()

# Add the executable (target) from the source files
add_executable(TestProject ${SOURCES})

# Set OpenSSL paths manually
set(OPENSSL_ROOT_DIR "C:/msys64/ucrt64")
set(OPENSSL_INCLUDE_DIR "C:/msys64/ucrt64/include/openssl")
set(OPENSSL_CRYPTO_LIBRARY "C:/msys64/ucrt64/lib/libcrypto.a")  # Adjust this to .dll if necessary
set(OPENSSL_SSL_LIBRARY "C:/msys64/ucrt64/lib/libssl.a")  # Add SSL library

# Find OpenSSL package
find_package(OpenSSL REQUIRED)

# Link the OpenSSL libraries
target_link_libraries(TestProject PRIVATE OpenSSL::Crypto OpenSSL::SSL)

# If you're not on Windows, you may want to link pthreads
if(NOT WIN32)
    find_package(Threads REQUIRED)
    target_link_libraries(TestProject PRIVATE Threads::Threads)
endif()
