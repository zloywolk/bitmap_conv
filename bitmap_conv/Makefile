CC=gcc
CFLAGS=-I. -Wall -c

BUILD_DIR=obj

SOURCES=$(wildcard *.c)
DEPS=$(wildcard *.h)
BMPS=$(wildcard *.bmp)
OBJS=$(patsubst %,$(BUILD_DIR)/%,$(patsubst %.c,%.o,$(wildcard *.c)))

TARGET=bitmap_conv

.PHONY: all clean

all: $(TARGET) copy_bmp

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(BUILD_DIR)/$@

$(BUILD_DIR)/%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) $< -o $@

copy_bmp:
	cp $(BMPS) $(BUILD_DIR) -rvf

clean:
	rm -rvf $(BUILD_DIR)/*


