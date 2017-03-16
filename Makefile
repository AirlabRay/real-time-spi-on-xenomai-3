target = spi
skin1 = rtdm
skin2 = native
xeno_config =/usr/xenomai/bin/xeno-config

CC := $(shell $(xeno_config) --cc)
CFLAGS := $(shell $(xeno_config) --rtdm --native --cflags)
LDFLAGS := $(shell $(xeno_config) --rtdm --native --ldflags)

spi: main.c $(target).c
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	@rm $(target)

