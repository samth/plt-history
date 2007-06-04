#reader(lib "docreader.ss" "scribble")
@require["mz.ss"]

@title{PLT Scheme Reference Manual}

This manual defines the core PLT Scheme language and describes its
most prominent libraries. The companion manual
@italic{@link["../guide/index.html"]{A Guide to PLT Scheme}} provides
a friendlier (though less precise and complete) overview of the
language.

@table-of-contents[]

@include-section["model.scrbl"]
@include-section["read.scrbl"]
@include-section["macros.scrbl"]
@include-section["syntax.scrbl"]
@include-section["data.scrbl"]

@index-section["mzscheme-index"]
