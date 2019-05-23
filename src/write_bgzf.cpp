
#include <Rcpp.h>


#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "htslib/bgzf.h"  // BGZF

using namespace Rcpp;


//' Compress a file using htslib's bgzf format.
//'
//' @param in_file Input file name.
//' @param compress Integer indicating compression level.
//'
//'
//' @export
//'
//[[Rcpp::export]]
int compress_file(const std::string& in_file,
                  const int& compress) {

    if (compress < -1 || compress > 9) {
        stop("\nInvalid bgzip compress level. It must be in range [0,9].");
    }
    std::string out_mode = "w";
    if (compress >= 0) out_mode += std::to_string(compress);

    std::string out_file = in_file + ".gz";
    BGZF *file = bgzf_open(out_file.c_str(), out_mode.c_str());
    if (file == NULL) {
        stop("\nIn bgzip step, it can't create file");
    }

    void *buffer;
    int c;
    struct stat sbuf;
    int f_src = fileno(stdin);

    const int WINDOW_SIZE = 64 * 1024;

    // Checking source file:
    if (stat(in_file.c_str(), &sbuf) < 0) {
        stop("\nIn bgzip step, input file had non-zero status");
    }
    if ((f_src = open(in_file.c_str(), O_RDONLY)) < 0) {
        stop("\nIn bgzip step, input file could not be opened.");
    }

    int in_code;

    // Create buffer of info to pass between:
    buffer = malloc(WINDOW_SIZE);
#ifdef _WIN32
    _setmode(f_src, O_BINARY);
#endif
    // Writing info from one to another
    while ((c = read(f_src, buffer, WINDOW_SIZE)) > 0) {
        in_code = bgzf_write(file, buffer, c);
    }

    unlink(in_file.c_str());
    free(buffer);
    close(f_src);

    int out_code = bgzf_close(file);

    return out_code;
}

