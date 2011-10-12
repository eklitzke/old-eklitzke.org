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

static void
local_cb(struct evhttp_request *req, void *data)
{
  assert(data == NULL);

  struct evbuffer* response = NULL;
  if ((response = evbuffer_new()) == NULL) {
	return;
  }

  ctemplate::TemplateDictionary dict("home");
  std::string output;
  ctemplate::ExpandTemplate("home.html", ctemplate::DO_NOT_STRIP, &dict, &output);
  evbuffer_add(response, output.c_str(), output.length());
  evhttp_send_reply(req, 200, "OK", response);
  evbuffer_free(response);
}


int main(int argc, char **argv) {
  int port;
  std::string iface;
  struct event_base *base;
  struct evhttp *server;
  po::options_description desc("Server options");
  po::variables_map vm;


  desc.add_options()
    ("help,h", "produce help message")
    ("port,p", po::value<int>(&port)->default_value(9000), "port to bind on")
    ("interface,i", po::value<std::string>(&iface)->default_value("0.0.0.0"), "interface to listen on")
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

  evhttp_set_cb(server, "/", local_cb, NULL);
  evhttp_bind_socket(server, iface.c_str(), port);

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
