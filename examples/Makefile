CFLAGS = -Wall -I$(HOME)/include
LDFLAGS = -L$(HOME)/lib
LDLIBS = -lstk

SAMPLES = list_eg stk_eg

.PHONY: samples clean

samples: $(SAMPLES)

clean:
	rm -f $(SAMPLES) *.o
