==========
 dbt5-run
==========

---------------
Database Test 5
---------------

:Date: 2024-04-23

:Manual section: 1
:Manual group: Database Test 5 0.9.4 Documentation
:Version: Database Test 5 0.9.4

SYNOPSIS
========

**dbt5-run** [option...] dbms directory

DESCRIPTION
===========

**dbt5-run** is a wrapper script that helps execute a test.  It is intended to
be an easier way to execute the DBT-5 workload than running the individual
components by hand, but does not provide all of the flexibility that can be
achieved by running the individual components by hand.  It additionally aids in
collecting operating system and database statistics as well as capturing
software profiles if the user so desires.

OPTIONS
=======

-b PARAMETERS  Database *parameters*.
-c CUSTOMERS  Active *customers*, default 5000.
-d SECONDS  Test duration in *seconds*.
--dbaas  Flag to signify that the database is a service so only collect
        database statistics.
-f SCALE_FACTOR  Default 500.
--help  This usage message.  Or **-?**.
-h HOSTNAME  Database *hostname*, default localhost.
-l DELAY  Pacing *delay* in seconds, default 0.
-n NAME  Database *name*, default dbt5.
--privileged  Run test as a privileged database user.
-p PORT  Database *port* number.
-r SEED  Random number *seed*, using this invalidates test.
--stats  Collect system stats and profile.
-s DELAY  *delay* between starting threads in milliseconds, default 1000.
--tpcetools=EGENHOME  *egenhome* is the directory location of the TPC-E Tools
-t CUSTOMERS  Total *customers*, default 5000.
-u USERS  Number of *users* to emulate, default 1.
-V, --version  output version information, then exit
-w DAYS  Initial trade *days*, default 300.
-z COMMENT  *comment* describing this test run.

*dbms* options are:

* pgsql  PostgreSQL

*directory* is the path to save test results.

EXAMPLES
========

Run a quick 120 second (2 minute) test with 1 user::

    dbt5 run -d 120 pgsql /tmp/results

TOML CONFIG FILE
----------------

This is an example::

    # DBT-5 Test Execution Example

    # General settings:

    # Path to built TPC-E tools.
    #tpcetools = "/opt/egen"

    # How long to run the test in seconds.
    duration = 120

    # Total number of customers.
    #customers = 5000

    # Database name.
    #database_name = "dbt5"

    # Days of initial trades.
    #itd = 300

    # Scale factor, i.e. number of customers per TRTPS
    #scale_factor = 500

    # Need at least one Driver.
    [[driver]]
    # Driver server hostname of IP to connect to.
    driver_addr = "driver1"

    # Brokerage House server hostname or IP address to connect to.
    brokerage_addr = "brokerage1"

    # Brokerage House port.
    #brokerage_port = 30000

    # Number of users to drive workload.
    users = 1

    # Milliseconds of sleep between users starting.
    #user_creation_delay = 1000

    # Need at least one Market Exchange.
    [[market]]
    # Market Exchange server hostname of IP address to start Market Exchange.
    market_addr = "market1"

    # Market Exchange server port.
    #market_port = 30010

    # Brokerage House server hostname or IP address to connect to.
    brokerage_addr = "brokerage1"

    # Brokerage House port.
    #brokerage_port = 30000

    # Need at least one Brokerage House.

    [[brokerage]]
    # Brokerage House server hostname or IP address to connect to.
    brokerage_addr = "brokerage1"

    # Brokerage House port.
    #brokerage_port = 30000

    # Database server hostname or IP address to connect to.
    database_addr = "db1"

    # Database port
    #database_port = 5432

    # Market Exchange server hostname of IP address to connect to.
    market_addr = "market1"

    # Market Exchange port.
    #market_port = 30010

    # Define additional database servers not previously defined in
    # [[brokerage]] sections for stats collection.
    #[[database]]
    #database_addr = "database2"

SEE ALSO
========

**dbt5**\ (1)
