CFLAGS = -g -O2 -Wall -Wextra -Isrc -rdynamic -DNDEBUG $(OPTFLAGS)
LIBS = -ldl $(OPTLIBS)
PREFIX ?= /usr/local

SOURCES = $(wildcard src/**/*.c src/*.c)
OBJECTS = $(patsubst %.c, %.o, $(SOURCES))

TEST_SRC = $(wildcard tests/*_tests.c)
TESTS = $(patsubst tests/%.c, build/%, $(TEST_SRC))

TARGET = build/liblcthw.a
SO_TARGET = $(patsubst %.a,%.so,$(TARGET))

# The Target Build
all: $(TARGET) $(SO_TARGET) tests move_ccjson

CCJSON = ./compile_commands.json
move_ccjson:
	@if [ -f "$(CCJSON)" ]; then \
		echo "Moving $(CCJSON) to build/"; \
		mv "$(CCJSON)" build/ 2>/dev/null || echo "No $(CCJSON) file to move"; \
	else \
		echo "No $(CCJSON) file to move"; \
	fi

dev: CFLAGS = -g -Wall -Isrc -Wall -Wextra $(OPTFLAGS)
dev: all

$(TARGET): CFLAGS += -fPIC
$(TARGET): build $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	ranlib $@
	
$(SO_TARGET): $(TARGET) $(OBJECTS)
	$(CC) -shared -o $@ $(OBJECTS)

build:
	@mkdir -p build

# The Unit Tests
LDLIBS = $(TARGET) $(LIBS)
$(TESTS): build/%_tests: tests/%_tests.c $(TARGET)
	$(CC) $(CFLAGS) $< -o $@ $(LDLIBS)

TEST_LOG = "build/tests.log"
tests: $(TESTS) move_ccjson
	@TEST_LOG=$(TEST_LOG) sh ./tests/runtests.sh

valgrind:
	VALGRIND="valgrind --log-file=build/valgrind-%p.log" $(MAKE) tests

# The Cleaner
clean:
	rm -f $(TEST_LOG)
	rm -rf build $(OBJECTS) $(TESTS)
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`

# The Install
install: all
	install -d $(DESTDIR)/$(PREFIX)/lib/
	install $(TARGET) $(DESTDIR)/$(PREFIX)/lib/

# The Checker
BADFUNCS='[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)'
check:
	@echo Files with potentially dangerous functions.
	@egrep $(BADFUNCS) $(SOURCES) || true

.PHONY: all build tests clean install check move_ccjson
