#include "mime.hpp"

namespace eck {
  namespace mime {

    std::map<std::string, std::string> content_types;

    void
    initialize(void)
    {
      content_types["3gp"]     = "video/3gpp";
      content_types["3gpp"]    = "video/3gpp";
      content_types["7z"]      = "application/x-7z-compressed";
      content_types["ai"]      = "application/postscript";
      content_types["asf"]     = "video/x-ms-asf";
      content_types["asx"]     = "video/x-ms-asf";
      content_types["atom"]    = "application/atom+xml";
      content_types["avi"]     = "video/x-msvideo";
      content_types["bin"]     = "application/octet-stream";
      content_types["bmp"]     = "image/x-ms-bmp";
      content_types["cco"]     = "application/x-cocoa";
      content_types["crt"]     = "application/x-x509-ca-cert";
      content_types["css"]     = "text/css";
      content_types["deb"]     = "application/octet-stream";
      content_types["der"]     = "application/x-x509-ca-cert";
      content_types["dll"]     = "application/octet-stream";
      content_types["dmg"]     = "application/octet-stream";
      content_types["doc"]     = "application/msword";
      content_types["ear"]     = "application/java-archive";
      content_types["eot"]     = "application/octet-stream";
      content_types["eps"]     = "application/postscript";
      content_types["exe"]     = "application/octet-stream";
      content_types["flv"]     = "video/x-flv";
      content_types["gif"]     = "image/gif";
      content_types["hqx"]     = "application/mac-binhex40";
      content_types["htc"]     = "text/x-component";
      content_types["htm"]     = "text/html";
      content_types["html"]    = "text/html";
      content_types["ico"]     = "image/x-icon";
      content_types["img"]     = "application/octet-stream";
      content_types["iso"]     = "application/octet-stream";
      content_types["jad"]     = "text/vnd.sun.j2me.app-descriptor";
      content_types["jar"]     = "application/java-archive";
      content_types["jardiff"] = "application/x-java-archive-diff";
      content_types["jng"]     = "image/x-jng";
      content_types["jnlp"]    = "application/x-java-jnlp-file";
      content_types["jpeg"]    = "image/jpeg";
      content_types["jpg"]     = "image/jpeg";
      content_types["js"]      = "application/x-javascript";
      content_types["kar"]     = "audio/midi";
      content_types["kml"]     = "application/vnd.google-earth.kml+xml";
      content_types["kmz"]     = "application/vnd.google-earth.kmz";
      content_types["mid"]     = "audio/midi";
      content_types["midi"]    = "audio/midi";
      content_types["mml"]     = "text/mathml";
      content_types["mng"]     = "video/x-mng";
      content_types["mov"]     = "video/quicktime";
      content_types["mp3"]     = "audio/mpeg";
      content_types["mpeg"]    = "video/mpeg";
      content_types["mpg"]     = "video/mpeg";
      content_types["msi"]     = "application/octet-stream";
      content_types["msm"]     = "application/octet-stream";
      content_types["msp"]     = "application/octet-stream";
      content_types["ogg"]     = "audio/ogg";
      content_types["pdb"]     = "application/x-pilot";
      content_types["pdf"]     = "application/pdf";
      content_types["pem"]     = "application/x-x509-ca-cert";
      content_types["pl"]      = "application/x-perl";
      content_types["pm"]      = "application/x-perl";
      content_types["png"]     = "image/png";
      content_types["ppt"]     = "application/vnd.ms-powerpoint";
      content_types["prc"]     = "application/x-pilot";
      content_types["ps"]      = "application/postscript";
      content_types["ra"]      = "audio/x-realaudio";
      content_types["rar"]     = "application/x-rar-compressed";
      content_types["rpm"]     = "application/x-redhat-package-manager";
      content_types["rss"]     = "application/rss+xml";
      content_types["rtf"]     = "application/rtf";
      content_types["run"]     = "application/x-makeself";
      content_types["sea"]     = "application/x-sea";
      content_types["shtml"]   = "text/html";
      content_types["sit"]     = "application/x-stuffit";
      content_types["svg"]     = "image/svg+xml";
      content_types["swf"]     = "application/x-shockwave-flash";
      content_types["tcl"]     = "application/x-tcl";
      content_types["tif"]     = "image/tiff";
      content_types["tiff"]    = "image/tiff";
      content_types["tk"]      = "application/x-tcl";
      content_types["txt"]     = "text/plain";
      content_types["war"]     = "application/java-archive";
      content_types["wbmp"]    = "image/vnd.wap.wbmp";
      content_types["wml"]     = "text/vnd.wap.wml";
      content_types["wmlc"]    = "application/vnd.wap.wmlc";
      content_types["wmv"]     = "video/x-ms-wmv";
      content_types["xhtml"]   = "application/xhtml+xml";
      content_types["xls"]     = "application/vnd.ms-excel";
      content_types["xml"]     = "text/xml";
      content_types["xpi"]     = "application/x-xpinstall";
      content_types["zip"]     = "application/zip";
    }

    void
    finish(void)
    {
      content_types.clear();
    }

    std::string
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
  }
}
