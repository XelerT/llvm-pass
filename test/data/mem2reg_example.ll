define dso_local i32 @square_of_7() #0 {
  %1 = alloca i32, align 4
  store i32 7, ptr %1, align 4
  %2 = load i32, ptr %1, align 4
  %3 = load i32, ptr %1, align 4

  %4 = mul i32 %2, %3

  ret i32 %4
}

