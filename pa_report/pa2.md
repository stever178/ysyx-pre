- `am-kernels/tests/`, `riscv-tests-am/`, `riscv-arch-test-am/` 测试命令
  - `make ARCH=riscv32-nemu ALL=... run`

- `请整理一条指令在NEMU中的执行过程.`
  - `cpu_exec` -> `execute` -> `exec_once` 

    - -> `isa_exec_once`

	  - -> `inst_fetch` -> `vaddr_ifetch` -> `paddr_read`
	  update pc
	  fetch instruction

	  - -> `decode_exec` -> `INSTPAT`
	  instruction pattern match (notie the relation between `(uint64_t)INSTPAT_INST(s) >> shift` and `little-endian storage format`)
	  execute

    - -> `disassemble`

- `为什么要有AM? 操作系统也有自己的运行时环境. AM和操作系统提供的运行时环境有什么不同呢? 为什么会有这些不同?`
	AM 和操作系统提供的运行时环境的不同，源于它们设计目标和服务对象的不同。

	AM 面向单个可信程序，提供最小、统一的硬件抽象，强调机制而非策略；
	操作系统面向多程序、多用户环境，负责资源管理、安全隔离和调度，因此运行时环境更复杂、更严格。

	AM 的存在使程序能够在没有或不依赖具体操作系统的情况下运行，同时也为理解和构建操作系统提供了基础。
