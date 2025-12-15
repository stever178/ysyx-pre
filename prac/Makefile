hello: hello.c
	@gcc -g -ggdb -o hello.out $^

cal: cal.c
	@echo "== O0 =="
	@gcc -g -O0 -fsanitize=shift -o cal.out $^
	#@gcc -g -O0 -fsanitize=shift -Werror=shift-count-overflow -Werror=div-by-zero -o cal.out $^
	@./cal.out

clean:
	@rm -rf *.out

