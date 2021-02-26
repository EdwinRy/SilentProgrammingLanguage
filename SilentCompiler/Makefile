CC=gcc
OBJDIR=./obj
OUTDIR=./out
SRCDIR=./src
SOURCES = $(wildcard $(SRCDIR)/*.c)
IDIR=./include
EXEID = SilentC
INCLUDES=-I $(IDIR) 

CFLAGS = 
export CFLAGS

$(info $(shell mkdir -p $(OBJDIR) $(OUTDIR)))

OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES)) 

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) -c -o $@ $< $(INCLUDES) $(CFLAGS)

$(EXEID): $(OBJS)
	$(CC) -o $(OUTDIR)/$@ $^ 

all: $(EXEID)

.PHONY: clean
clean: 
	rm -rf out
	rm -rf obj