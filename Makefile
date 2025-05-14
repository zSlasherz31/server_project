CC       = gcc
CFLAGS   = -Wall -Wextra
SRCS     = main.c mongoose/mongoose.c input/input.c output/output.c
SRV_DIR  = ./server
LOGS_DIR = $(SRV_DIR)/logs
OUT      = $(SRV_DIR)/server.out

$(OUT): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(OUT)

run: $(OUT)
	bash run.sh

clean:
	rm -f $(OUT) $(LOGS_DIR)/*

rebuild: clean $(OUT)

.PHONY: run clean rebuild

