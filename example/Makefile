# File:         Makefile
# Maintainer:   Shintaro Kaneko <kaneshin0120@gmail.com>
# Last Change:  05-Sep-2012.

CC = gcc
CFLAGS = -Wall -O3
LBFGSSRCS = nlp_component.c\
			lbfgs.c
LSSRCS = linesearch.c\
		 backtracking_wolfe.c
LBFGSOBJS = $(LBFGSSRCS:%.c=%.o)
LSOBJS = $(LSSRCS:%.c=%.o)
SRCDIR = ../lbfgs
LSDIR = linesearch
OBJDIR = bin
SRCS = $(patsubst %,$(SRCDIR)/%,$(LBFGSSRCS) $(LSSRCS))
OBJS = $(patsubst %,$(OBJDIR)/%,$(LBFGSOBJS) $(LSOBJS))

PROGS = driver1 driver4 driver7

all: $(PROGS)

# drivers
driver%: $(OBJDIR)/driver%.o $(OBJS) $(OBJDIR)/exmath.o
	$(CC) $(CFLAGS) $^ -o $(OBJDIR)/$@ -lm

# objects
$(OBJDIR)/driver%.o: driver%.c
	$(CC) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/$(LSDIR)/%.c
	$(CC) -c $< -o $@

$(OBJDIR)/exmath.o: $(SRCDIR)/exmath/exmath.c
	$(CC) -c $< -o $@

# post-processor
clean:
	rm -vf $(patsubst %,$(OBJDIR)/%,$(PROGS)) $(OBJDIR)/*.o

