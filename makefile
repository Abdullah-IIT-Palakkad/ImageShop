LIBDIR := staticlib
DLIBDIR:= dynamiclib
OBJDIR := obj
DOBJDIR:= Dobj
EXEDIR := binary
TESTDIR:= test
RESULT := result

EXE	   := $(EXEDIR)/ImageShop
DEXE   := $(EXEDIR)/DImageShop
LIBS   := $(LIBDIR)/libmatrixmath.a
DLIB   := $(DLIBDIR)/libmatrixmath.so
SRCS   := $(wildcard src/*.c)
OBJS   := $(addprefix $(OBJDIR)/,$(patsubst src/%.c,%.o,$(SRC)))
DOBJS  := $(addprefix $(DOBJDIR)/,$(patsubst src/%.c,%.o,$(SRC)))

INPUT  := ./input.ppm
OUTPUT := ./output.ppm

vpath %.c src
vpath %.h include

all: build
	@echo "Finished Successfully"

build: $(EXE)
	@echo "Compiled Successfully"

buildso: $(DEXE)
	@echo "Compiled Successfully using Dynamic Library"

$(EXE): obj/main.o obj/image.o $(LIBS) | $(EXEDIR)
	gcc -o $@ -I include $< $(word 2,$^) -L $(LIBDIR) -lmatrixmath

$(DEXE): Dobj/matrix.o Dobj/image.o Dobj/main.o main.c image.c $(DLIB) | $(EXEDIR)
	gcc -I include $(word 4,$^) $(word 5,$^) -L $(DLIBDIR) -lmatrixmath -o $@

$(OBJDIR)/%.o: %.c image.h matrix.h | $(OBJDIR)
	gcc -o $@ -c -I include $<

$(DOBJDIR)/%.o: %.c image.h matrix.h | $(DOBJDIR)
	gcc -fPIC -c -I include $< -o $@

$(LIBDIR)/libmatrixmath.a: obj/matrix.o | $(LIBDIR)
	ar rcs $@ $^

$(DLIBDIR)/libmatrixmath.so: Dobj/matrix.o | $(DLIBDIR)
	gcc -shared -Wl,-soname,libmatrixmath.so -o $@ $(DOBJDIR)/matrix.o

$(OBJDIR):
	mkdir -p $@

$(DOBJDIR):
	mkdir -p $@

$(LIBDIR):
	mkdir -p $@

$(DLIBDIR):
	mkdir -p $@

$(EXEDIR):
	mkdir -p $@

$(RESULT):
	mkdir -p $@

run:
	./$(EXE) $(INPUT) $(OUTPUT) 1 1

runso:
	@LD_LIBRARY_PATH="./dynamiclib" ./$(DEXE) $(INPUT) $(OUTPUT) 1 1

prepare:
	@echo "Enter a valid input file path including the file"; \
	read INPUT; \
	convert -compress none "$$INPUT" input.ppm
	@echo "Enter a valid output file path"; \
	read OUTPUT; \
	make run; \
	mv output.ppm "$$OUTPUT"
	
tests: 
	$(foreach file, $(wildcard $(TESTDIR)/*),convert -compress none ${file} input.ppm; make run ; cat output.ppm >> result/$(basename $(notdir ${file})).ppm;)

test%:
	convert -compress none $(shell find $(TESTDIR) -name 'test$.') input.ppm; \
	make run; \
	cat output.ppm >> $(RESULT)/$(basename $(notdir ${shell find $(TESTDIR) -name 'test$.'})).ppm

# Credit for tests and test% : Rajiv Goyal

T1:
	./$(EXE) $(INPUT) $(OUTPUT) 1 0

T2:
	./$(EXE) $(INPUT) $(OUTPUT) 0 1


.PHONY: clean T1 T2 prepare run build buildso tests test%
clean:
	rm -rf $(EXEDIR) $(OBJDIR) $(LIBDIR) $(DLIBDIR) $(DOBJDIR) output.ppm $(RESULT)/