# Generated by using Rcpp::compileAttributes() -> do not edit by hand
# Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#' Compress a file using htslib's bgzf format.
#'
#' @param in_file Input file name.
#' @param compress Integer indicating compression level.
#'
#'
#' @export
#'
compress_file <- function(in_file, compress) {
    .Call(`_RhtslibTest_compress_file`, in_file, compress)
}

