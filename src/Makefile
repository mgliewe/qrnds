

CXXOPT += -g -O3
LXXOPT += -L../lib -lATSApi


all: ats-tools

qrnd-core.o: qrnd-core.cpp qrnd-core.h
	c++ $(CXXOPT) -c qrnd-core.cpp

fauchinger.o: fauchinger.cpp fauchinger.h qrnd-core.h
	c++ $(CXXOPT) -c fauchinger.cpp

von_neumann.o: von_neumann.cpp von_neumann.h qrnd-core.h
	c++ $(CXXOPT) -c von_neumann.cpp

qrnd.so: qrnd-core.o fauchinger.o von_neumann.o
	c++ $(CXXOPT) -o qrnd.so -shared \
		qrnd-core.o fauchinger.o von_neumann.o \ 
		$(LXXOPT)


ats-tools: ls-ats ats-capture

ls-ats: ls-ats.o ATS-SDK.o
	c++ $(CXXOPT) -o ls-ats ls-ats.o ATS-SDK.o $(LXXOPT)

ats-capture: ats-capture.o ATS-SDK.o
	c++ $(CXXOPT) -o ats-capture ats-capture.o ATS-SDK.o $(LXXOPT)

ls-ats.o: ls-ats.cpp ATS-SDK.h
	c++ $(CXXOPT) -c ls-ats.cpp

ATS-SDK.o: ATS-SDK.cpp ATS-SDK.h
	c++ $(CXXOPT) -c ATS-SDK.cpp


clean: 
	rm -f *.o ls-ats ats-capture qrnd-core.o