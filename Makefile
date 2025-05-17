CC       = gcc
CFLAGS   = -Wall -Wextra

SRC_DIR	 = ./src
SRV_DIR  = ./server
LOGS_DIR = $(SRV_DIR)/logs

SRCS     = $(SRC_DIR)/main.c libs/mongoose/mongoose.c $(SRC_DIR)/input/input.c \
$(SRC_DIR)/output/output.c $(SRC_DIR)/cache/cache.c

OUT      = $(SRV_DIR)/server.out

$(OUT): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(OUT)

run: $(OUT)
	bash run.sh

clean:
	rm -f $(OUT) $(LOGS_DIR)/*

rebuild: clean $(OUT)

.PHONY: run clean rebuild

