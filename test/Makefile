# File:         Makefile
# Maintainer:   Shintaro Kaneko <kaneshin0120@gmail.com>
# Last Change:  05-Sep-2012.

CC = gcc
CFLAGS = -Wall -O3
LBFGSSRCS = nlp_component.c\
			lbfgs.c
LSSRCS = linesearch.c
LBFGSOBJS = $(LBFGSSRCS:%.c=%.o)
LSOBJS = $(LSSRCS:%.c=%.o)
SRCDIR = ../lbfgs
LSDIR = linesearch
OBJDIR = bin
SRCS = $(patsubst %,$(SRCDIR)/%,$(LBFGSSRCS) $(LSSRCS))
OBJS = $(patsubst %,$(OBJDIR)/%,$(LBFGSOBJS) $(LSOBJS))

PROGS = test_nlp_component.o test_linesearch.o test_exmath.o

CUNIT = -I/usr/local/Cellar/cunit/2.1-2/include -L/usr/local/Cellar/cunit/2.1-2/lib

all: $(PROGS)

# test_exmath
# test_nlp_component
test_nlp_component.o: test_nlp_component.c $(OBJDIR)/nlp_component.o
	$(CC) $(CUNIT) $^ -lcunit -o $(OBJDIR)/$@

test_linesearch.o: test_linesearch.c $(OBJDIR)/linesearch.o $(OBJDIR)/nlp_component.o $(OBJDIR)/exmath.o
	$(CC) $(CUNIT) $^ -lcunit -o $(OBJDIR)/$@

test_exmath.o: test_exmath.c $(OBJDIR)/exmath.o
	$(CC) $(CUNIT) $^ -lcunit -o $(OBJDIR)/$@

# objects
$(OBJDIR)/nlp_component.o: $(SRCDIR)/nlp_component.c
	$(CC) -c $< -o $@

$(OBJDIR)/linesearch.o: $(SRCDIR)/linesearch/linesearch.c
	$(CC) -c $< -o $@

$(OBJDIR)/exmath.o: $(SRCDIR)/exmath/exmath.c
	$(CC) -c $< -o $@

# post-processor
clean:
	rm -vf $(patsubst %,$(OBJDIR)/%,$(PROGS)) $(OBJDIR)/*.o

