CC       := clang
CFLAGS   := -Wall -Werror -Wextra -O2 -g
CPPFLAGS := -MMD
LDFLAGS  :=
LDLIBS   := -lraylib -lm -lpthread -ldl -lrt -lX11

SRC_DIR   := src
BUILD_DIR := .build
ROMS_DIR  := roms
BIN       := $(BUILD_DIR)/gem-league

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(LDFLAGS) $(OBJS) -o $@ $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	@rm -rf $(BUILD_DIR)
