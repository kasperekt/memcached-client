CC=clang
CFLAGS=-Wall

SRCDIR=src
OBJDIR=obj
SRC=main.c
OBJ=$(addprefix $(OBJDIR)/,$(SRC:.c=.o))
EXE=mc_client

all: clean prepare $(EXE)

run: prepare $(EXE)
	./$(EXE) 127.0.0.1 1234 file1.jpg

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

lint:
	splint $(SRCDIR)/*.c

prepare:
	if [ ! -d $(OBJDIR) ]; then mkdir $(OBJDIR); fi

clean:
	if [ -d $(OBJDIR) ]; then rm -r $(OBJDIR); fi
	if [ -f $(EXE) ]; then rm $(EXE); fi