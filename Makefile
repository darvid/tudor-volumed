include config.mk

all: $(BIN)

$(BIN): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $(BIN) $(LIBS) -ggdb

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@ -ggdb

clean:
	rm -f $(BIN) $(OBJECTS)

.PHONY: clean all