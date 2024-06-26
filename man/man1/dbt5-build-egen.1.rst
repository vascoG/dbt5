=================
 dbt5-build-egen
=================

---------------
Database Test 5
---------------

:Date: 2024-04-23

:Manual section: 1
:Manual group: Database Test 5 0.9.4 Documentation
:Version: Database Test 5 0.9.4

SYNOPSIS
========

**dbt5-build-egen** [option...] directory

DESCRIPTION
===========

**dbt5-build-egen** builds the TPC-E Tools with DBT-5 sponsored code.  It
applies patches and copies the sponsored code into the TPC-E Tools directory
before building TPC-E Tools with appropriate compiler and linker flags.

Proofed against TPC-E Tools v1.14.0.

OPTIONS
=======

--build-only  Do not patch TPC-E Tools before building
--debug  Build with -DDEBUG
--include-dir=DIRECTORY  *directory* of any custom code to build with TPC-E
        Tools
--patch-dir=DIRECTORY  *directory* of patches to apply to TPC-E Tools,
        overrides *patchdir* environment variable, default
        /usr/share/dbt5/patches
--source-dir=DIRECTORY  *directory* of code to copy to TPC-E Tools, overrides
        *sourcedir* environment variable, default /usr/share/dbt5/src
-V, --version  output version information, then exit
--help  This usage message.  Or **-?**.

EXAMPLES
========

The TPC-E Tools needs to be downloaded from the TPC:
https://www.tpc.org/tpc_documents_current_versions/current_specifications5.asp

The TPC-E Tools needs to be uncompressed into its own directory before
building::

    mkdir /tmp/egen
    cd /tmp/egen
    unzip /path/to/*-tpc-e-tool.zip
    dbt5-build-egen /tmp/egen

SEE ALSO
========

**dbt5**\ (1)
