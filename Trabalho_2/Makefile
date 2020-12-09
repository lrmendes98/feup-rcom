CXX = gcc
CXXFLAGS = -Wall -c 
PROJ_NAME = rcom

OBJS_DIR := obj
SRCS_DIR := src
INC_DIR := include
SRCS_FILES := $(SRCS_DIR)/main.c $(SRCS_DIR)/auxiliar.c

OBJS_FILES = $(patsubst $(SRCS_DIR)/%.c,$(OBJS_DIR)/%.o, $(SRCS_FILES))

.PHONY: all clean test

all: download

download: $(OBJS_FILES)
	$(CXX) $(OBJS_FILES) $(LDFLAGS) -o $@


$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p obj
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) $< -o $@

test:
	@echo "SRC FILES = '$(SRCS_FILES)'"
	@echo "OBJ FILES = '$(OBJS_FILES)'"

clean:
	rm -f $(OBJS_DIR)/*.o
	rm -f download
