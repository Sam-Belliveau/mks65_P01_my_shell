# Directories for compiling results
BIN=./bin
OBJ=./obj
SRC=./src

# Output for binary
OUTPUT=$(BIN)/enlist_your_c_skillz

# Get headers and c files
DEPS=$(wildcard $(SRC)/*.h)
SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))

# Compiler / Compiler Settings
LINKS=-lm
FLAGS=-O2 -D DEBUG_TRAINS
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

# Clean make output
clean:
	rm -rf $(BIN)
	rm -rf $(OBJ)