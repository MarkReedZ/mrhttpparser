MrHTTPParser
=============

MrHTTPParser is a fast HTTP/1 parser designed to be behind nginx which allows it to take some liberties with the parsing in the name of speed. Make test to run through the tests, and make bench / time bench for the benchmarks.

The software is under the MIT License.

Usage
-----

The library exposes four functions: `mr_parse_request`, `mr_parse_response`, `mr_parse_headers`, `mr_parse_chunked`. 

### mr_parse_request

The example below reads an HTTP request from socket `sock` using `read(2)`, parses it using `mr_parse_request`, and prints the details. `mr_parse_response` and `mr_parse_headers` provide a similar interface parsing a response and headers respectively.

```c
char buf[4096], *method, *path;
int pret, minor_version;
struct mr_header headers[100];
size_t buflen = 0, prevbuflen = 0, method_len, path_len, num_headers;
ssize_t rret;

while (1) {
  while ((rret = read(sock, buf + buflen, sizeof(buf) - buflen)) == -1 && errno == EINTR);
  if (rret <= 0) return IOError;
  prevbuflen = buflen;
  buflen += rret;
  /* parse the request */
  num_headers = sizeof(headers) / sizeof(headers[0]);
  pret = phr_parse_request(buf, buflen, &method, &method_len, &path, &path_len, &minor_version, headers, &num_headers, prevbuflen);
  if (pret > 0) break; /* successfully parsed the request */
  else if (pret == -1) return ParseError;
  /* request is incomplete, continue the loop */
  assert(pret == -2);
  if (buflen == sizeof(buf)) return RequestIsTooLongError;
}

printf("request is %d bytes long\n", pret);
printf("method is %.*s\n", (int)method_len, method);
printf("path is %.*s\n", (int)path_len, path);
printf("HTTP version is 1.%d\n", minor_version);
printf("headers:\n");
for (i = 0; i != num_headers; ++i) {
    printf("%.*s: %.*s\n", (int)headers[i].name_len, headers[i].name,
           (int)headers[i].value_len, headers[i].value);
}
```


### phr_decode_chunked

The example below decodes incoming data in chunked-encoding.  The data is decoded in-place.

```c
struct phr_chunked_decoder decoder = {}; /* zero-clear */
char *buf = malloc(4096);
size_t size = 0, capacity = 4096, rsize;
ssize_t rret, pret;

/* set consume_trailer to 1 to discard the trailing header, or the application
 * should call phr_parse_headers to parse the trailing header */
decoder.consume_trailer = 1;

do {
    /* expand the buffer if necessary */
    if (size == capacity) {
        capacity *= 2;
        buf = realloc(buf, capacity);
        assert(buf != NULL);
    }
    /* read */
    while ((rret = read(sock, buf + size, capacity - size)) == -1 && errno == EINTR)
        ;
    if (rret <= 0)
        return IOError;
    /* decode */
    rsize = rret;
    pret = phr_decode_chunked(&decoder, buf + size, &rsize);
    if (pret == -1)
        return ParseError;
    size += rsize;
} while (pret == -2);

/* successfully decoded the chunked data */
assert(pret >= 0);
printf("decoded data is at %p (%zu bytes)\n", buf, size);
```
