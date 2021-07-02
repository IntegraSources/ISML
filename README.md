# ISML

IntegraSources Messaging Library

## Getting Started

### Prerequisites

1. C++ compiler supporting C++2a/C++20
2. Conan package manager

### Build

```shell
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_CXX_FLAGS="-std=c++2a -Wall -pedantic -Wextra -Weffc++" \
      ..
```

## Authors

- Oleg E. Vorobiov <o.vorobiov@integrasources.com>
