- `riscv32`
  - `riscv32`有哪几种指令格式?
    1. base instruction formats: 6 types, including `R-type`, `I-type`, `S-type`, `B-type`, `U-type`, `J-type`
    2. compressed instruction formats: 9 types, including CR, CI, CSS, CIW, CL, CS, CA, CB (C extension can be added to any of the base ISAs)
  - `LUI`指令的行为是什么?
    - places the U-immediate value in the top 20 bits of the destination register rd, filling in the lowest 12 bits with zeros
  - `mstatus`寄存器的结构是怎么样的?
    - 详见手册。在仅有机器模式且无 F 和 V 扩展的简单处理器中，有效字段只有全局中断使能、MIE 和 MPIE（发生异常后存放 MIE 的旧值）。


- 完成PA1的内容之后, `nemu/`目录下的所有.c和.h和文件总共有多少行代码? 
  - 统计行数（包括空行）
    ```shell
    find . -type f \( -name "*.c" -o -name "*.h" \) -exec cat {} \; | wc -l
    ```
  - 统计代码行数
    ```shell
    cloc --match-f="\.(c|h)$" .
    # or
    cloc --include-lang="C,C/C++ Header" .
    ```


- 请解释`gcc`中的`-Wall`和`-Werror`有什么作用? 为什么要使用`-Wall`和`-Werror`?
`-Wall`: 启用所有警告，开启GCC认为“有用”的大多数警告选项，帮助发现潜在问题
`-Werror`: 将警告视为错误，将所有警告转换为编译错误，使编译在出现警告时失败
