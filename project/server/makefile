target = server
GCC = gcc 

vpath %.h ../header
OBJS = server.o
SRCS = $(OBJS, .o=.c)
HDRS = dataStruct.h globalVar.h threadFunc.h fileRW.h
 
$(target) : $(OBJS)
	$(GCC) -o $(target) $(OBJS)

%.o : %.c
	$(GCC) -g -c $< 

$(SRCS) : $(HDRS)

clean:
	-rm *.o $(target)
 
