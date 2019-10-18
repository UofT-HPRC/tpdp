CC = aarch64-linux-gnu-g++
C_FLAGS = -std=c++0x
SOURCE = src
HEADERS = include
OUTDIR = output

main: assemble clean

assemble: xvc_axi2pl xvc_server2axi xvc_server
	aarch64-linux-gnu-g++ $(OUTDIR)/xvc_axi2pl.o $(OUTDIR)/xvc_server2axi.o $(OUTDIR)/xvc_server.o -o $(OUTDIR)/xvcserver

xvc_axi2pl: $(SOURCE)/xvc_axi2pl.cpp $(HEADERS)/xvc_axi2pl.h outputdir
	aarch64-linux-gnu-g++ -c $(C_FLAGS) -I$(HEADERS) $(SOURCE)/xvc_axi2pl.cpp -o $(OUTDIR)/xvc_axi2pl.o 

xvc_server2axi: $(SOURCE)/xvc_server2axi.c $(HEADERS)/xvc_axi2pl.h $(HEADERS)/xvcserver.h outputdir
	aarch64-linux-gnu-g++ -c $(C_FLAGS) -I$(HEADERS) $(SOURCE)/xvc_server2axi.c -o $(OUTDIR)/xvc_server2axi.o

xvc_server: $(SOURCE)/xvc_server.c $(HEADERS)/xvcserver.h outputdir
	aarch64-linux-gnu-g++ -c $(C_FLAGS) -I$(HEADERS) $(SOURCE)/xvc_server.c -o $(OUTDIR)/xvc_server.o

outputdir:
	mkdir -p $(OUTDIR)

clean:
	rm -f $(OUTDIR)/*.o

clean_all:
	rm -f $(OUTDIR)/*
