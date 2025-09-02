# Makefile for Mahler Music Theory Library

# Variables
CMAKE = cmake
BUILD_DIR = build
TEST_BUILD_DIR = test/build

# Default target
.PHONY: all
all: compile

# Compile the library
.PHONY: compile
compile:
	@echo "Compiling Mahler library..."
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && $(CMAKE) ..
	cd $(BUILD_DIR) && $(CMAKE) --build .

# Run all tests
.PHONY: test
test: test-compile
	@echo "Running test suite..."
	cd $(TEST_BUILD_DIR) && ./test

# Compile tests
.PHONY: test-compile
test-compile:
	@echo "Compiling test suite..."
	mkdir -p $(TEST_BUILD_DIR)
	cd test && mkdir -p build
	cd $(TEST_BUILD_DIR) && $(CMAKE) ..
	cd $(TEST_BUILD_DIR) && $(CMAKE) --build .

# Clean build artifacts
.PHONY: clean
clean:
	@echo "Cleaning build artifacts..."
	rm -rf $(BUILD_DIR)
	rm -rf $(TEST_BUILD_DIR)

# Clean and rebuild everything
.PHONY: rebuild
rebuild: clean all

# Clean and rebuild with tests
.PHONY: rebuild-all
rebuild-all: clean compile test

# Show help
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all          - Compile the library (default)"
	@echo "  compile      - Compile the Mahler library"
	@echo "  test         - Compile and run all tests"
	@echo "  test-compile - Compile the test suite only"
	@echo "  clean        - Remove build artifacts"
	@echo "  rebuild      - Clean and rebuild library"
	@echo "  rebuild-all  - Clean and rebuild library and tests"
	@echo "  help         - Show this help message"