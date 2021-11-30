# Directories for compiling results
BIN=./bin
OBJ=./obj
SRC=./src

# Output for binary
OUTPUT=$(BIN)/my_shell

# Get headers and c files
DEPS=$(wildcard $(SRC)/*.h)
SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))

# Compiler / Compiler Settings
LINKS=-lm
FLAGS=-O2
COMPILER=gcc $(FLAGS)

# Command to create directory
MKDIR=mkdir

# Compile the Binary
all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	$(MKDIR) -p $(BIN)
	$(COMPILER) $^ -o $@ $(LINKS)

# Compile Every Object
$(OBJ)/%.o: $(SRC)/%.c $(DEPS)
	$(MKDIR) -p $(@D)
	$(COMPILER) -c $< -o $@

# Run the binary
run: $(OUTPUT)
	$(OUTPUT)

# Run the binary
test: $(OUTPUT)
	$(OUTPUT) ./scripts/test_script.salsh

# Clean make output
clean:
	rm -rf $(BIN)
	rm -rf $(OBJ)