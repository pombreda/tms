#include "archive.hpp"
// libarchive
#include <archive.h>
#include <archive_entry.h>
// system
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// boost
#include <boost/foreach.hpp>
// std
#include <fstream>

using namespace tms::common::archive;
using namespace boost::filesystem;
using namespace std;

void tms::common::archive::Compress(vector<path> files, path archive) {
  // memory leakage is possible. Shall be wrapped in c++ classes.
  struct archive *a = archive_write_new();
  archive_write_set_compression_gzip(a);
  archive_write_set_format_pax_restricted(a);
  archive_write_open_filename(a, archive.string().c_str());
  BOOST_FOREACH(path file, files) {
    struct stat st;
    stat(file.string().c_str(), &st);
    archive_entry *entry = archive_entry_new();
    archive_entry_set_pathname(entry, file.string().c_str());
    archive_entry_copy_stat(entry, &st);
    archive_entry_set_perm(entry, 0644);
    archive_write_header(a, entry);
    if (S_ISREG(st.st_mode)) {
      ifstream fin(file.string().c_str());
      const int buf_size = 8192;
      char buf[buf_size];
      while (!fin.eof()) {
	fin.read(buf, buf_size);
	archive_write_data(a, buf, fin.gcount());
      }
    }
    archive_entry_free(entry);
  }
  archive_write_close(a);
  archive_write_free(a);
}

static int copy_data(struct archive *ar, struct archive *aw) {
  int r;
  const void *buff;
  size_t size;
  int64_t offset;
  
  for (;;) {
    r = archive_read_data_block(ar, &buff, &size, &offset);
    if (r == ARCHIVE_EOF)
      return (ARCHIVE_OK);
    if (r != ARCHIVE_OK)
      return (r);
    r = archive_write_data_block(aw, buff, size, offset);
    if (r != ARCHIVE_OK) {
      fprintf(stderr, "%s\n", archive_error_string(aw));
      return (r);
    }
  }
}

void tms::common::archive::Decompress(path archive, path destination) {
  // ToDo: handle exceptions
  archive = absolute(archive);
  path cur_path = current_path();  
  chdir(destination.string().c_str());
  struct archive *a = archive_read_new();
  struct archive *ext = archive_write_disk_new();
  int flags = 0;
  flags |= ARCHIVE_EXTRACT_TIME;
  flags |= ARCHIVE_EXTRACT_PERM;
  flags |= ARCHIVE_EXTRACT_ACL ;
  flags |= ARCHIVE_EXTRACT_FFLAGS;
  archive_read_support_format_all(a);
  archive_read_support_compression_all(a);
  archive_write_disk_set_options(ext, flags);
  archive_write_disk_set_standard_lookup(ext);
  archive_read_open_filename(a, archive.string().c_str(), 10240);
  int r;
  for (;;) {
    archive_entry *entry;
    r = archive_read_next_header(a, &entry);
    if (r == ARCHIVE_EOF) {
      break;
    }
    archive_write_header(ext, entry);
    if (archive_entry_size(entry) > 0) {
      copy_data(a, ext);
    }
    r = archive_write_finish_entry(ext);
  }
  archive_read_close(a);
  archive_read_free(a);
  archive_write_close(ext);
  archive_write_free(ext);
  chdir(cur_path.string().c_str());
}
