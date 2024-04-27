# Sber LLVM Pass

### About

This is the third task at Sber Compilers additional semester class: Implementation of dead code elimination pass. Zero propagation and memory to register transformations are made before DCE pass.


### Requirements

- Cmake version  3.13.4
- LLVM

### Install & Build
        $ git clone https://github.com/XelerT/Sber-LLVM.git
        $ cd Sber-LLVM/
        $ cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build

### Run

You need to generate llvm IR file and then feed it to stasyan:

        $ cd build/
        $ ./stasyan input_file.ll

### Example of input file

        define dso_local i32 @sub(i32 noundef %0, i32 noundef %1) { 
                %3 = mul i32 %0, 0
                ret i32 %3
        }
