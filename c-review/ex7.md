
- 类型转换的规则
C 语言规定，当不同类型的操作数参与运算时，会根据以下规则进行类型转换：

整型提升：char、short 等较小类型会被转换为 int 或 unsigned int。如果变量的值可以放入 int 的范围内，则提升为 int，否则提升为 unsigned int。
两边的类型匹配：在二元运算（如加、减、乘、除）中，编译器会将两边的操作数转换为相同的类型。通常较小类型会提升为较大类型。
