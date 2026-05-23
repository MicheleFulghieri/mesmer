# --- variables defined by configure ---

SRC = ./src
QUADSUFFIX = 
PREFIX = .
LIBDIRSUFFIX = 64

EXE = 
DEF = -D
NOUNDERSCORE = 0
QUAD = 0
QUADSIZE = 16

FC = /data/mfulghieri/anaconda3/envs/mesmer/bin/x86_64-conda-linux-gnu-gfortran
FFLAGS = -O3 -ffast-math -fPIC -ffixed-line-length-none -fPIC -fno-range-check \
  $(DEF)QUAD=$(QUAD) $(DEF)QUADSIZE=$(QUADSIZE) \
  $(DEF)U77EXT=0

CDEFS = -DQUAD=$(QUAD) -DQUADSIZE=$(QUADSIZE) \
  -DNOUNDERSCORE=$(NOUNDERSCORE) \
  -DBIGENDIAN=0

CC = gcc
CFLAGS = -march=nocona -mtune=haswell -ftree-vectorize -fPIC -fstack-protector-strong -fno-plt -O2 -ffunction-sections -pipe -isystem /data/mfulghieri/anaconda3/envs/mesmer/include -m64

CXX = /data/mfulghieri/anaconda3/envs/mesmer/bin/x86_64-conda-linux-gnu-c++
CXXFLAGS = -fvisibility-inlines-hidden -fmessage-length=0 -march=nocona -mtune=haswell -ftree-vectorize -fPIC -fstack-protector-strong -fno-plt -O2 -ffunction-sections -pipe -isystem /data/mfulghieri/anaconda3/envs/mesmer/include -m64 -stdlib=libstdc++

ML = 0
MCC = mcc
MCDEFS = -D__int64=long\ long\ int
MCFLAGS = -n
MCLIBS = -lpthread -lrt

LDFLAGS = -Wl,-O2 -Wl,--sort-common -Wl,--as-needed -Wl,-z,relro -Wl,-z,now -Wl,--disable-new-dtags -Wl,--gc-sections -Wl,--allow-shlib-undefined -Wl,-rpath,/data/mfulghieri/anaconda3/envs/mesmer/lib -Wl,-rpath-link,/data/mfulghieri/anaconda3/envs/mesmer/lib -L/data/mfulghieri/anaconda3/envs/mesmer/lib -L/data/mfulghieri/anaconda3/envs/mesmer/bin/../libexec/gcc/x86_64-conda-linux-gnu/14.3.0/liblto_plugin.so -L/data/mfulghieri/anaconda3/envs/mesmer/bin/../lib/gcc/x86_64-conda-linux-gnu/14.3.0 -L/data/mfulghieri/anaconda3/envs/mesmer/bin/../lib/gcc -L/data/mfulghieri/anaconda3/envs/mesmer/bin/../lib/gcc/x86_64-conda-linux-gnu/14.3.0/../../../../x86_64-conda-linux-gnu/lib/../lib -L/data/mfulghieri/anaconda3/envs/mesmer/bin/../lib/gcc/x86_64-conda-linux-gnu/14.3.0/../../../../lib -L/data/mfulghieri/anaconda3/envs/mesmer/bin/../x86_64-conda-linux-gnu/sysroot/lib/../lib -L/data/mfulghieri/anaconda3/envs/mesmer/bin/../x86_64-conda-linux-gnu/sysroot/usr/lib/../lib -L/data/mfulghieri/anaconda3/envs/mesmer/bin/../lib/gcc/x86_64-conda-linux-gnu/14.3.0/../../../../x86_64-conda-linux-gnu/lib -L/data/mfulghieri/anaconda3/envs/mesmer/bin/../lib/gcc/x86_64-conda-linux-gnu/14.3.0/../../.. -L/data/mfulghieri/anaconda3/envs/mesmer/bin/../x86_64-conda-linux-gnu/sysroot/lib -L/data/mfulghieri/anaconda3/envs/mesmer/bin/../x86_64-conda-linux-gnu/sysroot/usr/lib -lgfortran -lm -lgcc_s -lgcc -lquadmath -lm -lgcc_s -lgcc -Wl,-rpath,/data/mfulghieri/anaconda3/envs/mesmer/lib -Wl,-rpath-link,/data/mfulghieri/anaconda3/envs/mesmer/lib -L -L/data/mfulghieri/anaconda3/envs/mesmer/lib/stubs -L -L/data/mfulghieri/anaconda3/envs/mesmer/lib

PARALLEL = -j 48

# --- end defs by configure ---

BLD = build$(QUADSUFFIX)

LIBDIR = $(PREFIX)/lib$(LIBDIRSUFFIX)
INCLUDEDIR = $(PREFIX)/include
BINDIR = $(PREFIX)/bin

LIB = libooptools$(QUADSUFFIX).a
FE = lt$(QUADSUFFIX)$(EXE)
MFE = LoopTools$(QUADSUFFIX)$(EXE)
FCC = fcc$(QUADSUFFIX)
FXX = f++$(QUADSUFFIX)
INCLUDE = $(BLD)/looptools.h $(BLD)/clooptools.h

ARGS = $(PARALLEL) \
  LIB="$(LIB)" \
  FE="$(FE)" \
  MFE="$(MFE)" \
  FCC="$(FCC)" \
  FXX="$(FXX)" \
  EXE="$(EXE)" \
  DEF="$(DEF)" \
  NOUNDERSCORE="$(NOUNDERSCORE)" \
  XFC="$(FC) $(FFLAGS) -I." \
  F90="$(F90)" \
  CDEFS="$(CDEFS)" \
  MCDEFS="$(MCDEFS)" \
  CC="$(CC)" \
  CFLAGS="$(CFLAGS)" \
  CXX="$(CXX)" \
  CXXFLAGS="$(CXXFLAGS)" \
  ML="$(ML)" \
  MCC="$(MCC)" \
  MCFLAGS="$(MCFLAGS)" \
  MCLIBS="$(MCLIBS)" \
  AR="$(AR)" \
  RANLIB="$(RANLIB)" \
  NM="$(NM)" \
  DLLTOOL="$(DLLTOOL)" \
  LDFLAGS="$(LDFLAGS)" \
  LIBPATH="$(LIBPATH)"


default all lib frontend mma: force
	cd $(BLD) && $(MAKE) $(ARGS) $@

install: lib frontend
	-mkdir $(PREFIX)
	-mkdir $(LIBDIR) $(BINDIR) $(INCLUDEDIR)
	cp -p $(BLD)/$(LIB) $(LIBDIR)
	cp -p $(INCLUDE) $(INCLUDEDIR)
	strip $(BLD)/$(FE)
	cp -p $(BLD)/$(FCC) $(BLD)/$(FE) $(BINDIR)
	rm -f $(BINDIR)/$(FXX)
	ln -s $(FCC) $(BINDIR)/$(FXX)
	test ! -f $(BLD)/$(MFE) || { \
	  test -z "$(EXE)" || ldd $(BLD)/$(MFE) | awk '/\/usr\/bin\/cyg/ { system("cp -p " $$3 " $(BINDIR)/"); }' ; \
	  strip $(BLD)/$(MFE) ; \
	  cp -p $(BLD)/$(MFE) $(BINDIR); \
	}

force: $(BLD)/timestamp

$(BLD)/timestamp:
	-mkdir $(BLD)
	find "`cd $(SRC) ; pwd`" -follow -exec ln -sf {} $(BLD) \;
	touch $(BLD)/timestamp

clean:
	rm -fr $(BLD)

