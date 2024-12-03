CC = gcc
LD = $(CC)

CFLAGS-COMMON = -Wall -Wextra -g -Isrc/frontend/include -std=gnu11 -MMD -MP -fsanitize=address
CFLAGS-FRONT = $(CFLAGS-COMMON)
CFLAGS-BACK = $(CFLAGS-COMMON)

LIBS-FRONT = -pthread -lasan -ldiscord -lcurl
LIBS-BACK = -lasan

CFILES-FRONT = $(shell find src/frontend/ -name '*.c')
DEPENDS-FRONT := $(patsubst src/frontend/%.c,build/frontend/%.d,$(CFILES-FRONT))

OBJ-FRONT = $(patsubst src/frontend/%.c,build/frontend/%.o,$(CFILES-FRONT))
BIN-FRONT = bin/wiibot

CFILES-BACK = $(shell find src/backend/ -name '*.c')
DEPENDS-BACK := $(patsubst src/backend/%.c,build/backend/%.d,$(CFILES-BACK))

OBJ-BACK = $(patsubst src/backend/%.c,build/backend/%.o,$(CFILES-BACK))
BIN-BACK = bin/wiibot-node

.PHONY: all clean
all: $(BIN-FRONT) $(BIN-BACK)

-include $(DEPENDS-FRONT)
-include $(DEPENDS-BACK)

$(BIN-BACK): $(OBJ-BACK)
	@mkdir -p $(@D)
	@$(info $S  LD $(patsubst build/backend/%.o,%.o,$^) ==> $@)
	@$(LD) $(LDFLAGS) $^ $(LIBS-BACK) -o $@

$(BIN-FRONT): $(OBJ-FRONT)
	@mkdir -p $(@D)
	@$(info $S  LD $(patsubst build/frontend/%.o,%.o,$^) ==> $@)
	@$(LD) $(LDFLAGS) $^ $(LIBS-FRONT) -o $@

build/backend/%.o: src/backend/%.c
	@mkdir -p $(@D)
	@$(info $S  CC $(patsubst src/backend/%.c,%.c,$<) ==> $(patsubst build/backend/%.o,%.o,$@))
	@$(CC) $(CFLAGS-FRONT) -c $< -o $@

build/frontend/%.o: src/frontend/%.c
	@mkdir -p $(@D)
	@$(info $S  CC $(patsubst src/frontend/%.c,%.c,$<) ==> $(patsubst build/frontend/%.o,%.o,$@))
	@$(CC) $(CFLAGS-FRONT) -c $< -o $@

clean:
	@rm -rf bin build
