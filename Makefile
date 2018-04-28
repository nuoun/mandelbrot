DEBUG ?= 0
WARNING ?= 0
CC = cc
CFLAGS = -std=gnu99 -fopenmp
RFLAGS = -O3
DFLAGS = -O0 -DDebug -g
RWARNINGS = -Werror -Wall -Wextra -Wpedantic
DWARNINGS = -Wabi -Waggregate-return -Waggressive-loop-optimizations -Wall -Wattributes -Wbad-function-cast -Wbuiltin-macro-redefined \
-Wc++-compat -Wcast-align -Wcast-qual -Wconversion -Wcoverage-mismatch -Wcpp \
-Wdate-time -Wdeprecated -Wdeprecated-declarations -Wdesignated-init -Wdisabled-optimization \
-Wdiscarded-array-qualifiers -Wdiscarded-qualifiers -Wdiv-by-zero -Wdouble-promotion -Wextra -Wfloat-equal -Wformat-signedness -Wframe-larger-than=32768 \
-Wfree-nonheap-object -Wimport -Wincompatible-pointer-types -Winline -Wint-conversion -Wint-to-pointer-cast -Winvalid-memory-model \
-Winvalid-pch -Wjump-misses-init -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wmissing-prototypes -Wmultichar \
-Wnested-externs -Wnormalized -Wodr -Wold-style-definition -Woverflow -Wpacked \
-Wpacked-bitfield-compat -Wpadded -Wpedantic -Wpointer-to-int-cast -Wpragmas -Wredundant-decls \
-Wreturn-local-addr -Wshadow -Wshift-count-negative -Wshift-count-overflow -Wsizeof-array-argument -Wstack-protector -Wstrict-aliasing \
-Wstrict-overflow -Wstrict-prototypes -Wsuggest-attribute=const -Wsuggest-attribute=format -Wsuggest-attribute=noreturn \
-Wsuggest-attribute=pure -Wsuggest-final-methods -Wsuggest-final-types -Wswitch-bool -Wswitch-default -Wswitch-enum -Wsync-nand \
-Wtrampolines -Wundef -Wunreachable-code -Wunsafe-loop-optimizations \
-Wunused-macros -Wunused-result -Wvarargs -Wvector-operation-performance -Wvla -Wwrite-strings 

CPPWARNINGS = -Wabi-tag -Wassign-intercept -Wconditionally-supported -Wconversion-null -Wctor-dtor-privacy -Wdelete-incomplete -Weffc++ -Winherited-variadic-ctor \
-Winvalid-offsetof -Wliteral-suffix -Wnoexcept -Wnon-template-friend -Wold-style-cast -Woverloaded-virtual -Wpmf-conversions -Wproperty-assign-default -Wprotocol \
-Wselector -Wsign-promo -Wstrict-null-sentinel -Wstrict-selector-match -Wsuggest-override -Wsynth -Wundeclared-selector -Wuseless-cast -Wvirtual-move-assign \
-Wzero-as-null-pointer-constant

LDLIBS = -lm
TARGET = mandelbrot

ifeq ($(DEBUG), 1)
    CFLAGS += $(DFLAGS)
else
    CFLAGS += $(RFLAGS)
endif

ifeq ($(WARNING), 1)
    CFLAGS += $(DWARNINGS)
else
    CFLAGS += $(RWARNINGS)
endif

SRCDIR = ./src
OBJDIR = ./obj

SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(addprefix $(OBJDIR)/,$(notdir $(SRC:.c=.o)))

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)
	@echo "Linking complete"

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $(CFLAGS) $< -o $@
	@echo "Compilation complete"

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJ)
	@echo "Cleanup complete"
