# name of your application
APPLICATION = hello-world

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../..

SRC = \mesh_led.c \mesh_uart.c \mesh_comms.c
# EXE_INC = -I$(RIOTBASE)/drivers/uart_half_duplex/include
# INCLUDES =  include/uart_half_duplex.h

# If no BOARD is found in the environment, use this default:
# BOARD ?= native
BOARD = frdm-kw41z

# USEMODULE += uart_half_duplex

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# ********* OPENTHREAD START *********


# ********* OPENTHREAD END *********

# ********* UART *********
FEATURES_REQUIRED += periph_uart
FEATURES_OPTIONAL += periph_lpuart  # STM32 L0 and L4 provides lpuart support
FEATURES_OPTIONAL += periph_uart_modecfg

# ********* UART END *********

# ********* UDP **********
# NOTE: 6LoWPAN will be included if IEEE802.15.4 devices are present
USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif
# Specify the mandatory networking modules for socket communication via UDP
USEMODULE += gnrc_ipv6_default
USEMODULE += gnrc_udp
USEMODULE += gnrc_sock_udp
USEMODULE += posix_sockets
USEMODULE += posix_time
USEMODULE += posix_inet
# ********* UDP END *********


# ********* TIMER *********
USEMODULE += xtimer
# ********* TIMER END *********

USEMODULE += gnrc_sock_udp
USEMODULE += gnrc_ipv6_default

# include $(RIOTBASE)/drivers/uart_half_duplex/Makefile
# include $(RIOTBASE)/Makefile.base
include $(RIOTBASE)/Makefile.include
include $(RIOTBASE)/Makefile.dep

