debug  = y
COMPILE_PREFIX ?= 

CC  = $(COMPILE_PREFIX)gcc
CXX = $(COMPILE_PREFIX)g++
AR  = $(COMPILE_PREFIX)ar

target = connect
BUILD_DIR ?= make_build

LIBS    += -llibgo -ldl -lstatic_hook -lc  -lpthread -lrt 
STATIC_LIBS += ./lib/boost/libboost_json.a
INC_DIR = ./inc ./inc/libgo ./uhconnect ./uhshell/letter-shell/extensions/cpp_support ./uhshell/letter-shell/src  ./uhshell ./uhshell/letter-shell/extensions/log   #./kmeans
LIB_DIR = ./lib/libgo  ./lib/boost 
SRC_DIR = ./uhconnect  ./kmeans #./montecarlo 

DYNC_FLAGS += -fpic -shared

INC_DIR_FLAGS := $(addprefix -I, $(INC_DIR))
LIB_DIR_FLAGS := $(addprefix -L, $(LIB_DIR))

CFLAGS += -Wall -Wfatal-errors -fstack-protector-all -DRUN_DEMO -DDB_BLOCK_WITHOUT_COPY -DUSE_THREAD_CH  -DKMEANS_PIONT_FLOAT #-DSEP_THREAD #-DDEBUGINFO # -MMD
ifeq ($(debug), y)
    CFLAGS += -ggdb -rdynamic
else
    CFLAGS += -O2 -s
endif
CFLAGS +=  $(INC_DIR_FLAGS) 
CXXFLAGS = $(CFLAGS)  -std=c++11 
LDFLAGS += $(LIBS)  $(LIB_DIR_FLAGS) 
SRCS := $(shell find $(SRC_DIR) -name '*.cpp' -or -name '*.c')
SRCS += demo.cpp
OBJS = $(patsubst %.c,$(BUILD_DIR)/%.o, $(patsubst %.cpp,$(BUILD_DIR)/%.o, $(SRCS))) 

###############################################################################

all: $(target)

$(target): $(OBJS)
ifeq ($(suffix $(target)), .so)
	@echo "Generating dynamic lib file..." $(notdir $(target))
	$(CXX) $^ $(LDFLAGS) $(DYNC_FLAGS) -o $(target)
else ifeq ($(suffix $(target)), .a)
	@echo "Generating static lib file..." $(notdir $(target))
	$(AR) $(ARFLAGS) -o $(target) $^
else
	@echo "Generating executable file..." $(notdir $(target))
	$(CXX) $^ $(LDFLAGS) -o $(target) $(STATIC_LIBS)
endif

$(BUILD_DIR)/%.o: %.c
	@if [ ! -d  $(dir $@) ]; then mkdir -p $(dir $@); fi
	@echo "Compiling: " $(addsuffix .c, $(basename $(notdir $@)))
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.cpp
	@if [ ! -d $(dir $@) ]; then mkdir -p $(dir $@); fi
	@echo "Compiling: " $(addsuffix .cpp, $(basename $(notdir $@)))
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "Cleaning..."
ifneq ($(BUILD_DIR),)
	rm -rf  $(BUILD_DIR)/*
endif
	rm -rf $(target)

.PHONY: all clean
