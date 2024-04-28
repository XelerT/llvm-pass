# Sber LLVM Pass

### About

This is the third task at Sber Compilers additional semester class: Implementation of dead code elimination pass. Zero propagation and memory to register transformations are made before DCE pass.


### Requirements

- Cmake version  3.13.4
- LLVM

### Install & Build
        $ git clone https://github.com/XelerT/llvm-pass.git
        $ cd llvm-pass/
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

### Example of work

Firstly, you need to create llvm IR file (from ***build/***):

        $ clang -S -emit-llvm -O0 ../test/data/sources/ex3.cpp
        $ ./stasyan  ../test/data/ex3.ll

Output of the program:

        ; Function Attrs: mustprogress noinline nounwind optnone sspstrong uwtable
        define dso_local noundef i32 @_Z5code3i(i32 noundef %0) #0 {
                ret i32 %0
        }

Three passes worked on the IR (see code after every transformation): 
1) Zero propagation from [apselon](https://github.com/apselon)

        define dso_local noundef i32 @_Z5code3i(i32 noundef %0) #0 {
                %2 = alloca i32, align 4
                %3 = alloca i32, align 4
                %4 = alloca i32, align 4
                %5 = alloca i32, align 4
                %6 = alloca i32, align 4
                store i32 %0, ptr %2, align 4
                store i32 5, ptr %3, align 4
                store i32 10, ptr %4, align 4
                store i32 15, ptr %5, align 4
                store i32 20, ptr %6, align 4
                %7 = load i32, ptr %2, align 4
                %8 = load i32, ptr %4, align 4
                %9 = add nsw i32 %7, %8
                %10 = load i32, ptr %3, align 4
                %11 = add nsw i32 %9, %10
                %12 = load i32, ptr %5, align 4
                %13 = add nsw i32 %11, %12
                %14 = load i32, ptr %6, align 4
                %15 = add nsw i32 %13, %14
                store i32 %15, ptr %6, align 4
                %16 = load i32, ptr %2, align 4
                ret i32 %16
        }

2) Memory to registers from llvm:

        define dso_local noundef i32 @_Z5code3i(i32 noundef %0) #0 {
                %2 = add nsw i32 %0, 10
                %3 = add nsw i32 %2, 5
                %4 = add nsw i32 %3, 15
                %5 = add nsw i32 %4, 20
                ret i32 %0
        }

3) Dead code elimination from me:

        define dso_local noundef i32 @_Z5code3i(i32 noundef %0) #0 {
                ret i32 %0
        }


