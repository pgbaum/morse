CXXFLAGS += -g -Wall -pedantic -std=c++11
GTKLIBS = $(shell pkg-config --libs gtk+-3.0)

FILES=morseTest.o \
      morseReceiver.o \
      morseCodec.o

GTKFILES=morse.o \
      morseReceiver.o \
      morseCodec.o

all: morseTest morse

morseTest: $(FILES)
	g++ $(CXXFLAGS) $^ -o $@

morse: CXXFLAGS := $(CXXFLAGS) $(shell pkg-config --cflags gtk+-3.0)
morse: $(GTKFILES)
	g++ $(CXXFLAGS) $(GTKLIBS) $^ -o $@

clean:
	rm -f $(FILES) $(GTKFILES) morseTest morse
