#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include <ctemplate/template.h>  

#include <event2/buffer.h>
#include <event2/event.h>
#include <event2/http.h>

#include <gflags/gflags.h>
DEFINE_int32(port, 9000, "the port to listen on");
DEFINE_string(iface, "0.0.0.0", "the interface to bind to");
DEFINE_bool(daemon, false, "run as a daemon");

#include <glog/logging.h>

inline static void
respond(struct evhttp_request *req, const ctemplate::TemplateDictionary &dict, int status, const char *reason)
{
  std::string output;
  struct evbuffer *response;
  struct evkeyvalq *headers;

  headers = evhttp_request_get_output_headers(req);
  evhttp_add_header(headers, "Content-Type", "text/html; charset=utf-8");

  ctemplate::ExpandTemplate("templates/base.html", ctemplate::STRIP_BLANK_LINES, &dict, &output);
  response = evbuffer_new();
  if (response != NULL) {
    evbuffer_add(response, output.c_str(), output.length());
    evhttp_send_reply(req, status, reason, response);
    evbuffer_free(response);
  } else {
    std::cerr << "failed to evbuffer_new()" << std::endl;
  }
  assert(!evhttp_request_is_owned(req));
}

static void
req_home_cb(struct evhttp_request *req, void *data)
{
  LOG(INFO) << "got a request for /";

  ctemplate::TemplateDictionary dict("home");
  dict.SetValue("TITLE", "eklitzke.org");
  ctemplate::TemplateDictionary *child_dict = dict.AddIncludeDictionary("BODY");
  child_dict->SetFilename("templates/home.html");
  respond(req, dict, HTTP_OK, "OK");
}

static void
req_generic_cb(struct evhttp_request *req, void *data)
{
  LOG(INFO) << "got a 404 request";

  ctemplate::TemplateDictionary dict("not_found");
  dict.SetValue("TITLE", "document not found");
  ctemplate::TemplateDictionary *child_dict = dict.AddIncludeDictionary("BODY");
  child_dict->SetFilename("templates/error.html");
  respond(req, dict, HTTP_NOTFOUND, "Not Found");
}

int main(int argc, char **argv) {
  struct event_base *base = NULL;
  struct evhttp *server = NULL;

  google::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);

  if ((base = event_base_new()) == NULL) {
	std::cerr << "failed to event_base_new()" << std::endl;
    goto err;
  }
  if ((server = evhttp_new(base)) == NULL) {
	std::cerr << "failed to evhttp_new()" << std::endl;
    goto err;
  }

  evhttp_set_allowed_methods(server,  EVHTTP_REQ_GET);
  evhttp_set_cb(server, "/", req_home_cb, NULL);
  evhttp_set_gencb(server, req_generic_cb, NULL);
  evhttp_bind_socket(server, FLAGS_iface.c_str(), FLAGS_port);

  if (FLAGS_daemon) {
    daemon(1, 0);
  }

  /* start the event loop */
  if (event_base_dispatch(base) != 0) {
    std::cerr << "failed to event_base_dispatch(); check that " <<
      FLAGS_iface << ":" << FLAGS_port << " is available" << std::endl;
    goto err;
  }
  
  /* cleanup */
  evhttp_free(server);
  event_base_free(base);
  return EXIT_SUCCESS;

 err:
  if (server)
    evhttp_free(server);
  if (base)
    event_base_free(base);
  return EXIT_FAILURE;
}
