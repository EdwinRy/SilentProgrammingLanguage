CC = g++
CFLAGS = -std=c++11
SRC_PATH = src
OBJ_PATH = obj
OUT_PATH = bin
EXE_NAME = SilentC
EXE = $(OUT_PATH)/$(EXE_NAME)
MKDIR = mkdir

SOURCES = \
	$(SRC_PATH)/SilentCleanup.cpp \
	$(SRC_PATH)/SilentCodeGen.cpp \
	$(SRC_PATH)/SilentCompiler.cpp \
	$(SRC_PATH)/SilentConsole.cpp \
	$(SRC_PATH)/SilentDebugger.cpp \
	$(SRC_PATH)/SilentIntCode.cpp \
	$(SRC_PATH)/SilentIntCodeParser.cpp \
	$(SRC_PATH)/SilentParser.cpp \
	$(SRC_PATH)/SilentTokenizer.cpp \
	$(SRC_PATH)/SilentAssembler.cpp 
	
OBJECTS = $(SOURCES:$(SRC_PATH)/%.cpp=$(OBJ_PATH)/%.o)
DEPENDENCIES = $(OBJECTS:.o=.d)


.PHONY: cleanOBJ
cleanOBJ:
	@$(RM) -r $(OBJ_PATH)/*

# .PHONY: dirs

# dirs: $(OBJ_PATH)

# $(OBJ_PATH):
# 	$(MKDIR) $(OBJ_PATH)

.PHONY: all
#all: dirs $(EXE)
all: dirs $(EXE)

$(EXE) : $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

-include $(DEPENDENCIES)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	$(CC) $(CFLAGS) -MP -MMD -c $< -o $@

