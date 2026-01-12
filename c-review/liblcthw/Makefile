CFLAGS = -g -O2 -Wall -Wextra -Isrc -rdynamic -DNDEBUG $(OPTFLAGS)
LIBS = -ldl $(OPTLIBS)
PREFIX ?= /usr/local

SOURCES = $(wildcard src/**/*.c src/*.c)
OBJECTS = $(patsubst src/lcthw/%.c, build/%.o, $(SOURCES))

TEST_SRC = $(wildcard tests/*_tests.c)
TESTS = $(patsubst tests/%.c, build/%, $(TEST_SRC))

TARGET = build/liblcthw.a
SO_TARGET = $(patsubst %.a,%.so,$(TARGET))

TEST_LOG = "build/tests.log"

CCJSON = ./compile_commands.json
move_ccjson:
	@if [ -f "$(CCJSON)" ]; then \
		echo "Moving $(CCJSON) to build/"; \
		mv "$(CCJSON)" build/ 2>/dev/null || echo "No $(CCJSON) file to move"; \
	else \
		echo "No $(CCJSON) file to move"; \
	fi

all: $(TARGET) $(SO_TARGET) move_ccjson

dev: CFLAGS = -g -Wall -Isrc -Wall -Wextra $(OPTFLAGS)
dev: all

$(OBJECTS): build/%.o: src/lcthw/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): CFLAGS += -fPIC
$(TARGET): $(OBJECTS)
	@echo "=== Building $(TARGET), depending on $(OBJECTS) ==="
	ar rcs $@ $(OBJECTS)
	ranlib $@
	
$(SO_TARGET): $(TARGET) $(OBJECTS)
	$(CC) -shared -o $@ $(OBJECTS)

LDLIBS = $(TARGET) $(LIBS)
$(TESTS): build/%_tests: tests/%_tests.c $(TARGET)
	$(CC) $(CFLAGS) $< -o $@ $(LDLIBS)

build-tests: $(TESTS) move_ccjson
	@echo "=== Building Tests ==="

# The Unit Tests
run-tests: build-tests
	@echo "=== Running Tests ==="
	TEST_LOG=$(TEST_LOG) sh ./tests/runtests.sh

tests: build-tests run-tests

valgrind: $(TESTS)
	VALGRIND="valgrind --log-file=build/valgrind.todo.log" $(MAKE) tests

cachegrind: $(TESTS)
	@mkdir -p build/cachegrind
	VALGRIND="valgrind --tool=cachegrind \
		--log-file=build/cachegrind/todo.log \
		--cachegrind-out-file=build/cachegrind/todo.out" \
		$(MAKE) tests
	@echo "=== All Cachegrind Analysis Complete ==="

callgrind: $(TESTS)
	@mkdir -p build/callgrind
	export VALGRIND="valgrind --tool=callgrind \
		--log-file=build/callgrind/todo.log \
		--callgrind-out-file=build/callgrind/todo.out" \
		$(MAKE) tests
	@echo "=== All Callgrind Analysis Complete ==="

analyze-cache: cachegrind
	@echo "=== Running Cachegrind Analysis ==="
	@sh ./tests/analyze_cache.sh

analyze-call: callgrind
	@echo "=== Running Callgrind Analysis ==="
	@sh ./tests/analyze_call.sh

analyze: analyze-cache analyze-call
	@echo "=== All Analysis Complete ==="

list-analysis:
	@echo "Available analysis targets:"
	@for target in $(ANALYSIS_TARGETS); do \
	    echo "  analyze_$$target"; \
	done
	@echo "Or use: make analyze  # for all files"

# The Cleaner
clean:
	rm -f $(TEST_LOG)
	rm -rf build $(OBJECTS) $(TESTS)
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`

vclean: valgrind-clean
valgrind-clean:
	rm -f build/valgrind-*.log
	rm -f build/cachegrind*
	rm -f build/callgrind*

clean-analysis:
	rm -f build/*.log.* build/*.out.*

# The Install
install: all
	install -d $(DESTDIR)/$(PREFIX)/lib/
	install $(TARGET) $(DESTDIR)/$(PREFIX)/lib/

# The Checker
BADFUNCS='[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)'
check:
	@echo Files with potentially dangerous functions.
	@egrep $(BADFUNCS) $(SOURCES) || true

debug-make:
	@echo "MAKE variable is: '$(MAKE)'"
	@echo "Length: $(words $(MAKE)) words"

.PHONY: all build install check debug-make clean move_ccjson \
	valgrind vclean valgrind-clean \
	cachegrind callgrind analyze clean-analysis list-analysis \
	build-tests run-tests tests
