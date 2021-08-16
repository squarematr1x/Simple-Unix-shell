BUILD_DIR = ./build

SOURCES = main.c command.c util.c

EXE = shell

CFLAGS = -Wall

CC = gcc

HIDE = @

OBJECTS = $(SOURCES:%=$(BUILD_DIR)/%.o)

$(EXE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

$(BUILD_DIR)/%.c.o: %.c
	@echo Building...
	$(HIDE) mkdir -p $(dir $@)
	$(CC) $(CFAGS) -c $< -o $@

.PHONY: clean
clean:
	$(HIDE) -rm $(EXE)
	$(HIDE) -rm -r $(BUILD_DIR)
