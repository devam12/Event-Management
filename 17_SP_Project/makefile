# create string with obj/<object filename> in our case obj/pow.o and obj/fact.o  (obj folder will be created under current
# i.e. source directory)
_LIBOBJ = generatebill.o storedata.o viewevent.o
LIBOBJ = $(patsubst %,$(ODIR)/%,$(_LIBOBJ))
LIBSRC = generatebill.c storedata.c viewevent.c
_APPOBJ = main.o
APPOBJ = $(patsubst %,$(ODIR)/%,$(_APPOBJ))

LDFLAGS= -lpthread
$(ODIR)/generatebill.o: generatebill.c
	$(CC) -c -fPIC -o $@ $< $(CFLAGS)
	
$(ODIR)/storedata.o: storedata.c 
	$(CC) -c -fPIC -o $@ $< $(CFLAGS)
	
$(ODIR)/viewevent.o: viewevent.c 
	$(CC) -c -fPIC -o $@ $< $(CFLAGS)

$(LDIR)/libbillfile.a:$(LIBOBJ)
	$(CC) -shared -o $@ $^

$(APPOBJ): main.c $(DEPS) 
	$(CC) -o $@ $< $(CFLAGS) 
	
main.out: $(APPOBJ) $(LDIR)/libbillfile.a
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) $(LDIR)/libbillfile.a
	
.PHONY: clean

# removes object files and temporary files created with ~ at the end of file name in current i.e. source and include directory

clean: 
	rm -f $(ODIR)/.o *~ core $(INCDIR)/~
	rm -f main.out $(LDIR)/libbillfile.so
	
.PHONY: all
all: $(LDIR)/libbillfile.a main.out
