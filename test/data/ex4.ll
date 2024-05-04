; ModuleID = 'sources/ex4.cpp'
source_filename = "sources/ex4.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: mustprogress noinline nounwind optnone sspstrong uwtable
define dso_local noundef i32 @_Z5code1v() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store i32 5, ptr %1, align 4
  %6 = load i32, ptr %1, align 4
  %7 = add nsw i32 6, %6
  store i32 %7, ptr %2, align 4
  %8 = load i32, ptr %2, align 4
  %9 = add nsw i32 7, %8
  store i32 %9, ptr %3, align 4
  %10 = load i32, ptr %3, align 4
  %11 = add nsw i32 8, %10
  store i32 %11, ptr %4, align 4
  %12 = load i32, ptr %4, align 4
  %13 = add nsw i32 9, %12
  store i32 %13, ptr %5, align 4
  ret i32 0
}

attributes #0 = { mustprogress noinline nounwind optnone sspstrong uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 16.0.6"}
