CXXFLAGS += -g -Wall -pedantic -std=c++11
GTKLIBS  = $(shell pkg-config --libs gtk+-3.0 gstreamer-1.0)
GTKFLAGS = $(shell pkg-config --cflags gtk+-3.0 gstreamer-1.0)

MORSEFILES= morseCodec.o \
      morseReceiver.o \
      morseTransmitter.o

FILES=morseTest.o $(MORSEFILES)

GTKFILES=morse.o $(MORSEFILES) \
      echoServer.o \
      eventServer.o \
      morseGdkTransmitter.o \
      receiverServer.o

all: morseTest morse

morseTest: $(FILES)
	g++ $(CXXFLAGS) $^ -o $@

morse: CXXFLAGS := $(CXXFLAGS) $(GTKFLAGS)
morse: $(GTKFILES)
	g++ $(CXXFLAGS) $(GTKLIBS) $^ -o $@

clean:
	rm -f $(FILES) $(GTKFILES) morseTest morse
