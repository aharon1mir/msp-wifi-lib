
CC = msp430-gcc


CFLAGS := -c -I ../include/

Csrc := $(shell find ./  -iname \*.c)

Cobj := $(patsubst %.c,%.o,$(Csrc))

#Asrc := $(shell find ./ -type f -iname *.asm -o -iname *.s)
Asrc := $(shell find ./  -iname \*.s)
# Aobj := $(patsubst %.asm,%.o,$(Asrc))
Aobj := $(patsubst %.s,%.o,$(Asrc))

Hsrc := $(shell find ./  -iname \*.h)

all: $(Aobj) $(Cobj) $(Hsrc)
	$(CC) $(Aobj) $(Cobj) -o main

$(Aobj): $(Asrc) $(Hsrc)
	$(CC) $(CFLAGS) $(patsubst %.o,%.s,$@) -o $@

$(Cobj): $(Csrc) $(Hsrc)
	$(CC) $(CFLAGS) $(patsubst %.o,%.c,$@) -o $@


msp: all
	@echo "[*] Done Compiling!"

upload: msp
	@echo "[*] Uploading..."
	echo "prog ./main" | mspdebug rf2500 -n
	@echo "[*] Done Uploading!"	

clean:
	/bin/rm $(shell find ./ -iname \*.o) ./main

debug: all main
	msp430-gdb ./main

simulate: all main
		mspdebug sim 
