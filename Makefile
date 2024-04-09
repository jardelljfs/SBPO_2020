CFLAGS= -Wall -m64 -g -w
CXX= g++
#HOME=/home/jardell/Musica
ILOG=/opt/ibm/ILOG/CPLEX_Studio129
CPPFLAGS= -DIL_STD -I$(ILOG)/cplex/include -I$(ILOG)/concert/include
CPLEXLIB= -L$(ILOG)/cplex/lib/x86-64_linux/static_pic -lilocplex -lcplex -L$(ILOG)/concert/lib/x86-64_linux/static_pic -lconcert -lm -lpthread -ldl
comp1:  
	$(CXX) $(CFLAGS) $(CPPFLAGS) -o phcp pHCP.cpp $(CPLEXLIB)
