


CXX = c++
CC = cc

CFLAGS += -g
CXXFLAGS += -g
LXXOPT += -L../lib -lATSApi 

WSSERVER_FILES = wsServer/base64.o wsServer/sha1.o  wsServer/utf8.o \
				 wsServer/handshake.o wsServer/ws.o
CJSON_FILES = cJSON/cJSON.o  cJSON/cJSON_Utils.o
QRND_FILES = qrnd-core.o qrnd-base.o qrnd-disk.o qrnd-measurement.o qrnd-ui.o \
			 gaussian.o


all: ats-tools qrnd.a node_defs.js qrnd-test

qrnd-core.o: qrnd-core.cpp qrnd-core.h
	$(CXX) $(CXXFLAGS) -c qrnd-core.cpp

qrnd-disk.o: qrnd-disk.cpp qrnd-disk.h qrnd-core.h
	$(CXX) $(CXXFLAGS) -c qrnd-disk.cpp

qrnd-ui.o: qrnd-ui.cpp qrnd-ui.h qrnd-core.h wsServer/ws.h cJSON/cJSON.h
	$(CXX) $(CXXFLAGS) -c qrnd-ui.cpp

qrnd-base.o: qrnd-base.cpp qrnd-base.h qrnd-core.h
	$(CXX) $(CXXFLAGS) -c qrnd-base.cpp

qrnd-measurement.o: qrnd-measurement.cpp qrnd-measurement.h qrnd-core.h
	$(CXX) $(CXXFLAGS) -c qrnd-measurement.cpp

gaussian.o: gaussian.cpp gaussian.h qrnd-core.h
	$(CXX) $(CXXFLAGS) -c gaussian.cpp


fauchinger.o: fauchinger.cpp fauchinger.h qrnd-core.h
	$(CXX) $(CXXFLAGS) -c fauchinger.cpp

von_neumann.o: von_neumann.cpp von_neumann.h qrnd-core.h
	$(CXX) $(CXXFLAGS) -c von_neumann.cpp

qrnd.a: $(QRND_FILES) $(CJSON_FILES) $(WSSERVER_FILES)
	$(AR) rcs qrnd.a $(QRND_FILES) $(CJSON_FILES) $(WSSERVER_FILES)

qrnd-test: $(QRND_FILES) qrnd-test.o
	$(CXX) $(CXXFLAGS) -o qrnd-test \
		$(QRND_FILES) qrnd-test.o \
		$(LXXOPT) qrnd.a

node_defs.js: ../composer/htdocs/node_defs.js
../composer/htdocs/node_defs.js: make-ui-def.js qrnd-ui-nodes.h
	node  make-ui-def.js >../composer/htdocs/node_defs.js


std-seed.o: std-seed.c
	$(CC) -c std-seed.c

std-seed.c: bin2seed std-seed.bin
	./bin2seed std-seed.bin >std-seed.c

bin2seed: bin2seed.c
	$(CC) -o bin2seed bin2seed.c




ats-tools: ls-ats ats-capture ATS-spy.so

ls-ats: ls-ats.o ATS-SDK.o
	$(CXX) $(CXXFLAGS) -o ls-ats ls-ats.o ATS-SDK.o $(LXXOPT)

ats-capture: ats-capture.o std-seed.o ATS-SDK.o
	$(CXX) $(CXXFLAGS) -o ats-capture ats-capture.o std-seed.o ATS-SDK.o $(LXXOPT)

ls-ats.o: ls-ats.cpp ATS-SDK.h
	$(CXX) $(CXXFLAGS) -c ls-ats.cpp

ats-capture.o: ats-capture.cpp ATS-SDK.h
	$(CXX) $(CXXFLAGS) -c ats-capture.cpp

ATS-spy.so: ATS-spy.c
	cc -o ATS-spy.so -g ATS-spy.c -shared -fPIC -DPIC -ldl

ATS-SDK.o: ATS-SDK.cpp ATS-SDK.h
	$(CXX) $(CXXFLAGS) -c ATS-SDK.cpp


clean: 
	rm -f *.o ls-ats ats-capture ATS-spy.so bin2seed std-seed.c
	rm -f qrnd.a 

dist-clean: clean




wsServer/base64.o: wsServer/base64.c wsServer/base64.h
wsServer/sha1.o: wsServer/sha1.c wsServer/sha1.h 
wsServer/utf8.o: wsServer/utf8.c wsServer/utf8.h
wsServer/handshake.o: wsServer/handshake.c wsServer/ws.h wsServer/base64.h wsServer/sha1.h
wsServer/ws.o: wsServer/ws.c wsServer/ws.h wsServer/utf8.h

cJSON/cJSON.o: cJSON/cJSON.c cJSON/cJSON.h
cJSON/cJSON_Utils.o: cJSON/cJSON_Utils.c cJSON/cJSON_Utils.h
