  define dso_local i32 @sub(i32 noundef %0, i32 noundef %1) { 
   %3 = sub nsw i32 %0, %1
   ret i32 %3
  }
