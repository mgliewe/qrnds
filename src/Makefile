

CXXOPT += -g -fPIC
LXXOPT += -L../lib -lATSApi


QRND_FILES = qrnd-core.o qrnd-disk.o fauchinger.o

all: ats-tools qrnd.so # qrnd-test

qrnd-core.o: qrnd-core.cpp qrnd-core.h
	c++ $(CXXOPT) -c qrnd-core.cpp

qrnd-disk.o: qrnd-disk.cpp qrnd-disk.h qrnd-core.h
	c++ $(CXXOPT) -c qrnd-disk.cpp

fauchinger.o: fauchinger.cpp fauchinger.h qrnd-core.h
	c++ $(CXXOPT) -c fauchinger.cpp

von_neumann.o: von_neumann.cpp von_neumann.h qrnd-core.h
	c++ $(CXXOPT) -c von_neumann.cpp

qrnd.so: $(QRND_FILES)
	c++ $(CXXOPT) -o qrnd.so -shared \
		$(QRND_FILES) \
		$(LXXOPT)


qrnd-test: $(QRND_FILES) qrnd-test.o
	c++ $(CXXOPT) -o qrnd-test \
		$(QRND_FILES) qrnd-test.o \
		$(LXXOPT)

std-seed.o: std-seed.c
	cc -c std-seed.c
std-seed.c: bin2seed std-seed.bin
	./bin2seed std-seed.bin >std-seed.c
bin2seed: bin2seed.c
	cc -o bin2seed bin2seed.c

ats-tools: ls-ats ats-capture ATS-spy.so

ls-ats: ls-ats.o ATS-SDK.o
	c++ $(CXXOPT) -o ls-ats ls-ats.o ATS-SDK.o $(LXXOPT)

ats-capture: ats-capture.o std-seed.o ATS-SDK.o
	c++ $(CXXOPT) -o ats-capture ats-capture.o std-seed.o ATS-SDK.o $(LXXOPT)

ls-ats.o: ls-ats.cpp ATS-SDK.h
	c++ $(CXXOPT) -c ls-ats.cpp

ats-capture.o: ats-capture.cpp ATS-SDK.h
	c++ $(CXXOPT) -c ats-capture.cpp

ATS-spy.so: ATS-spy.c
	cc -o ATS-spy.so -g ATS-spy.c -shared -fPIC -DPIC -ldl

ATS-SDK.o: ATS-SDK.cpp ATS-SDK.h
	c++ $(CXXOPT) -c ATS-SDK.cpp


clean: 
	rm -f *.o ls-ats ats-capture ATS-spy.so bin2seed std-seed.c
