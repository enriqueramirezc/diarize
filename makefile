#variables
XC=g++
STD=-std=c++20
WFLAGS=-Wall -Wextra
APPNAME=program

#directorios
SRC=src
BUILD=build
BIN=bin

#archivos
DIRS=$(shell find -L $(SRC) -type d)
FSOURCE=$(wildcard $(DIRS:%=%/*.cpp))
FHEADER=$(wildcard $(DIRS:%=%/*.hpp))
FOBJECT=$(FSOURCE:$(SRC)/%.cpp=$(BUILD)/%.o)

.PHONY: all bin show

all: $(BIN)/$(APPNAME)

$(BIN)/$(APPNAME): $(FOBJECT) | $(BIN)
	$(XC) $(STD) $(WFLAGS) -g $^ -o $@

$(BUILD)/%.o: $(SRC)/%.cpp | $(BUILD)
	$(XC) $(STD) $(WFLAGS) -c -g $^ -o $@

$(BIN):
	mkdir -p $@

$(BUILD):
	mkdir -p $@

run:
	./$(BIN)/$(APPNAME)

clean:
	rm -rf $(BIN) $(BUILD)

show:
	echo $(FOBJECT)
