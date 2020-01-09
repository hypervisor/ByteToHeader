# ByteToHeader

Simple C tool to generate a C header file with an array of bytes of given file. Useful for storing resources in memory without relying on a file.

Usage:
```
./bth my_file my_file.h my_file_array(optional)
```
Output:
```c
#pragma once

// This file was auto-generated.

unsigned char my_file_array[0xSIZE] = {
  // ...
};
