# MeowMeow - A stream encoder/decoder

ENCODER= meow
DECODER= unmeow

TARGETS= $(ENCODER) $(DECODER)

SRC= main.c mmencode.c mmdecode.c
OBJ= $(SRC:.c=.o)
HDR= $(SRC:.c=.h)

MANIFEST= $(SRC) $(HDR) README.md Makefile
VERSION = 0.1.0
SDIST_ROOT= meowmeow
SDIST_TARFILE= $(SDIST_ROOT)-$(VERSION).tar.gz


CC= cc
CP= cp
LN= ln
MKDIR= mkdir
RM= rm
TAR= tar

CFLAGS= -g
LDFLAGS=

.PHONY: tests test_encoder test_decoder
.PHONY: sdist

all: $(TARGETS)

$(ENCODER): $(OBJ)
	$(CC) -o $@ $(OBJ) $(CFLAGS) $(LDFLAGS)

.c.o: $(HDR)
	$(CC) -c $< $(CFLAGS)

$(DECODER): $(ENCODER)
	$(LN) -f $< $@


tests: test_encoder test_decoder

test_encoder: $(ENCODER)
	@echo "XXX encoder testing here"

test_decoder: $(DECODER)
	@echo "XXX decoder testing here"


sdist: clobber
	$(MKDIR) -p $(SDIST_ROOT)
	$(CP) $(MANIFEST) $(SDIST_ROOT)
	$(TAR) zcf $(SDIST_TARFILE) $(SDIST_ROOT)

clean:
	@$(RM) -f $(OBJ) *~


clobber: clean
	@$(RM) -f $(TARGETS) $(SDIST_TARFILE)
	@$(RM) -rf $(SDIST_ROOT)
