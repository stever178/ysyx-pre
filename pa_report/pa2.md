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
