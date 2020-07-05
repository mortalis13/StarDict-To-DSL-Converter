PR := stardict_to_dsl.exe

CPPFLAGS:=-std=c++11
LDFLAGS:=-std=c++11 -lws2_32

HEADERS := $(wildcard *.h)

SRC := $(wildcard *.cpp)
OBJ := $(notdir $(SRC:.cpp=.o))


# ------------------------

all: $(PR)

$(PR): $(OBJ)
	g++ -g $^ -o $@ $(LDFLAGS)

%.o: %.cpp $(HEADERS)
	g++ -o $@ $(CPPFLAGS) -c $<

clean:
	del /Q /S *.o *.exe
