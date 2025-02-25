# Set EC_DIR only if it's not already set
EC_DIR ?= /home/$(USER)/Documents/ethercat

# Set CC to gcc if it's not already set
CC ?= gcc

# Target and source
TARGET = ectest
SRC = ectest.c

# Compiler flags
CFLAGS = -I$(EC_DIR)/include

# Build the target
$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(EC_DIR)/install/lib/libethercat.a

# Clean up
clean:
	rm -f $(TARGET)

.PHONY: clean
