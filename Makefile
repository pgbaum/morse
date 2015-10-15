CXXFLAGS += -g -Wall -pedantic -std=c++11

FILES=morseTest.o \
      morseReceiver.o \
      morseCodec.o

morseTest: $(FILES)
	g++ $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(FILES) morseTest
