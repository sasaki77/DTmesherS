CC      = g++
CFLAGS  = -g -MMD -MP
LDFLAGS = -lm
LIBS    = 
INCLUDE = -I ./include
SRC_DIR = ./src
OBJ_DIR = ./build
SOURCES = $(shell ls $(SRC_DIR)/*.cpp) 
OBJS    = $(subst $(SRC_DIR),$(OBJ_DIR), $(SOURCES:.cpp=.o))
TARGET  = DTmesherS
DEPENDS = $(OBJS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJS) $(LIBS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp 
	@if [ ! -d $(OBJ_DIR) ]; \
		then echo "mkdir -p $(OBJ_DIR)"; mkdir -p $(OBJ_DIR); \
		fi
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $< 

# program test
MAIN = $(OBJ_DIR)/main.o
OBJS_NOMAIN = $(filter-out $(MAIN),$(OBJS))
TEST_DIR    = ./test
TEST_TARGET = $(TEST_DIR)/test
TEST_OBJ    = $(TEST_TARGET).o

dotest: test
	$(TEST_TARGET)

test: $(TEST_OBJ) $(OBJS_NOMAIN) $(LIBS)
	$(CC) -o $(TEST_DIR)/$@ $(TEST_OBJ) $(OBJS_NOMAIN)

$(TEST_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $<

clean:
	$(RM) $(OBJS) $(TARGET) $(DEPENDS) $(TEST_TARGET) $(TEST_OBJ)

-include $(DEPENDS)

.PHONY: all clean dotest
