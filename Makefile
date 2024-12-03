CC = gcc
LD = $(CC)

CFLAGS = -Wall -Wextra -g -Isrc/include -std=gnu11 -MMD -MP -fsanitize=address

LIBS = -pthread -lasan -ldiscord -lcurl

CFILES = $(shell find src/ -name '*.c')
DEPENDS := $(patsubst src/%.c,build/%.d,$(CFILES))

OBJ = $(patsubst src/%.c,build/%.o,$(CFILES))
BIN = bin/wiibot

.PHONY: all clean
all: $(BIN)

-include $(DEPENDS)

$(BIN): $(OBJ)
	@mkdir -p $(@D)
	@$(info $S  LD $(patsubst build/%.o,%.o,$^) ==> $@)
	@$(LD) $(LDFLAGS) $^ $(LIBS) -o $@

build/%.o: src/%.c
	@mkdir -p $(@D)
	@$(info $S  CC $(patsubst src/%.c,%.c,$<) ==> $(patsubst build/%.o,%.o,$@))
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf bin build
