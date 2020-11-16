CXX = gcc

# SDL_LIB = -lSDL2
# SDL_INCLUDE = -I/usr/local/include

CXXFLAGS = -Wall -c 
# LDFLAGS = $(SDL_LIB)
PROJ_NAME = rcom

OBJS_DIR := obj
SRCS_DIR := src
INC_DIR := include
SRCS_FILES := $(SRCS_DIR)/main.c $(SRCS_DIR)/auxiliar.c $(SRCS_DIR)/linkLayer.c $(SRCS_DIR)/appLayer.c $(SRCS_DIR)/frame.c $(SRCS_DIR)/stuffing.c

OBJS_FILES = $(patsubst $(SRCS_DIR)/%.c,$(OBJS_DIR)/%.o, $(SRCS_FILES))


.PHONY: all clean test

all: read write

read: $(OBJS_FILES)
	$(CXX) $(OBJS_FILES) $(LDFLAGS) -o $@

write: $(OBJS_FILES)
	$(CXX) $(OBJS_FILES) $(LDFLAGS) -o $@


$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p obj
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) $< -o $@

test:
	@echo "SRC FILES = '$(SRCS_FILES)'"
	@echo "OBJ FILES = '$(OBJS_FILES)'"

clean:
	rm -f $(OBJS_DIR)/*.o
	rm -f read
	rm -f write
