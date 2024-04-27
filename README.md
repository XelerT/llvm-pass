# Sber Syntax

### About

This is second task at Sber Compilers additional semester class. Flex is used to get stream of tokens. My program visualizes syntax analyzation.


### Requirements

- Cmake version  3.13.4
- Flex


### Install & Build
        $ git clone https://github.com/XelerT/Sber-LLVM.git
        $ cd Sber-LLVM/
        $ cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build
### Run
        $ cd build/
        $ ./stasyan input_file.txt

### Code example


        Size_t size = 1;

        Int64_t start_position = 10;
        Int64_t mid_position   = 10;
        Int64_t end_position   = 10;

        True  bool = true;
        False bool = false;

        iF 10 tHeN print 42;

        String string = "String with string";
        String newline = "Devided by \n newline.\n";

        -- one line comment

        /* MY string with s1mb0|s\n /*/**/aaa/*aaa*/*/ */
