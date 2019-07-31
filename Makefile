
EXCEC = game

SRCDIR  = src 
OBJDIR  = obj
CC = gcc

SRCS    := $(shell find $(SRCDIR) -name '*.c')
HDRS    := $(shell find $(SRCDIR) -name '*.h')
SRCDIRS := $(shell find . -name '*.c' -exec dirname {} \; | uniq)
OBJS    := $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))

CFLAGS := -Wall -ggdb3

LDFLAGS := `sdl2-config --libs --cflags` -lSDL2_image -lSDL2_ttf -lm


all: $(EXCEC)

$(EXCEC) : buildrepo $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

$(OBJDIR)/%.o: %.c $(HDRS)
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	$(RM) $(OBJS)

distclean: clen
	$(RM) $(EXCEC)
	
buildrepo:
	@$(call make-repo)

define make-repo
   for dir in $(SRCDIRS); \
   do \
	mkdir -p $(OBJDIR)/$$dir; \
   done
endef
