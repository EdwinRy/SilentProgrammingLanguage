CC = gcc
CFLAGS = -std=c99 -Wall -O3
#LIBS = -ldl
SRC_PATH = src
OBJ_PATH = obj
OUT_PATH = bin
EXE_NAME = SilentVM
EXE = $(OUT_PATH)/$(EXE_NAME)
MKDIR = mkdir

SOURCES = \
	$(SRC_PATH)/main.c \
	$(SRC_PATH)/SilentLib.c \
	$(SRC_PATH)/SilentVM.c
	
OBJECTS = $(SOURCES:$(SRC_PATH)/%.c=$(OBJ_PATH)/%.o)
DEPENDENCIES = $(OBJECTS:.o=.d)

.PHONY: all
all: $(EXE)

$(EXE) : $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $@

-include $(DEPENDENCIES)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	$(CC) $(CFLAGS) -MP -MMD -c $< -o $@

.PHONY: debug
debug: CFLAGS += -g
debug: all 


.PHONY: clean
clean:
	@$(RM) -r $(OBJ_PATH)/*
