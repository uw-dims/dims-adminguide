.. _tridentguide:

Trident
=======

This chapter introduces `Trident`_, a "Trusted Information Exchange
Toolkit" that facilitates the formation of trust groups, 
communication between members of trust groups, among other things.
This chapter will walk through the installation and configuration 
of Trident and its prerequisites. How to use Trident and its various 
features will be covered in a different section. 

.. TODO(mboggess)
.. todo::

     Add intersphinx link in the above paragraph to indicate
     where to find instructions on how to use Trident.
     :ref:`tbd:tbd`
..

.. _ansiblemanagedtrident:

Installing Trident with Ansible
-------------------------------

Prerequisites
^^^^^^^^^^^^^

The following items are necessary before installing Trident via
Ansible:

     * Access to and knowledge of how to use Ansible roles
       foundational to provisioning DIMS systems. More information
       about these roles can be found at tbd:tbd.

     * Host(s) provisioned by Ansible roles foundational to 
       DIMS systems. If using multiple hosts for a Trident
       instance, they must all be provisioned with these roles.

     * Access to and knowledge of how to use Ansible roles
       specific to standing up a working Trident instance. More 
       information about these roles can be found below, and 
       information about how to provision a host with them can
       be found at tbd:tbd.
       
     * Latest Trident package OR

     * Access to the github.com Trident repo

.. TODO(mboggess)
.. todo::

     Add intersphinx link in the above points to indicate
     where to find instructions on foundational DIMS Ansible
     roles and how to provision hosts with those roles.
     :ref:`tbd:tbd`
..

Trident Artifact Build Process
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. note::

     You must have access to the Trident github repo
     in order to build Debian packages. You must be
     able to clone the repo.

..

The following section outlines the steps needed to
obtain/update the Trident source code and build a Debian
package from it so that artifact is available for
use by the Ansible role.

#. Prerequisite environment, per Trident documentation
   on their ``DEV-1.3`` branch:

     * Debian Jessie

#. Prerequisite packages, per Trident documentation
   on their ``DEV-1.3`` branch:

     * build-essential

     * git

     * pbuilder

#. Additional packages required, not listed in Trident's
   documentation:

     * dh-systemd

     * golang-go

#. Also, not listed in Trident's "build" requirements list,
   you must have Go installed. In Trident's "runtime"
   requirements list, it says version 1.5.1+, so I have
   downloaded and installed version 1.5.1.

   .. code-block:: none

        $ cd /usr/local
        $ wget https://storage.googleapis.com/golang/go1.5.1.linux-amd64.tar.gz
        $ sudo tar -xzf go1.5.5.linux-amd64.tar.gz
        $ export PATH=$PATH:/usr/local/go/bin

   ..

#. If you have a copy of the Trident source code, determine
   which version it is by running

   .. code-block:: none

        $ /usr/sbin/tridentd --version

   ..

#. Compare this with the latest version of Trident source
   code on GitHub. This is a little tricky because there
   is a mismatch of version numbers between the ``debian/changelog``
   file in the repo and the tags and branch names.

   As of 13 Jul 2016, the official latest version is 1.2.0.

   Go to the `Trident repo` on the master branch and go to
   the ``debian/changelog`` file. Here you will see the
   latest version.

#. Update or retrieve source code from GitHub. This may be
   a ``git clone`` or a ``git pull`` depending on how you
   are utilizing the Trident source (whether you need it
   once or if you are forking the repo).

#. In root directory of Trident git source, build the package:

   .. code-block:: none

       $ dpkg-buildpackage -b -uc -us

   ..

   This will build the binaries one level up from the trident root dir.

   .. note::

       The ``dpkg-buildpackage`` command will prompt you for
       your github username and password.

   ..

   .. note::

       The ``dpkg-buildpackage`` command runs a script called 
       ``doc/deps.sh`` which has a plethora for "cannot find
       package X" errors. This is a known issue, see
       https://github.com/bapril/trident/issues/371. It still
       seems to build a usable artifact...

   ..

#. Place debian package wherever your Ansible role retrieves
   the package from for installation.


Provisioning Process
^^^^^^^^^^^^^^^^^^^^

The following section outlines the steps needed to
provision a host to stand up a working Trident instance.

#. Ensure all variables for your deployment are set to
   the correct values. In particular, ensure any 
   Trident-Postgres-Nginx-Postfix networking
   variables are set correctly.

#. Apply the ``postgres`` Ansible role.

#. Apply the ``nginx`` Ansible role.

#. Apply the ``postfix`` Ansible role.

#. Apply the ``trident`` Ansible role.

Once all the roles have been applied, on the nginx host,
you should be able to browse to the proxy address and see
the Trident homepage. Instructions about how to actually
use Trident and set up trust groups, etc. can be found
at tbd:tbd.

.. TODO(mboggess)
.. todo::

     Add intersphinx link in the above points to indicate
     where to find instructions on how to use Trident.
     :ref:`tbd:tbd`
..

.. _tridentprerequisites:

Trident Prerequisites
---------------------

The following are prerequisites that must be installed and configured
before installing and configuring Trident:

* PostgreSQL 9.1+ database
* Postfix
* Nginx

.. _tridentpostgres:

PostgreSQL Database
^^^^^^^^^^^^^^^^^^^

The `Trident documentation`_ gives instructions on how to set up both a local
postgres server and Trident database, as well as a remote server and database. In this
section, we will cover and expand the instructions for installing and configuring
a remote postgres server and Trident database. See Trident's documentation page
for a local installation and configuration.

For remote postgres servers, the `Trident documentation`_ recommends temporarily
installing Trident on the remote target on which the postgres server will reside,
use Trident's ``tsetup`` command to create and setup the Trident database, then
remove the Trident package.

.. note::

    The "In a nutshell" steps in the "Remote Database" section of the `Trident documentation`_
    seem to conflict with each other and the steps outlined in the "Local Database" section,
    which the location should really be the only thing that differentiates the two, I believe.

    The following is my best interpretation, though it is just that, my interpretation. Notes
    and todo blocks follow at steps where I'm interpreting.
..

Essentially, the following steps would need to occur on the remote target:

#. Install PostgreSQL 9.1+

#. Create the *system* ``trident`` user

   .. TODO(mboggess)
   .. todo::

       In the `Trident documentation`_ on the "Remote Database", it says that the ``trident``
       user needs to be created on the remote server. Is this different from the PostgreSQL
       ``trident`` user? (A system user vs. an application's user?) Thus, a *system* ``trident``
       user needs to be created to run the Trident ``tsetup`` command, which then creates
       the PostgreSQL ``trident`` user.

       I'm writing these docs under the assumption that there needs to be a *system* ``trident``
       user to run the ``tsetup`` command, which will then create a *PostgreSQL* ``trident``
       user.

   ..

#. Temporarily install the Trident package(s).

    .. TODO(mboggess)
    .. todo::

       Currently, the Trident dockerfile ($GIT/dims-dockerfiles/dockerfiles/trident/Dockerfile)
       retrieves the debian packages from ``source.prisem.washington.edu`` and then uses
       ``dpkg`` to install them.

       However, in the `Trident documentation`_ section "Quick and Dirty", a command
       ``dpkg -i trident-VERSION.deb`` is given, without any indication anywhere as to what
       ``VERSION`` is.

       Will this be available or do we need to keep populating the package on ``source``?
       According to an email from Linda Parsons (see email in :ref:`emailsotherdocs`), there's
       a Docker container she built specifically for updating the Trident package on ``source``.

    ..

    .. note::

        Here is a confusing bit from the "nutshell" steps in the "Remote Database"
        section of the `Trident documentation`_. The first two steps are to "Create
        the ``trident`` user" and "Create the ``trident`` database``, and the last
        step is "Run ``tsetup`` from the remote server as normal". However, ``tsetup``
        *does* those two things (user and database creation).

        The third step says "Provide permissions for *the user* to access the database".
        I'm not sure *which* user this means--the PostgreSQL ``trident`` user, I'm
        assuming. I'm also assuming that since ``tsetup`` creates a ``trident`` user for
        PostgreSQL, it will also give it the appropriate permissions. (I'm assuming
        this because the "Local Database" section said nothing about giving anyone 
        appropriate permissions.)

        Perhaps I'm confused, and this step means give the *system* ``trident`` user
        appropriate permissions, but...I don't think the system user would be accessing
        the database.

        Either way, for now, until this is clarified, I'm "skipping" this step because
        it seems to be taken care of by another "step".

    ..

#. Properly configure the Trident daemon at ``/etc/trident/trident.conf``

   The following is a template of ``trident.conf``:
   
   .. literalinclude:: trident-trident.conf.j2

#. Properly configure the postgres ``pg_hba.conf`` file (location variable)

   The following is a template of ``pg_hba.conf``:
   
   .. literalinclude:: pg_hba.conf.j2

#. Ensure reachability of the database port defined in ``/etc/trident/trident.conf``

#. Create the Trident database using the following command: ``su - postgres -c "/usr/sbin/tsetup setup_db``

#. Remove the Trident packages

   .. TODO(mboggess)
   .. todo::

       If the system ``trident`` user had to be added to run
       the ``tsetup`` commands, should that user now be deleted?

   ..

.. _tridentnginx:

Nginx Webserver
^^^^^^^^^^^^^^^

#. Install Nginx

#. Properly configure ``/etc/nginx/conf.d/trident.conf``

   The following is a template of the nginx ``trident.conf``
   for a *production* system:
   
   .. literalinclude:: nginx-trident-production.conf.j2

   .. TODO(mboggess)
   .. todo::

       This still needs ssl certs, etc. 
   ..

   The following is a template of the nginx ``trident.conf``
   for a *development* system:
   
   .. literalinclude:: nginx-trident-development.conf.j2

   .. note::

       With this config, Nginx will only listen for the
       Trident daemon on an HTTP port (no HTTPS).

   ..

#. Properly configure Trident Daemon Upstream at ``/etc/trident/nginx/trident-upstream.inc``

   The following is a template of ``trident-upstream.inc``:
   
   .. literalinclude:: trident-upstream.inc.j2

#. Properly configure the Trident server at ``/etc/trident/nginx/trident-server.inc``

   The following is an example of ``trident-server.inc``:
   
   .. literalinclude:: trident-server.inc

   .. TODO(mboggess)
   .. todo::

      ``trident-server.inc`` may still need to be templated.

   ..

.. _tridentpostfix:

Postfix
^^^^^^^

#. Install Postfix

#. Know the answers to the following:

    * What type of mail configuration
    
    * The Fully Qualified Domain Name (FQDN) of your server

#. Properly configure Postfix's main config file at ``/etc/postfix/main.cf``

   The following is a template of ``main.cf``:
   
   .. literalinclude:: postfix-main.cf.j2

#. Properly configure ``/etc/aliases``

   The following is a template of ``aliases``:

   .. literalinclude:: aliases.j2

#. Might have to configure Postfix's master config file at ``/etc/postfix/master.cf``

   The following is an example of ``master.cf``:
   
   .. literalinclude:: postfix-master.cf

   .. TODO(mboggess)
   .. todo::

      ``master.cf`` may still need to be templated.

   ..

#. Might have to configure additional email addresses at ``/etc/postfix/virtual``

   The following is a template of ``virtual``:
   
   .. literalinclude:: postfix-virtual.j2

   .. TODO(mboggess)
   .. todo::

       I didn't template the emails/domains on the left side because 
       I wasn't really sure what they should be or if they were 
       related to any variables from other templates (particularly the
       domain--would that be the {{ tridentFQDN }} from the Trident
       daemon config? 

   ..

.. note::

    The `Trident documentation`_ gave the information used to configure
    the ``/etc/aliases`` file and the ``/etc/postfix/virtual`` file, 
    but then just said "Of course do configure the rest of Postfix
    properly." I don't really know what that means, so that's why
    I included the ``master.cf`` file, since that was included in the
    ``/etc/postfix`` dir. There are a couple other files there,
    ``/etc/postfix/dynamicmaps.cf`` and ``/etc/postfix/postfix-files``,
    along with a ``sasl/`` dir and a couple scripts. 

..


.. _tridentinstall:

Install Trident
---------------

Now we can install the Trident server and the Trident CLI.

#. Retrieve the Trident debian packages from ``source.prisem.washington.edu``

   .. code-block:: none

       $ wget http://source.prisem.washington.edu:8442/trident-server_1.0.3_amd64.deb
       $ wget http://source.prisem.washington.edu:8442/trident-cli_1.0.3_amd64.deb
       
   ..

   .. note::

       The version may change...the above commands need to be kept in sync.

   ..

#. Properly configure the Trident daemon at ``/etc/trident/trident.conf``

   This template can be seen in the :ref:`tridentpostgres` section.

#. Properly configure Trident daemon defaults at ``/etc/default/trident``

   The following is an example of ``/etc/default/trident``:
   
   .. literalinclude:: trident-default

   .. TODO(mboggess)
   .. todo::

      ``trident-default`` may still need to be templated.

   ..

.. _runningtrident:

Running Trident
---------------

There are several ways of running the Trident daemon, but we have divided
them into a "secure, non-debug" way and a "non-secure, debug" way.

* Insecure, debug:

    .. code-block:: none

        DAEMON_USER=trident /usr/sbin/tridentd \
           -insecurecookies \
           -disabletwofactor \
           -debug \
           -config /etc/trident/ \
           -daemonize \
           -syslog \
           -verbosedb

    ..

* Secure, non-debug:

    .. code-block:: none

        DAEMON_USER=trident /usr/sbin/tridentd \
           -config /etc/trident/ \
           -daemonize \

    ..

.. note::

    * The above code is from a start script used by the Dockerfile
      created by Linda Parsons ($GIT/dims-dockerfiles/dockerfiles/trident/conf/start.sh). 
      I just grabbed it to show how to run the daemon. We should 
      probably always have syslog enabled...

    * There's a note in that start script that says using the
      ``daemonize`` flag doesn't appear to be daemonizing the
      Trident daemon. Should keep that in mind.

    .. todo::

        * Probably all of these services should be controlled by supervisor 
          or something.

    ..

..


.. _emailsotherdocs:

Emails and other non-official documentation
-------------------------------------------

* Email from Linda in response to Megan asking for any additional documentation.

.. literalinclude:: tridentemailsandanyotherdocumentation.txt

* There is an Ansible role called ``trident-docker-deploy`` located in
  ``$GIT/ansible-playbooks/roles``. This roles creates a volume container to
  be paired with a DIMS postgres container (if it doesn't already exist), and
  a DIMS postgres container and DIMS Trident container.

  The Dockerfiles and related files and scripts for these containers can
  be viewed at:

      * Postgres: ``$GIT/dims-dockerfiles/dockerfiles/postgres-trident``

      * Trident:  ``$GIT/dims-dockerfiles/dockerfiles/trident``

* Additionally, Linda created a couple "helper" containers. One container
  updates ``source.prisem.washington.edu`` and another builds off the 
  "fresh-install" DIMS postgres container to install a copy of the DIMS
  OPS-Trust database. 

  These can be viewed at:

      * Build: ``$GIT/dims-dockerfiles/dockerfiles/trident-build``

      * Original Database: ``$GIT/dims-dockerfiles/dockerfiles/postgres-trident-clone``


.. _Trident: https://trident.li
.. _Trident documentation: https://trident.li/doc/
.. _Trident repo: https://github.com/bapril/trident