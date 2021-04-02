TARGET_TEST ?= test
TARGET_EXEC ?= base45
TARGET_LIB ?= libbase45.a

SRC_DIRS ?= .

SRCS := base45.c
OBJS := $(SRCS:.c=.o)
DEPS := $(OBJS:.o=.d)
OBJS_TEST := test.o

INC_DIRS := 
INC_FLAGS := 

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP

tests: all
	for i in 1 7 13 10 103 1002 10009 100007 1000001 0; do \
		openssl rand $$i > $$TMPDIR/x; \
	 	cat $$TMPDIR/x | ./base45 | ./base45 -d > $$TMPDIR/y; \
                diff $$TMPDIR/x $$TMPDIR/y || exit 1; \
	done; \
	rm  $$TMPDIR/x $$TMPDIR/y 

all: $(TARGET_EXEC) $(TARGET_LIB)

$(TARGET_EXEC): $(SRCS)
	$(CC) $(SRCS) -DBASE45_UTIL -o $@ $(LDFLAGS) 

$(TARGET_TEST): $(OBJS_TEST) $(TARGET_LIB)
	$(CC) $(OBJS_TEST) -o $@ $(LDFLAGS) -lbase64 -L.

$(TARGET_LIB): $(OBJS)
	ar -rcs $@ $<

# c source
%.c.o: %.c base64.h
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) -f $(OBJS) $ $(OBJS_TEST) $(TARGET_EXEC) $(DEPS) $(TARGET_LIB)

-include $(DEPS)

