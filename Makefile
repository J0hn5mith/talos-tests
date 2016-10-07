all: sw-paillier
APPS += flash-erase

CONTIKI = ../..

RELIC_SOURCE=$(CONTIKI)/../relic
RELIC_TARGET=$(CONTIKI)/../relic/build
CFLAGS += -I$(RELIC_SOURCE)/include -I$(RELIC_TARGET)/include -Os
#PROJECT_LIBRARIES += $(RELIC_TARGET)/lib/librelic_s.a
TARGET_LIBFILES += -L$(RELIC_TARGET)/lib/ -lrelic_s


include $(CONTIKI)/Makefile.include
