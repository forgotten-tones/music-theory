# Makefile for Mahler Music Theory Library

# Variables
CMAKE = cmake
BUILD_DIR = build
TEST_BUILD_DIR = test/build
BIN_DIR = bin
EXAMPLES_DIR = ex

# Default target
.PHONY: all
all: compile

# Compile the library
.PHONY: compile
compile: clean
	@echo "Compiling Mahler library..."
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && $(CMAKE) ..
	cd $(BUILD_DIR) && $(CMAKE) --build .

# Run all tests
.PHONY: test
test: clean test-compile
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

# Compile and install examples
.PHONY: examples
examples: compile
	@echo "Building examples with CMake..."
	cd $(BUILD_DIR) && $(CMAKE) --build . --target chart
	cd $(BUILD_DIR) && $(CMAKE) --build . --target fifth
	cd $(BUILD_DIR) && $(CMAKE) --build . --target quartal
	cd $(BUILD_DIR) && $(CMAKE) --build . --target quintal
	cd $(BUILD_DIR) && $(CMAKE) --build . --target trans
	@echo "Copying examples to $(BIN_DIR)..."
	@mkdir -p $(BIN_DIR)
	@cp $(BUILD_DIR)/chart $(BIN_DIR)/
	@cp $(BUILD_DIR)/fifth $(BIN_DIR)/
	@cp $(BUILD_DIR)/quartal $(BIN_DIR)/
	@cp $(BUILD_DIR)/quintal $(BIN_DIR)/
	@cp $(BUILD_DIR)/trans $(BIN_DIR)/
	@echo "Examples installed in $(BIN_DIR)/"

# Run all compiled examples
.PHONY: run-examples
run-examples: examples
	@echo "Running all examples..."
	@for example in $(BIN_DIR)/*; do \
		if [ -x "$$example" ]; then \
			example_name=$$(basename "$$example"); \
			echo "=== Running $$example_name ==="; \
			"$$example" || exit 1; \
			echo ""; \
		fi; \
	done
	@echo "All examples completed."

# Clean build artifacts
.PHONY: clean
clean:
	@echo "Cleaning build artifacts..."
	rm -rf $(BUILD_DIR)
	rm -rf $(TEST_BUILD_DIR)
	rm -rf $(BIN_DIR)

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
	@echo "  examples     - Compile and install all examples to ./bin"
	@echo "  run-examples - Compile and run all examples"
	@echo "  test         - Compile and run all tests"
	@echo "  test-compile - Compile the test suite only"
	@echo "  clean        - Remove build artifacts and examples"
	@echo "  rebuild      - Clean and rebuild library"
	@echo "  rebuild-all  - Clean and rebuild library and tests"
	@echo "  help         - Show this help message"
