#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include <fcntl.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <ctemplate/template.h>  

#include <event2/buffer.h>
#include <event2/event.h>
#include <event2/http.h>

#include <gflags/gflags.h>
DEFINE_int32(port, 9000, "the port to listen on");
DEFINE_string(iface, "0.0.0.0", "the interface to bind to");
DEFINE_bool(daemon, false, "run as a daemon");
DEFINE_string(user, "", "run as a daemon");
DEFINE_bool(chroot, true, "chroot after starting");

#include <glog/logging.h>

std::map<std::string, const char*> content_types;

static void
initialize(void)
{
  content_types["cc"]  = "text/plain";
  content_types["h"]   = "text/plain";
  content_types["ico"] = "image/x-icon";
  content_types["jpg"] = "image/jpeg";
  content_types["png"] = "image/png";
  content_types["txt"] = "text/plain";
}

static void
finish(void)
{
  content_types.clear();
}

static const char *
get_content_type(const std::string &filename) {
  size_t dotpos = filename.find_last_of('.');
  if (dotpos == filename.npos) {
    return "application/octet-stream";
  }
  std::string extension = filename.substr(dotpos + 1, filename.length() - dotpos);
  if (content_types.count(extension)) {
    return content_types[extension];
  } else {
    return "application/octet-stream";
  }
}

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
req_notfound(struct evhttp_request *req, void *data)
{
  ctemplate::TemplateDictionary dict("not_found");
  dict.SetValue("TITLE", "document not found");
  ctemplate::TemplateDictionary *child_dict = dict.AddIncludeDictionary("BODY");
  child_dict->SetFilename("templates/error.html");
  respond(req, dict, HTTP_NOTFOUND, "Not Found");
}

static void
req_generic_cb(struct evhttp_request *req, void *data)
{
  const struct evhttp_uri *evuri = evhttp_request_get_evhttp_uri(req);
  const char *uripath = evhttp_uri_get_path(evuri);
  size_t pathsize;
  char *path = evhttp_uridecode(uripath, 0, &pathsize);
  size_t offset = 7;
  if (strncmp(path, "/static/", 8) == 0) {
    while (offset < pathsize && *(path + offset) == '/') {
      offset++;
    }

    int fd = open(path + offset, O_RDONLY);
    if (fd < 0)
      goto notfound;

    struct stat buf;
    if (fstat(fd, &buf) != 0)
      goto notfound;
      
    struct evkeyvalq *headers = evhttp_request_get_output_headers(req);
    evhttp_add_header(headers, "Content-Type", get_content_type(path + offset));

    free(path);
    struct evbuffer *response = evbuffer_new();
    evbuffer_add_file(response, fd, 0, buf.st_size);
    evhttp_send_reply(req, HTTP_OK, "OK", response);
    return;
  }

 notfound:
  LOG(INFO) << "failed to find " << uripath;
  free(path);
  req_notfound(req, data);
}

int main(int argc, char **argv) {
  struct event_base *base = NULL;
  struct evhttp *server = NULL;

  google::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  initialize();

  if ((base = event_base_new()) == NULL) {
	std::cerr << "failed to event_base_new()" << std::endl;
    goto err;
  }
  if ((server = evhttp_new(base)) == NULL) {
	std::cerr << "failed to evhttp_new()" << std::endl;
    goto err;
  }

  if (FLAGS_user.length()) {
    //struct passwd *p = getpwnam(FLAGS_user.c_str());
    struct passwd *p = getpwnam("evan");
    if (p != NULL) {
      setuid(p->pw_uid);
      setgid(p->pw_gid);
    } else {
      LOG(WARNING) << "failed to lookup user " << FLAGS_user << ", errno " << errno;
    }
  }
  if (FLAGS_chroot) {
    char *rp = NULL;
    if ((rp = realpath(".", rp)) != NULL) {
      if (chroot(rp) != 0) {
        LOG(INFO) << "failed to chroot";
        perror("chroot()");
      }
      LOG(INFO) << rp;
      free(rp);
    } else {
      LOG(INFO) << "failed to realpath";
    }
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
  finish();
  evhttp_free(server);
  event_base_free(base);
  return EXIT_SUCCESS;

 err:
  finish();
  if (server)
    evhttp_free(server);
  if (base)
    event_base_free(base);
  return EXIT_FAILURE;
}
