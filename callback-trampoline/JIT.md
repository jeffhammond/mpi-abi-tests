# Command-line codegen
```
echo "#include <stdio.h>\nint main(void){printf(\"Hello, World\\\n\");return 0;}" | gcc -x c - && ./a.out
Hello, World
```
