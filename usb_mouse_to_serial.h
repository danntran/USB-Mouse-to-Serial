/*
 * USB Mouse to Serial - Translates USB mouse inputs to serial mouse protocol
 *
 * Copyright (c) 2018 Martin Börjesson
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <libevdev/libevdev.h>
#include <signal.h>
#include <time.h>
#include <pthread.h>
#include <regex.h>
#include <dirent.h>
#include <getopt.h>
#include <locale.h>
#include <math.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define VERSION "1.1.2"

/* Protocol reset timeout in milliseconds. Set to 0 to disable. */
#define PROTOCOL_RESET_TIMEOUT 1000

/* The queue size for mouse buttons */
#define MOUSE_BUTTON_QUEUE_SIZE 10

#define THREE_BUTTONS 1<<10
#define MOUSE_WHEEL 1<<11
#define SERIAL_7N1 1<<12
#define SERIAL_8N1 1<<13

#define PROTOCOL_MICROSOFT (1<<0|SERIAL_7N1)
#define PROTOCOL_MICROSOFT_WHEEL (1<<1|THREE_BUTTONS|MOUSE_WHEEL|SERIAL_7N1)
#define PROTOCOL_LOGITECH (1<<2|THREE_BUTTONS|SERIAL_7N1)
#define PROTOCOL_MOUSE_SYSTEMS (1<<3|THREE_BUTTONS|SERIAL_8N1)

#define SOCKET_RECV_SIZE 512
#define SOCKET_BUF_SIZE 64
#define SOCKET_SEND_SIZE 8196
