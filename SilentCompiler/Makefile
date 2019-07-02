CC = g++
CFLAGS = -std=c++11 -Wall
SRC_PATH = src
OBJ_PATH = obj
OUT_PATH = bin
EXE_NAME = SilentC
EXE = $(OUT_PATH)/$(EXE_NAME)
MKDIR = mkdir

# SOURCES = \
# 	$(SRC_PATH)/SilentCompiler.cpp \
# 	$(SRC_PATH)/SilentParser.cpp \
# 	$(SRC_PATH)/SilentTokenizer.cpp \
# 	$(SRC_PATH)/SilentCodeGen.cpp 
	
	# $(SRC_PATH)/SilentConsole.cpp 
#$(SRC_PATH)/SilentCleanup.cpp 
#	$(SRC_PATH)/SilentIntCode.cpp
#	$(SRC_PATH)/SilentDebugger.cpp
#	$(SRC_PATH)/SilentAssembler.cpp 

SOURCES = \
	$(SRC_PATH)/SCompiler.cpp \
	$(SRC_PATH)/STokenizer.cpp  \
	$(SRC_PATH)/SParser2.cpp  \
	$(SRC_PATH)/SCodeGen.cpp 


OBJECTS = $(SOURCES:$(SRC_PATH)/%.cpp=$(OBJ_PATH)/%.o)
DEPENDENCIES = $(OBJECTS:.o=.d)

.PHONY: all
all: $(EXE)

$(EXE) : $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

-include $(DEPENDENCIES)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	$(CC) $(CFLAGS) -MP -MMD -c $< -o $@

.PHONY: debug
debug: CFLAGS += -g
debug: all


.PHONY: clean
clean:
	@$(RM) -r $(OBJ_PATH)/*
