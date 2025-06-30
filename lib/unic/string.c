/* SPDX-License-Identifier: MIT */
/*
 * Copyright (c) 2024, Ivaylo Ivanov <ivo.ivanov.ivanov1@gmail.com>
 */

#include "stdbool.h"
#include "string.h"

#ifdef __arm__
void *memcpy (void *s1, const void *s2, size_t n)
{
	char *dest = (char *)s1;
	const char *src = (const char *)s2;

	while (n--) {
		*dest++ = *src++;
	}

	return s1;
}

void *memmove (void *s1, const void *s2, size_t n)
{
	char *dest = (char *)s1;
	const char *src = (const char *)s2;

	if (dest <= src) {
		while (n--) {
			*dest++ = *src++;
		}
	} else {
		src += n;
		dest += n;

		while (n--) {
			*--dest = *--src;
		}
	}

	return s1;
}
#endif

void *memset (void *m, int c, size_t n)
{
	char *s = (char *) m;

	unsigned int i;
	unsigned long buffer;
	unsigned long *aligned_addr;
	unsigned int d = c & 0xff;

	while (UNALIGNED (s)) {
		if (n--)
			*s++ = (char) c;
		else
			return m;
	}

	if (!TOO_SMALL (n)) {
		/* If we get this far, we know that n is large and s is word-aligned. */
		aligned_addr = (unsigned long *) s;

		/*
		 * Store D into each char sized location in BUFFER so that
		 * we can set large blocks quickly.
		 */
		buffer = (d << 8) | d;
		buffer |= (buffer << 16);
		for (i = 32; i < LBLOCKSIZE * 8; i <<= 1)
			buffer = (buffer << i) | buffer;

		/* Unroll the loop. */
		while (n >= LBLOCKSIZE*4) {
			*aligned_addr++ = buffer;
			*aligned_addr++ = buffer;
			*aligned_addr++ = buffer;
			*aligned_addr++ = buffer;
			n -= 4*LBLOCKSIZE;
		}

		while (n >= LBLOCKSIZE)	{
			*aligned_addr++ = buffer;
			n -= LBLOCKSIZE;
		}

		/* Pick up the remainder with a bytewise loop.	*/
		s = (char*)aligned_addr;
	}

	return m;
}

int memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char *p1 = (const unsigned char *)s1;
	const unsigned char *p2 = (const unsigned char *)s2;

	while (n--) {
		if (*p1 != *p2) {
			return *p1 - *p2;
		}

		++p1;
		++p2;
	}

	return 0;
}

void *memchr(const void *s, int c, size_t n)
{
	const unsigned char *p = (const unsigned char *)s;

	while (n--) {
		if (*p == (unsigned char)c) {
			return (void *)p;
		}

		++p;
	}

	return NULL;
}

char *strcpy(char *s1, const char *s2)
{
	char *rc = s1;

	while ((*s1++ = *s2++)) {
		/* EMPTY */
	}

	return rc;
}


char *strncpy(char *d, char *s, long n)
{
	int len = strlen(s);
	if (len > n)
		len = n;
	memcpy(d, s, len);
	memset(d + len, 0, n - len);
	return d;
}

char *strcat(char *d, char *s)
{
	strcpy(d + strlen(d), s);
	return d;
}

int strcmp(const char *s1, const char *s2)
{
	while ((*s1) && (*s1 == *s2)) {
		++s1;
		++s2;
	}

	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

int strncmp(const char *s1, const char *s2, size_t n)
{
	while (n && *s1 && (*s1 == *s2)) {
		++s1;
		++s2;
		--n;
	}

	if (n == 0) {
		return 0;
	} else {
		return (*(unsigned char *)s1 - *(unsigned char *)s2);
	}
}

size_t strlen(const char *s)
{
	size_t rc = 0;

	while (s[rc]) {
		++rc;
	}

	return rc;
}

size_t strnlen(const char *s, size_t n)
{
	size_t rc = 0;

	while (rc < n && s[rc]) {
		++rc;
	}

	return rc;
}

char *strchr(const char *s, int c)
{
	do {
		if (*s == (char)c) {
			return (char *)s;
		}
	} while (*s++);

	return NULL;
}

char *strrchr(const char *s, int c)
{
	size_t i = 0;

	while (s[i++]) {
		/* EMPTY */
	}

	do {
		if (s[--i] == (char)c) {
			return (char *)s + i;
		}
	} while (i);

	return NULL;
}

/* Very naive, no attempt to check for errors */
long atol(const char *s)
{
	long val = 0;
	bool neg = false;

	if (*s == '-') {
		neg = true;
		s++;
	}

	while (*s >= '0' && *s <= '9')
		val = (val * 10) + (*s++ - '0');

	if (neg)
		val = -val;

	return val;
}

void writel(unsigned int value, void* address)
{
	volatile unsigned int* ptr = (volatile unsigned int*)address;
	*ptr = value;
}

uint32_t readl(volatile uint32_t *addr)
{
	return *addr;
}
