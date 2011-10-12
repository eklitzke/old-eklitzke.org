#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include <ctemplate/template.h>  

#include <event2/buffer.h>
#include <event2/event.h>
#include <event2/http.h>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

inline static void
respond(struct evhttp_request *req, const ctemplate::TemplateDictionary &dict, int status, const char *reason)
{
  std::string output;
  struct evbuffer* response;

  ctemplate::ExpandTemplate("templates/base.html", ctemplate::DO_NOT_STRIP, &dict, &output);
  response = evbuffer_new();
  if (response != NULL) {
    evbuffer_add(response, output.c_str(), output.length());
    evhttp_send_reply(req, status, reason, response);
    evbuffer_free(response);
  } else {
    std::cerr << "failed to evbuffer_new()" << std::endl;
  }
}

static void
req_home_cb(struct evhttp_request *req, void *data)
{
  ctemplate::TemplateDictionary dict("home");
  dict.SetValue("title", "eklitzke.org");
  dict.SetValue("body",
                "hi, my name is evan klitzke. i like python, robots, and hip-hop.\n"
                "email me at &#x0065;&#x0076;&#x0061;&#x006e;&#x0040;&#x0065;&#x006b;"
                "&#x006c;&#x0069;&#x0074;&#x007a;&#x006b;&#x0065;&#x002e;&#x006f;"
                "&#x0072;&#x0067;.");
  respond(req, dict, HTTP_OK, "OK");
}

static void
req_generic_cb(struct evhttp_request *req, void *data)
{
  ctemplate::TemplateDictionary dict("not_found");
  dict.SetValue("title", "document not found");
  dict.SetValue("body", "sorry, that url does not exist.");
  respond(req, dict, HTTP_NOTFOUND, "Not Found");
}

int main(int argc, char **argv) {
  int port;
  std::string iface;
  struct event_base *base = NULL;
  struct evhttp *server = NULL;
  po::options_description desc("Server options");
  po::variables_map vm;

  desc.add_options()
    ("help,h", "produce help message")
    ("port,p", po::value<int>(&port)->default_value(9000), "port to bind on")
    ("interface,i", po::value<std::string>(&iface)->default_value("0.0.0.0"), "interface to listen on")
    ("daemon,d", "run as a daemon")
    ;

  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);   

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
  evhttp_bind_socket(server, iface.c_str(), port);

  if (vm.count("daemon")) {
    daemon(1, 0);
  }

  /* start the event loop */
  if (event_base_dispatch(base) != 0) {
    std::cerr << "failed to event_base_dispatch(); check that " <<
      iface << ":" << port << " is available" << std::endl;
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
