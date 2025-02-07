#ifndef HTTP_H_
#define HTTP_H_

/*
RFC: https://www.rfc-editor.org/rfc/rfc1945#section-4.3
general-header = Date
               | Pragma
*/
typedef struct {
    const char *date;
    const char *pragma;
} gnrl_hdr_t;

/*
RFC: https://www.rfc-editor.org/rfc/rfc1945#section-5.1.1
method = "GET"
       | "POST"
       | "HEAD"
*/
typedef enum { GET, POST, HEAD } http_method_t;

/*
RFC: https://www.rfc-editor.org/rfc/rfc1945#section-5.1
request-line = method uri http-version CRLF
*/
typedef struct {
    http_method_t method;
    const char *uri;
    const char *version;
} req_line_t;

/*
RFC: https://www.rfc-editor.org/rfc/rfc1945#section-5.2
request-header = authorization
               | from
               | if-modified-since
               | referer
               | user-agent
*/
typedef struct {
    const char *authorization;
    const char *from;
    const char *if_modified_since;
    const char *referer;
    const char *user_agent;
} req_hdr_t;

/*
RFC: https://www.rfc-editor.org/rfc/rfc1945#section-7.1
entity-header = allow
              | content-encoding
              | content-length
              | content-type
              | exires
              | last-modified
*/
typedef struct {
    const char *allow;
    const char *content_encoding;
    int content_length;
    const char *content_type;
    const char *expires;
    const char *last_modified;
} ent_hdr_t;

/*
RFC: https://www.rfc-editor.org/rfc/rfc1945#section-5
full-request = request-line
            *( general-header
            | request-header
            | entity-header )
            CRLF
            [ entity-body ]
*/
typedef struct {
    req_line_t line;
    gnrl_hdr_t gnrl_hdr;
    req_hdr_t hdr;
    ent_hdr_t entity_hdr;
    const char *body;
} http_req_t;

/*
RFC: https://www.rfc-editor.org/rfc/rfc1945#section-6.1
status-line = http-version status-code reason-phrase CRLF
*/
typedef struct {
    const char *version;
    int status_code;
    const char *reason_phrase;
} status_line;

/*
RFC: https://www.rfc-editor.org/rfc/rfc1945#section-6
response-header = location
                | server
                | www-authenticate
*/
typedef struct {
    const char *location;
    const char *server;
    const char *www_authenticate;
} res_hdr_t;

/*
RFC: https://www.rfc-editor.org/rfc/rfc1945#section-6
full-response = status-line
             *( general-header
              | response-header
              | entity-header )
              CRLF
              [ entity-body ]
*/
typedef struct {
    status_line line;
    res_hdr_t res_hdr;
    ent_hdr_t entity_hdr;
    const char *body;
} http_res_t;

int parse_http_req(char *req, http_req_t *http_req);
int compose_http_res(http_res_t *http_res, char *res);

#endif