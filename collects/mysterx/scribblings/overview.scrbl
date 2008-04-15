#lang scribble/doc
@(require "common.ss")

@title[#:tag "overview"]{Overview}

@section{Installation}

Two Windows DLLs support low-level operations in MysterX:
@filepath{myspage.dll} and @filepath{myssink.dll}.  Both are installed
in the registry (using @exec{regsvr32.exe}) when Setup PLT runs the
the MysterX post-installer. If you move the location of your PLT
installation, you may need to re-run Setup PLT to make MysterX
work. Neither of these DLLs is specific to a PLT Scheme version, so
it's ok for one version of PLT Scheme to use the DLLs registered by
another.

@margin-note{Prior to version 369.4, @filepath{myssink.dll} was
version-specific. Its GUID was changed when it was made
version-independent.}

If you build a stand-alone executable that uses MysterX, you need to
specifically include @filepath{myspage.dll} and @filepath{myssink.dll}
with your distribution, and the DLLs will need to be registered on the
end user's machine. One way to do that is to include the following
little setup program (as an executable) in your distribution:

@schemeblock[
  (module setup scheme/base
    (require mzlib/runtime-path
             mzlib/process)

    (code:comment #, @t{Ensure that DLLs are included with the distribution:})
    (define-runtime-path myspage-dll '(so "myspage"))
    (define-runtime-path myssink-dll '(so "myssink"))

    (code:comment #, @t{Register the DLLs:})
    (define regsvr32 
      (path->string (find-executable-path "regsvr32.exe" #f)))
    (system* regsvr32 (path->string myspage-dll))
    (system* regsvr32 (path->string myssink-dll)))
]

@section{Running a Demo}

Try 

@schemeblock[
  (require mysterx/mxdemo)
]

The demo requires the MSCal Calendar control.  The calendar control is
normally installed with Microsoft Office, but it can also be
downloaded from elsewhere; look for @filepath{mscal.ocx}.


@section{Loading}

Load the MysterX module with 

@schemeblock[
  (require mysterx)
]

Because some MysterX code relies on the @schememodname[scheme/class]
class system, you may also need

@schemeblock[
  (require mzlib/class)
]

Several MysterX procedures take HTML strings as input.  The
@schememodname[xml] library provides procedures that convert Scheme
syntax into XML strings.  You may find using these procedures useful
in creating HTML strings for use by MysterX.
