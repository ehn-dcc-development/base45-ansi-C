TARGET_EXEC ?= test
TARGET_LIB ?= libbase64.a

SRC_DIRS ?= .

SRCS := base45.c
OBJS := $(SRCS:.c=.o)
DEPS := $(OBJS:.o=.d)
OBJS_TEST := test.o

INC_DIRS := 
INC_FLAGS := 

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP

all: $(TARGET_EXEC) $(TARGET_LIB)
	./$(TARGET_EXEC)

$(TARGET_EXEC): $(OBJS_TEST) $(TARGET_LIB)
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

