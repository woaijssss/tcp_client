BUILD:=debug

SRC_DIR:=src
BUILD_DIR:=build
BIN_DIR:=bin

TEST_DIR:=test

SRC:=$(wildcard $(SRC_DIR)/*.cpp) 

OBJ:=$(addprefix $(BUILD_DIR)/, $(SRC:.cpp=.o))
BIN:=tcp_client

CPPFLAGS+=-Iinclude -Iinclude/$(COLLECTOR) -Iinclude/$(LOGGER) \
		-Iinclude/$(UTILS) -I/usr/local/include

CXXFLAGS+=-Wall -pedantic -Wextra -std=c++11 -MMD -D_GLIBCXX_USE_NANOSLEEP \
	-Wno-deprecated -Wdeprecated-declarations \
	-Wno-unused-parameter  # 设置此项，不提示“未使用变量”，正式程序需要去掉
LDFLAGS:=-L/usr/local/lib
LDLIBS:=-pthread -lboost_system -lboost_thread -lrt -lcurl -llog4cplus -ljson

ifeq ($(BUILD), release)
CPPFLAGS+=-DNDEBUG
CFLAGS+=-O2
CXXFLAGS+=-O2
LDFLAGS+=-O2 -s
else
#CPPFLAGS+=-DDEBUG
CFLAGS+=-O0 -g
CXXFLAGS+=-O0 -g
LDFLAGS+=-O0 -g
endif

.PHONY: all release clean

all: $(BIN_DIR)/$(BIN)
	@:

release:
	@make -s "BUILD=release"

$(BIN_DIR)/$(BIN): $(OBJ)
	@mkdir -p $(dir $@)
	@echo "(LD) $@"
	@$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "(CXX) $@"
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< -c -o $@

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "(CXX) $@"
	@$(CC) $(CFLAGS) $< -c -o $@

clean:
	@rm -rf $(BUILD_DIR)/* $(BIN_DIR)/$(BIN)

clean_all:
	@rm -rf $(BUILD_DIR)/* $(BIN_DIR)/*

-include $(OBJ:.o=.d)
