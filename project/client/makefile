target = client 
GCC = gcc 

vpath %.h ../header
OBJS = client.o
SRCS = $(OBJS, .o=.c)
HDRS = dataStruct.h globalVar.h
 
$(target) : $(OBJS)
	$(GCC) -o $(target) $(OBJS)

%.o : %.c
	$(GCC) -g -c $< 

$(SRCS) : $(HDRS)

clean:
	-rm *.o $(target)
 
