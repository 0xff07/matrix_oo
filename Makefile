EXEC = \
    tests/test-matrix \
    tests/test-stopwatch

GIT_HOOKS := .git/hooks/applied
OUT ?= .build
.PHONY: all
all: $(GIT_HOOKS) $(OUT) $(EXEC)

$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

CC ?= gcc
CFLAGS = -Wall -mavx -mavx2 -mfma -std=gnu99 -g -O2 -I.
LDFLAGS = -lpthread

OBJS := \
	stopwatch.o \
	matrix_naive.o

deps := $(OBJS:%.o=%.o.d)
OBJS := $(addprefix $(OUT)/,$(OBJS))
deps := $(addprefix $(OUT)/,$(deps))

tests/test-%: $(OBJS) tests/test-%.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OUT)/%.o: %.c $(OUT)
	$(CC) $(CFLAGS) -c -o $@ -MMD -MF $@.d $<

$(OUT):
	@mkdir -p $@

check: $(EXEC)
	@for test in $^ ; \
	do \
		echo "Execute $$test..." ; $$test && echo "OK!\n" ; \
	done

gendata: $(EXEC)
	echo "naive row_major avx_row_major" > "fix-sized-time.txt";\
	for i in `seq 1 1 100`; \
	do \
		tests/test-matrix >> "fix-sized-time.txt";\
	done

plot: gendata
	gnuplot scripts/plot/plot.gp

clean:
	$(RM) $(EXEC) $(OBJS) $(deps)
	@rm -rf $(OUT)
	rm -f *.txt *.png

-include $(deps)
