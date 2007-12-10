#lang scribble/doc
@(require "mz.ss"
          (for-label framework/preferences))

@title{Filesystem}

@;------------------------------------------------------------------------
@section[#:tag "findpaths"]{Locating Paths}

@defproc[(find-system-path [kind symbol?]) path?]{

Returns a machine-specific path for a standard type of path specified
by @scheme[kind], which must be one of the following:

@itemize{

 @item{@indexed-scheme['home-dir] --- the current user's home
 directory.

 Under Unix and Mac OS X, this directory is determined by expanding
 the path @filepath{~}, which is expanded by first checking for a
 @indexed-envvar{HOME} environment variable. If none is defined, the
 @indexed-envvar{USER} and @indexed-envvar{LOGNAME} environment
 variables are consulted (in that order) to find a user name, and then
 system files are consulted to locate the user's home directory.

 Under Windows, the user's home directory is the user-specific profile
 directory as determined by the Windows registry. If the registry
 cannot provide a directory for some reason, the value of the
 @indexed-envvar{USERPROFILE} environment variable is used instead, as
 long as it refers to a directory that exists. If @envvar{USERPROFILE}
 also fails, the directory is the one specified by the
 @indexed-envvar{HOMEDRIVE} and @indexed-envvar{HOMEPATH} environment
 variables.  If those environment variables are not defined, or if the
 indicated directory still does not exist, the directory containing
 the current executable is used as the home directory.}

 @item{@indexed-scheme['pref-dir] --- the standard directory for
 storing the current user's preferences. Under Unix, the directory is
 @filepath{.plt-scheme} in the user's home directory.  Under Windows, it
 is @filepath{PLT Scheme} in the user's application-data folder as
 specified by the Windows registry; the application-data folder is
 usually @filepath{Application Data} in the user's profile
 directory. Under Mac OS X, it is @filepath{Library/Preferences} in the
 user's home directory. This directory might not exist.}

 @item{@indexed-scheme['pref-file] --- a file that contains a
 symbol-keyed association list of preference values. The file's
 directory path always matches the result returned for
 @scheme['pref-dir]. The file name is @filepath{plt-prefs.ss} under Unix
 and Windows, and it is @filepath{org.plt-scheme.prefs.ss} under Mac OS
 X. The file's directory might not exist. See also
 @scheme[get-preference].}

 @item{@indexed-scheme['temp-dir] --- the standard directory for
 storing temporary files. Under @|AllUnix|, this is the directory
 specified by the @indexed-envvar{TMPDIR} environment variable, if it
 is defined.}

 @item{@indexed-scheme['init-dir] --- the directory containing the
 initialization file used by stand-alone @exec{mzscheme} executable.
 It is the same as the current user's home directory.}

 @item{@indexed-scheme['init-file] --- the file loaded at start-up by
 the stand-alone @exec{mzscheme} executable. The directory part of the
 path is the same path as returned for @scheme['init-dir].  The file
 name is platform-specific:

  @itemize{

  @item{@|AllUnix|: @indexed-file{.mzschemerc}}

  @item{Windows: @indexed-file{mzschemerc.ss}}

  }}

 @item{@indexed-scheme['addon-dir] --- a directory for installing PLT Scheme
 extensions. It's the same as @scheme['pref-dir], except under Mac OS
 X, where it is @filepath{Library/PLT Scheme} in the user's home
 directory. This directory might not exist.}

 @item{@indexed-scheme['doc-dir] --- the standard directory for
 storing the current user's documents. Under Unix, it's the same as
 @scheme['home-dir]. Under Mac OS X, it's the
 @filepath{Documents} directory in the user's home directory. Under
 Windows, it is the user's documents folder as specified by the
 Windows registry; the documents folder is usually @filepath{My Documents}
 in the user's home directory.}

 @item{@indexed-scheme['desk-dir] --- the directory for the current user's
 desktop. Under Unix, it's the same as @scheme['home-dir]. Under
 Windows, it is the user's desktop folder as specified by the Windows
 registry; the documents folder is usually @filepath{Desktop} in the
 user's home directory. Under Mac OS X, it is the desktop directory,
 which is specifically @filepath{~/Desktop} under Mac OS X.}

 @item{@indexed-scheme['sys-dir] --- the directory containing the
 operating system for Windows. Under @|AllUnix|, the
 result is @scheme["/"].}

 @item{@indexed-scheme['exec-file] --- the path of the @exec{mzscheme}
 executable as provided by the operating system for the current
 invocation.

 @margin-note{For MrEd, the executable path is the name of a MrEd
 executable.}}

 @item{@indexed-scheme['run-file] --- the path of the current
  executable; this may be different from result for
  @scheme['exec-file] because an alternate path was provided through a
  @DFlag{name} or @Flag{N} command-line flag to the @exec{mzscheme}
  (or @exec{mred}) executable, or because an embedding executable
  installed an alternate path. In particular a ``launcher'' script
  created by @scheme[make-mzscheme-launcher] sets this path to the
  script's path. In the @exec{mzscheme} executable, this path is also
  bound initially to @scheme[program].}

 @item{@indexed-scheme['collects-dir] --- a path to the main
 collection of libraries (see @secref["collects"]). If this path is
 relative, it's relative to the directory of @scheme[(find-system-path
 'exec-file)].  This path is normally embedded in the @exec{mzscheme}
 executable, but it can be overridden by the @DFlag{collects} or
 @Flag{X} command-line flag.}

 @item{@indexed-scheme['orig-dir] --- the current directory at
 start-up, which can be useful in converting a relative-path result
 from @scheme[(find-system-path 'exec-file)] or
 @scheme[(find-system-path 'run-file)] to a complete path.}

 }}

@defproc[(path-list-string->path-list [str (or/c string? bytes?)]
                                      [default-path-list (listof path?)])
         (listof path?)]{
 

Parses a string or byte string containing a list of paths, and returns
a list of path strings. Under @|AllUnix|, paths in a path list are
separated by a @litchar{:}; under Windows, paths are separated by a
@litchar{;}. Whenever the path list contains an empty path, the list
@scheme[default-path-list] is spliced into the returned list of
paths. Parts of @scheme[str] that do not form a valid path are not
included in the returned list.}


@defproc[(find-executable-path [program-sub path-string?][related-sub path-string?][deepest? any/c #f]) 
         (or/c path? false/c)]{

Finds a path for the executable @scheme[program-sub], returning
@scheme[#f] if the path cannot be found.

If @scheme[related-sub] is not @scheme[#f], then it must be a relative
path string, and the path found for @scheme[program-sub] must be such
that the file or directory @scheme[related-sub] exists in the same
directory as the executable. The result is then the full path for the
found @scheme[related-sub], instead of the path for the executable.
 
This procedure is used by the @exec{mzscheme} executable to find the
standard library collection directory (see @secref["collects"]).  In
this case, @scheme[program] is the name used to start MzScheme and
@scheme[related] is @scheme["collects"].  The @scheme[related-sub]
argument is used because, under @|AllUnix|, @scheme[program-sub] may
involve to a sequence of soft links; in this case,
@scheme[related-sub] determines which link in the chain is relevant.

If @scheme[related-sub] is not @scheme[#f], then when
@scheme[find-executable-path] does not finds a @scheme[program-sub]
that is a link to another file path, the search can continue with the
destination of the link. Further links are inspected until
@scheme[related-sub] is found or the end of the chain of links is
reached. If @scheme[deepest?] is @scheme[#f] (the default), then the
result corresponds to the first path in a chain of links for which
@scheme[related-sub] is found (and further links are not actually
explored); otherwise, the result corresponds to the last link in the
chain for which @scheme[related-sub] is found.

If @scheme[program-sub] is a pathless name,
@scheme[find-executable-path] gets the value of the
@indexed-envvar{PATH} environment variable; if this environment
variable is defined, @scheme[find-executable-path] tries each path in
@envvar{PATH} as a prefix for @scheme[program-sub] using the search
algorithm described above for path-containing
@scheme[program-sub]s. If the @envvar{PATH} environment variable is
not defined, @scheme[program-sub] is prefixed with the current
directory and used in the search algorithm above. (Under Windows, the
current directory is always implicitly the first item in
@envvar{PATH}, so @scheme[find-executable-path] checks the current
directory first under Windows.)}

@;------------------------------------------------------------------------
@section[#:tag "fileutils"]{Files}

@defproc[(file-exists? [path path-string?]) boolean?]{

Returns @scheme[#t] if a file (not a directory) @scheme[path] exists,
@scheme[#f] otherwise.

Under Windows, @scheme[file-exists?]  reports @scheme[#t] for all
variations of the special filenames (e.g., @scheme["LPT1"],
@scheme["x:/baddir/LPT1"]).}

@defproc[(link-exists? [path path-string?]) boolean?]{

Returns @scheme[#t] if a link @scheme[path] exists (@|AllUnix|),
@scheme[#f] otherwise.

The predicates @scheme[file-exists?]  or @scheme[directory-exists?]
work on the final destination of a link or series of links, while
@scheme[link-exists?]  only follows links to resolve the base part of
@scheme[path] (i.e., everything except the last name in the
path).

This procedure never raises the @scheme[exn:fail:filesystem]
exception.}


@defproc[(delete-file [path path-string?]) void?]{

Feletes the file with path @scheme[path] if it exists, otherwise the
@exnraise[exn:fail:filesystem]. If @scheme[path] is a link, the link
is deleted rather than the destination of the link.}

@defproc[(rename-file-or-directory [old path-string?]
                                   [new path-string?]
                                   [exists-ok? any/c #f]) 
         void?]{
 
Renames the file or directory with path @scheme[old]---if it
exists---to the path @scheme[new]. If the file or directory is not
renamed successfully, the @exnraise[exn:fail:filesystem].

This procedure can be used to move a file/directory to a different
directory (on the same disk) as well as rename a file/directory within
a directory. Unless @scheme[exists-ok?]  is provided as a true value,
@scheme[new] cannot refer to an existing file or directory. Even if
@scheme[exists-ok?] is true, @scheme[new] cannot refer to an existing
file when @scheme[old] is a directory, and vice versa. (If
@scheme[new] exists and is replaced, the replacement is atomic in the
filesystem, except under Windows 95, 98, or Me. However, the check for
existence is not included in the atomic action, which means that race
conditions are possible when @scheme[exists-ok?] is false or not
supplied.)

If @scheme[old] is a link, the link is renamed rather than the
destination of the link, and it counts as a file for replacing any
existing @scheme[new].}


@defproc[(file-or-directory-modify-seconds [path path-string?]
                                           [secs-n (or/c exact-integer? false/c) #f]
                                           [fail-thunk (-> any) (lambda () (raise (make-exn:fail:filesystem ....)))])
         any]{

Returns the file or directory's last modification date as
platform-specific seconds (see also @secref["time"]) when
@scheme[secs-n] is not provided or is @scheme[#f]. (For FAT
filesystems under Windows, directories do not have modification
dates. Therefore, the creation date is returned for a directory (but
the modification date is returned for a file).)

If @scheme[secs-n] is provided and not @scheme[#f], the access and
modification times of @scheme[path] are set to the given time.

On error (e.g., if no such file exists), @scheme[fail-thunk] is
called, and the default @scheme[fail-thunk] raises
@scheme[exn:fail:filesystem].}


@defproc[(file-or-directory-permissions [path path-string?]) (listof symbol?)]{

Returns a list containing @indexed-scheme['read],
@indexed-scheme['write], and/or @indexed-scheme['execute] for the
given file or directory path. On error (e.g., if no such file exists),
the @exnraise[exn:fail:filesystem]. Under @|AllUnix|, permissions are
checked for the current effective user instead of the real user.}

@defproc[(file-size [path path-string?]) nonnegative-exact-integer?]{

Returns the (logical) size of the specified file in bytes. Under Mac
OS X, this size excludes the resource-fork size. On error (e.g., if no
such file exists), the @exnraise[exn:fail:filesystem].}


@defproc[(copy-file [src path-string?][dest path-string?]) void?]{

Creates the file @scheme[dest] as a copy of @scheme[src]. If the file
is not successfully copied, the @exnraise[exn:fail:filesystem]. If
@scheme[dest] already exists, the copy will fail. File permissions are
preserved in the copy. Under Mac OS X, the resource fork is also
preserved in the copy. If @scheme[src] refers to a link, the target of
the link is copied, rather than the link itself.}

@defproc[(make-file-or-directory-link [to path-string?][path path-string?]) 
         void?]{

Creates a link @scheme[path] to @scheme[to] under @|AllUnix|. The
creation will fail if @scheme[path] already exists. The @scheme[to]
need not refer to an existing file or directory, and @scheme[to] is
not expanded before writing the link. If the link is not created
successfully,the @exnraise[exn:fail:filesystem]. Under Windows, the
@exnraise[exn:fail:unsupported] always.}

@;------------------------------------------------------------------------
@section[#:tag "directories"]{Directories}

See also: @scheme[rename-file-or-directory],
@scheme[file-or-directory-modify-seconds],
@scheme[file-or-directory-permissions].

@defparam[current-directory path path-string?]{

A parameter that determines the current directory for resolving
relative paths.

When the parameter procedure is called to set the current directory,
the path argument is @tech{cleanse}d using @scheme[cleanse-path],
simplified using @scheme[simplify-path], and then converted to a
directory path with @scheme[path->directory-path]; cleansing and
simplification raise an exception if the path is ill-formed. Thus, the
current value of @scheme[current-directory] is always a cleansed,
simplified, complete, directory path.

The path is not checked for existence when the parameter is set.}


@defproc[(current-drive) path?]{

Returns the current drive name Windows. For other platforms, the
@exnraise[exn:fail:unsupported]. The current drive is always the drive
of the current directory.}


@defproc[(directory-exists? [path path-string?]) boolean?]{

Returns @scheme[#t] if @scheme[path] refers to a directory,
@scheme[#f] otherwise.}

@defproc[(make-directory [path path-string?]) void?]{

Creates a new directory with the path @scheme[path].  If the directory
is not created successfully, the @exnraise[exn:fail:filesystem].}


@defproc[(delete-directory [path path-string?]) void?]{

Deletes an existing directory with the path @scheme[path]. If the
directory is not deleted successfully, the
@exnraise[exn:fail:filesystem].}


@defproc[(directory-list [path path-string? (current-directory)]) 
         (listof path?)]{

Returns a list of all files and directories in the directory specified
by @scheme[path]. If @scheme[path] is omitted, a list of files and
directories in the current directory is returned. Under @|AllUnix|, an
element of the list can start with @litchar{./~} if it would otherwise
start with @litchar{~}. Under Windows, an element of the list may
start with @litchar["\\\\?\\REL\\\\"].}


@defproc[(filesystem-root-list) (listof path?)]{

Returns a list of all current root directories. Obtaining this list
can be particularly slow under Windows.}

@;------------------------------------------------------------------------
@section{More File and Directory Utilities}

@note-lib[scheme/file]

@defproc[(copy-directory/files [src path-string?][dest path-string?]) 
         void?]{

Copies the file or directory @scheme[src] to @scheme[dest], raising
@scheme[exn:fail:filesystem] if the file or directory cannot be
copied, possibly because @scheme[dest] exists already. If @scheme[src]
is a directory, the copy applies recursively to the directory's
content. If a source is a link, the target of the link is copied
rather than the link itself.}

@defproc[(delete-directory/files [path path-string?])
         void?]{

Deletes the file or directory specified by @scheme[path], raising
@scheme[exn:fail:filesystem] if the file or directory cannot be
deleted. If @scheme[path] is a directory, then
@scheme[delete-directory/files] is first applied to each file and
directory in @scheme[path] before the directory is deleted.}

@defproc[(find-files [predicate (path? . -> . any/c)]
                     [start-path (or/c path-string? false/c) #f])
         (listof path?)]{

Traverses the filesystem starting at @scheme[start-path] and creates a
list of all files and directories for which @scheme[predicate] returns
true. If @scheme[start-path] is @scheme[#f], then the traversal starts
from @scheme[(current-directory)]. In the resulting list, each
directory precedes its content.

The @scheme[predicate] procedure is called with a single argument for
each file or directory. If @scheme[start-path] is @scheme[#f], the
argument is a pathname string that is relative to the current
directory. Otherwise, it is a path building on
@scheme[start-path]. Consequently, supplying
@scheme[(current-directory)] for @scheme[start-path] is different from
supplying @scheme[#f], because @scheme[predicate] receives complete
paths in the former case and relative paths in the latter.  Another
difference is that @scheme[predicate] is not called for the current
directory when @scheme[start-path] is @scheme[#f].

The @scheme[find-files] traversal follows soft links. To avoid
following links, use the more general @scheme[fold-files] procedure.

If @scheme[start-path] does not refer to an existing file or
directory, then @scheme[predicate] will be called exactly once with
@scheme[start-path] as the argument.}

@defproc[(pathlist-closure [path-list (listof path-string?)])
         (listof path?)]{

Given a list of paths, either absolute or relative to the current
directory, returns a list such that

@itemize{

 @item{if a nested path is given, all of its ancestors are also
       included in the result (but the same ancestor is not added
       twice);}

 @item{if a path refers to directory, all of its descendants are also
       included in the result;}

 @item{ancestor directories appear before their descendants in the
       result list.}

}}


@defproc[(fold-files [proc (and/c (path? (one-of/c 'file 'dir 'link) any/c 
                                   . -> . any/c)
                                  (or/c procedure?
                                        ((path? (one-of/c 'dir) any/c) 
                                         . ->* . (any/c any/c))))]
                     [init-val any/c]
                     [start-path (or/c path-string? false/c) #f]
                     [follow-links? any/c #t])
         any]{

Traverses the filesystem starting at @scheme[start-path], calling
@scheme[proc] on each discovered file, directory, and link. If
@scheme[start-path] is @scheme[#f], then the traversal starts from
@scheme[(current-directory)].

The @scheme[proc] procedure is called with three arguments for each
file, directory, or link:

@itemize{

 @item{If @scheme[start-path] is @scheme[#f], the first argument is a
 pathname string that is relative to the current directory. Otherwise,
 the first argument is a pathname that starts with
 @scheme[start-path]. Consequently, supplying
 @scheme[(current-directory)] for @scheme[start-path] is different
 from supplying @scheme[#f], because @scheme[proc] receives complete
 paths in the former case and relative paths in the latter. Another
 difference is that @scheme[proc] is not called for the current
 directory when @scheme[start-path] is @scheme[#f].}

 @item{The second argument is a symbol, either @scheme['file],
 @scheme['dir], or @scheme['link]. The second argument can be
 @scheme['link] when @scheme[follow-links?] is @scheme[#f],
 in which case the filesystem traversal does not follow links. If
 @scheme[follow-links?] is @scheme[#t], then @scheme[proc]
 will only get a @scheme['link] as a second argument when it
 encounters a dangling symbolic link (one that does not resolve to an
 existing file or directory).}

 @item{The third argument is the accumulated result. For the first
 call to @scheme[proc], the third argument is @scheme[init-val]. For the
 second call to @scheme[proc] (if any), the third argument is the result
 from the first call, and so on. The result of the last call to
 @scheme[proc] is the result of @scheme[fold-files].}

}

The @scheme[proc] argument is used in an analogous way to the
procedure argument of @scheme[foldl], where its result is used as the
new accumulated result.  There is an exception for the case of a
directory (when the second argument is @scheme['dir]): in this case
the procedure may return two values, the second indicating whether the
recursive scan should include the given directory or not.  If it
returns a single value, the directory is scanned.

An error is signaled if the @scheme[start-path] is provided but no
such path exists, or if paths disappear during the scan.}


@defproc[(make-directory* [path path-string?]) void?]{

Creates directory specified by @scheme[path], creating intermediate
directories as necessary.}


@defproc[(make-temporary-file [template string? "mztmp~a"]
                              [copy-from-filename (or/c path-string? false/c (one-of/c 'directory)) #f]
                              [directory (or/c path-string? false/c) #f])
         path?]{

Creates a new temporary file and returns a pathname string for the
file.  Instead of merely generating a fresh file name, the file is
actually created; this prevents other threads or processes from
picking the same temporary name.

The @scheme[template] argument must be a format string suitable
for use with @scheme[format] and one additional string argument (where
the string contains only digits). If the resulting string is a
relative path, it is combined with the result of
@scheme[(find-system-path 'temp-dir)], unless @scheme[directory] is
provided and non-@scheme[#f], in which case the
file name generated from @scheme[template] is combined with
@scheme[directory] to obtain a full path.

If @scheme[copy-from-filename] is provided as path, the temporary file
is created as a copy of the named file (using @scheme[copy-file]). If
@scheme[copy-from-filename] is @scheme[#f], the temporary file is
created as empty. If @scheme[copy-from-filename] is
@scheme['directory], then the temporary ``file'' is created as a
directory.

When a temporary file is created, it is not opened for reading or
writing when the pathname is returned. The client program calling
@scheme[make-temporary-file] is expected to open the file with the
desired access and flags (probably using the @scheme['truncate] flag;
see @scheme[open-output-file]) and to delete it when it is no longer
needed.}

@defproc[(get-preference [name symbol?]
                         [failure-thunk (-> any) (lambda () #f)]
                         [flush-mode any/c 'timestamp]
                         [filename (or/c string-path? false/c) #f])
         any]{

Extracts a preference value from the file designated by
@scheme[(find-system-path 'pref-file)], or by @scheme[filename] if it
is provided and is not @scheme[#f].  In the former case, if the
preference file doesn't exist, @scheme[get-preferences] attempts to
read a @filepath{plt-prefs.ss} file in the @filepath{defaults}
collection, instead. If neither file exists, the preference set is
empty.

The preference file should contain a symbol-keyed association list
(written to the file with the default parameter settings).  Keys
starting with @scheme[mzscheme:], @scheme[mred:], and @scheme[plt:] in
any letter case are reserved for use by PLT.

The result of @scheme[get-preference] is the value associated with
@scheme[name] if it exists in the association list, or the result of
calling @scheme[failure-thunk] otherwise.

Preference settings are cached (weakly) across calls to
@scheme[get-preference], using @scheme[(path->complete-path filename)]
as a cache key. If @scheme[flush-mode] is provided as @scheme[#f], the
cache is used instead of the re-consulting the preferences file. If
@scheme[flush-mode] is provided as @scheme['timestamp] (the default),
then the cache is used only if the file has a timestamp that is the
same as the last time the file was read. Otherwise, the file is
re-consulted.

See also @scheme[put-preferences]. For a more elaborate preference
system, see @scheme[preferences:get].}



@defproc[(put-preferences [names (listof symbol?)]
                          [vals list?]
                          [locked-proc (path? . -> . any) (lambda (p) (error ....))]
                          [filename (or/c false/c path-string?) #f])
         void?]{

Installs a set of preference values and writes all current values to
the preference file designated by @scheme[(find-system-path
'pref-file)], or @scheme[filename] if it is supplied and not
@scheme[#f].

The @scheme[names] argument supplies the preference names, and
@scheme[vals] must have the same length as @scheme[names]. Each
element of @scheme[vals] must be an instance of a built-in data type
whose @scheme[write] output is @scheme[read]able (i.e., the
@scheme[print-unreadable] parameter is set to @scheme[#f] while
writing preferences).

Current preference values are read from the preference file before
updating, and an update ``lock'' is held starting before the file
read, and lasting until after the preferences file is updated. The
lock is implemented by the existence of a file in the same directory
as the preference file. If the directory of the preferences file does
not already exist, it is created.

If the update lock is already held (i.e., the lock file exists), then
@scheme[locked] is called with a single argument: the path of the lock
file. The default @scheme[locked] reports an error; an alternative
thunk might wait a while and try again, or give the user the choice to
delete the lock file (in case a previous update attempt encountered
disaster).

If @scheme[filename] is @scheme[#f] or not supplied, and the
preference file does not already exist, then values read from the
@filepath{defaults} collection (if any) are written for preferences
that are not mentioned in @scheme[names].}

