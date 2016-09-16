CC := g++-4.8
Flag := -w -stdc++11
BIN := bin
BUILD := build
HEAD := include
SRC := src
INCLUDE := -I./$(HEAD)

$(BIN)/Agenda: $(BUILD)/Agenda.o $(BUILD)/AgendaUI.o \
	$(BUILD)/AgendaService.o $(BUILD)/Storage.o $(BUILD)/Meeting.o \
	$(BUILD)/Date.o $(BUILD)/User.o
	@mkdir -p $(BIN)
	$(CC) $(FLAGS) $(INCLUDE) $(BUILD)/Agenda.o $(BUILD)/AgendaUI.o \
	$(BUILD)/AgendaService.o $(BUILD)/Storage.o $(BUILD)/Meeting.o \
	$(BUILD)/Date.o $(BUILD)/User.o -o $@

$(BUILD)/Agenda.o: $(SRC)/Agenda.cpp
	@mkdir -p $(BUILD)
	$(CC) $(FLAGS) $(INCLUDE) $(SRC)/Agenda.cpp -c -o $@

$(BUILD)/AgendaUI.o: $(SRC)/AgendaUI.cpp
	@mkdir -p $(BUILD)
	$(CC) $(FLAGS) $(INCLUDE) $(SRC)/AgendaUI.cpp -c -o $@

$(BUILD)/AgendaService.o: $(SRC)/AgendaService.cpp
	@mkdir -p $(BUILD)
	$(CC) $(FLAGS) $(INCLUDE) $(SRC)/AgendaService.cpp -c -o $@

$(BUILD)/Storage.o: $(SRC)/Storage.cpp
	@mkdir -p $(BUILD)
	$(CC) $(FLAGS) $(INCLUDE) $(SRC)/Storage.cpp -c -o $@

$(BUILD)/Meeting.o: $(SRC)/Meeting.cpp
	@mkdir -p $(BUILD)
	$(CC) $(FLAGS) $(INCLUDE) $(SRC)/Meeting.cpp -c -o $@

$(BUILD)/Date.o: $(SRC)/Date.cpp
	@mkdir -p $(BUILD)
	$(CC) $(FLAGS) $(INCLUDE) $(SRC)/Date.cpp -c -o $@

$(BUILD)/User.o: $(SRC)/User.cpp
	@mkdir -p $(BUILD)
	$(CC) $(FLAGS) $(INCLUDE) $(SRC)/User.cpp -c -o $@

make clean:
	rm -f $(BUILD)/*.o
	rm -f $(BIN)/main 
