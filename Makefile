CFLAGS ?= -O2
TREELS_CFLAGS =  -Wall -Wextra -Wshadow -pedantic

TARGET = treels

INSTALL_DIR = /usr/local/bin/

SRC_DIR = src

.PHONY: all check install clean

all: $(TARGET)
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
		$(CC) $(CFLAGS) $(TREELS_CFLAGS) -c $< -o $@

treels-obj = $(SRC_DIR)/treels.o

$(TARGET): $(treels-obj)
	$(CC) $(LDFLAGS) -o $@ $(treels-obj)

install: all
	mkdir -p $(DESTDIR)$(INSTALL_DIR)
	cp -p $(TARGET) $(DESTDIR)$(INSTALL_DIR)

check: $(TARGET)

clean:
	rm -rf $(SRC_DIR)/*.o $(TARGET)
	
