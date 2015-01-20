#pragma once

#include "Common.h"
#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/keyvalq_struct.h>


typedef std::map<std::string, std::string> HttpHeaders;

class HttpRequest
{

	const char * internal_error = "Internal Server Error";
	const char * not_found = "Not Found";
	const char * not_allowed = " Method Not Allowed";

public:


	HttpRequest(evhttp_request * evReq)
		:m_evReq(evReq)
	{
		m_genHead = false;
	}

	~HttpRequest(){}
	
	int  Method();

	// std::string PathInfo()
	// {
	// 	struct evbuffer *evb = NULL;
	// }
	//void GetParam(std::string & key, std::string & value);
	//std::string * GetParams(std::string & key);

	bool Header(std::string &key, std::string &value)
	{
		// Lazy method
		_GenHeaders();
		auto it = m_headers.find(key);
		if (it == m_headers.end())
		{
			value += it->second;
			return true;
		}
		return false;
	}

	int Send(unsigned short status_code, std::string & status_msg, HttpHeaders & headers, std::string & body);
	//int Send(unsigned short status_code, std::string & status_msg, HttpHeaders & headers, std::sting & body, std::string & contentType);
	//int Fulsh();

private:

	void _GenHeaders();

private:

	evhttp_request * m_evReq;
	HttpHeaders		m_headers;
	bool 			m_genHead;

	// For stats
	// unsigned long m_startTime;
	// unsigned long m_finshTime;
};


// 	const char *cmdtype;
// 	struct evkeyvalq *headers;
// 	struct evkeyval *header;
// 	struct evbuffer *buf;

// 	switch (evhttp_request_get_command(req)) {
// 	case EVHTTP_REQ_GET: cmdtype = "GET"; break;
// 	case EVHTTP_REQ_POST: cmdtype = "POST"; break;
// 	case EVHTTP_REQ_HEAD: cmdtype = "HEAD"; break;
// 	case EVHTTP_REQ_PUT: cmdtype = "PUT"; break;
// 	case EVHTTP_REQ_DELETE: cmdtype = "DELETE"; break;
// 	case EVHTTP_REQ_OPTIONS: cmdtype = "OPTIONS"; break;
// 	case EVHTTP_REQ_TRACE: cmdtype = "TRACE"; break;
// 	case EVHTTP_REQ_CONNECT: cmdtype = "CONNECT"; break;
// 	case EVHTTP_REQ_PATCH: cmdtype = "PATCH"; break;
// 	default: cmdtype = "unknown"; break;
// 	}

// 	printf("Received a %s request for %s\nHeaders:\n",
// 	    cmdtype, evhttp_request_get_uri(req));

// 	headers = evhttp_request_get_input_headers(req);
// 	for (header = headers->tqh_first; header;
// 	    header = header->next.tqe_next) {
// 		printf("  %s: %s\n", header->key, header->value);
// 	}

// 	buf = evhttp_request_get_input_buffer(req);
// 	puts("Input data: <<<");
// 	while (evbuffer_get_length(buf)) {
// 		int n;
// 		char cbuf[128];
// 		n = evbuffer_remove(buf, cbuf, sizeof(cbuf));
// 		if (n > 0)
// 			(void) fwrite(cbuf, 1, n, stdout);
// 	}
// 	puts(">>>");



// #include <event2/http.h>
// #include <event2/buffer.h>
// #include <event2/util.h>
// #include <event2/keyvalq_struct.h>

// #ifdef EVENT__HAVE_NETINET_IN_H
// #include <netinet/in.h>
// # ifdef _XOPEN_SOURCE_EXTENDED
// #  include <arpa/inet.h>
// # endif
// #endif

// #ifdef _WIN32
// #define stat _stat
// #define fstat _fstat
// #define open _open
// #define close _close
// #define O_RDONLY _O_RDONLY
// #endif

// /* Instead of casting between these types, create a union with all of them,
//  * to avoid -Wstrict-aliasing warnings. */
// typedef union
// { struct sockaddr_storage ss;
//   struct sockaddr sa;
//   struct sockaddr_in in;
//   struct sockaddr_in6 i6;
// } sock_hop;


// /* This callback gets invoked when we get any http request that doesn't match
//  * any other callback.  Like any evhttp server callback, it has a simple job:
//  * it must eventually call evhttp_send_error() or evhttp_send_reply().
//  */
// static void
// send_document_cb (struct evhttp_request *req, void *arg)
// { struct evbuffer *evb = NULL;
//   const char *uri = evhttp_request_get_uri (req);
//   struct evhttp_uri *decoded = NULL;

//   /* We only handle POST requests. */
//   if (evhttp_request_get_command (req) != EVHTTP_REQ_POST)
//     { evhttp_send_reply (req, 200, "OK", NULL);
//       return;
//     }

//   printf ("Got a POST request for <%s>\n", uri);

//   /* Decode the URI */
//   decoded = evhttp_uri_parse (uri);
//   if (! decoded)
//     { printf ("It's not a good URI. Sending BADREQUEST\n");
//       evhttp_send_error (req, HTTP_BADREQUEST, 0);
//       return;
//     }

//   /* Decode the payload */
//   struct evkeyvalq kv;
//   memset (&kv, 0, sizeof (kv));
//   struct evbuffer *buf = evhttp_request_get_input_buffer (req);
//   evbuffer_add (buf, "", 1);    /* NUL-terminate the buffer */
//   char *payload = (char *) evbuffer_pullup (buf, -1);
//   if (0 != evhttp_parse_query_str (payload, &kv))
//     { printf ("Malformed payload. Sending BADREQUEST\n");
//       evhttp_send_error (req, HTTP_BADREQUEST, 0);
//       return;
//     }

//   /* Determine peer */
//   char *peer_addr;
//   ev_uint16_t peer_port;
//   struct evhttp_connection *con = evhttp_request_get_connection (req);
//   evhttp_connection_get_peer (con, &peer_addr, &peer_port);

//   /* Extract passcode */
//   const char *passcode = evhttp_find_header (&kv, "passcode");
//   char response[256];
//   evutil_snprintf (response, sizeof (response),
//                    "Hi %s!  I %s your passcode.\n", peer_addr,
//                    (0 == strcmp (passcode, COMMON_PASSCODE)
//                     ?  "liked"
//                     :  "didn't like"));
//   evhttp_clear_headers (&kv);   /* to free memory held by kv */

//   /* This holds the content we're sending. */
//   evb = evbuffer_new ();

//   evhttp_add_header (evhttp_request_get_output_headers (req),
//                      "Content-Type", "application/x-yaml");
//   evbuffer_add (evb, response, strlen (response));

//   evhttp_send_reply (req, 200, "OK", evb);

//   if (decoded)
//     evhttp_uri_free (decoded);
//   if (evb)
//     evbuffer_free (evb);