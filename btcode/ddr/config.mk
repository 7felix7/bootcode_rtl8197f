LOCAL_DIR  := $(ROOTDIR)/ddr

LOCAL_SRCS := dram.c dramtest.c
LOCAL_SRCS := $(addprefix $(LOCAL_DIR)/,$(LOCAL_SRCS))

LOCAL_HEADERS := dram.h
LOCAL_HEADERS := $(addprefix $(LOCAL_DIR)/,$(LOCAL_HEADERS))

LOCAL_OBJS := $(LOCAL_SRCS:.c=.o)
$(LOCAL_OBJS): INCLUDES := -I./ddr/ -I./sdr/dram_deliver/ -I./efuse -I./monitor
$(LOCAL_OBJS): %.o: %.c $(LOCAL_HEADERS)

ROM_OBJS += $(LOCAL_OBJS)
