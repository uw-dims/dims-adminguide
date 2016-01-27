.. _diagnosingcoreos:

Managing CoreOS with Systemd and Other Tools
============================================

This chapter covers using ``systemctl`` commands and other
debugging commands and services for diagnosing problems
on a CoreOS system.

CoreOS uses `systemd`_ as both a system and service manager and
as an init system. The tool `systemctl`_ has many commands 
which allow a user to look at and control the state of systemd.

This is by no means an exhaustive list or description of the
potential of any of the tools described here, merely an overview
of tools and their most useful services. See the links provided
within this chapter for more information. For more debugging
information relevant to DIMS, see "Debugging CoreOS" section 
of :ref:`dimsdockerfiles:dockerincoreos`.

.. TODO(mboggess)
.. todo::

   Replace "Debugging CoreOS" with intersphinx link when dims-617
   feature branch has been merged into develop branch.

..

.. _systemd: http://www.freedesktop.org/wiki/Software/systemd/
.. _systemctl: http://www.freedesktop.org/software/systemd/man/systemctl.html

.. _stateofsystemd:

State of systemd
----------------

There are a few ways to check on the state of systemd, as a whole system.

#. Check all running units and their state on a node at once.

   .. literalinclude:: systemctl.txt
      :emphasize-lines: 1, 2, 122, 123, 124, 126
      :linenos:

   This shows all loaded units and their state, as well as a brief 
   description of the units. 

#. For a slightly more organized look at the state of a node, along with
   a list of failed unites, queued jobs, and a process tree based on CGroup:

   .. literalinclude:: systemctlstatus1.txt
      :emphasize-lines: 5, 7, 8, 9
      :linenos:

   This shows the status of the node (line 7), how many jobs are queued 
   (line 8), and any failed units (line 9). It also shows which services
   have started, and what command they are running at the time this status
   "snapshot" was taken. 

   .. literalinclude:: systemctldockerps.txt
      :emphasize-lines: 3, 4, 5, 68
      :linenos:

   This shows the status of another node in the cluster at a different 
   point in the startup process. It still shows the status of the node, the
   number of jobs queued and failed units, but there are a lot more services
   in the process tree. Finally, at line 68, you see how to check on the 
   status of active, running Docker containers. 

   .. note::

      If ``docker ps`` seems to "hang", this generally means there is one
      or more Docker containers trying to get started. Just be patient, 
      and it/they should show up. To check that the Docker daemon is
      indeed running, try to run "docker info". It might also hang until
      whatever activating container starts up, but as long as it doesn't
      return immediately with "Cannot connect to the Docker daemon. Is the
      docker daemon running on this host?", Docker is working, just be
      patient.

      If ``docker ps`` doesn't hang but shows up with just headings and no
      containers, but you are expecting there to be containers, run 
      ``docker ps -a``. This will show all docker containers, even ones 
      that have failed for some reason.

   .. 

#. systemd logs output to its `journal`_. The journal is queried by a tool called
   ``journalctl``. To see all journal output of all systemd processes since the
   node was created, run

   ``journalctl``

   This is a lot of output, so it won't be shown here. Use this tool to see
   output of all the things in one gigantic set. Particularly useful if 
   you're trying to see how different services might be affecting each other.

#. To only see journal output for the last boot, run

   ``journalctl -b``

   Same type of output as ``journalctl``, but only since the last boot.

.. _journal: http://www.freedesktop.org/software/systemd/man/journalctl.html

.. _stateofsysunits:

State of systemd units
----------------------

All services run on a node with systemd are referred to as units. You can 
check the state of these units individually.

#. Check the status of a unit and get the tail of its log output.

   .. literalinclude:: sysunitstatus.txt
      :emphasize-lines: 1, 3, 5, 9, 12, 14, 16, 20, 43, 57
      :linenos:

   The ``-l`` is important as the output will be truncated without it.

   This command also shows a multitude of things. It gives you a unit's
   state as well as from what unit file location a unit is run. Unit
   files can be placed in multiple locations, and they are run according
   to a hierarchy, but the file shown by here (line 3) is the one that
   systemd actually runs.

   This command also shows the status of any commands used in the
   stopping or starting of a service (i.e., all the ExecStart* or 
   ExecStop* directives in a unit file). See lines 9, 12, 14, 16. This
   is particularly useful if you have Exec* directives that could be
   the cause of a unit failure. 

   The command run from the ExecStart directive is shown, starting at
   line 20.

   Finally, this command gives essentially the tail of the service's
   journal output. As you can see at line 57, a Consul leader was 
   elected!

#. To see the unit file systemd runs, run 

   .. literalinclude:: sysunitcat.txt
      :emphasize-lines: 1, 2, 3, 8, 40
      :linenos:

   This command shows the service's unit file directives. It also
   shows at the top (line 2) the location of the file. In this 
   unit file, there are directives under three headings, "Unit",
   "Service", and "Install". To learn more about what can go in
   each of these sections of a unit file, see freedesktop.org's 
   page on `systemd unit files`_.

#. To make changes to a unit file, run

   ``systemctl edit consul.service``

   This will actually create a brand new file to which you can add or
   override directives to the unit definition. For slightly more 
   information, see DigitalOcean's `How to Use Systemctl to Manage 
   Systemd Services and Units`_.

#. You can also edit the actual unit file, rather than just creating
   an override file by running

   ``systemctl edit --full consul.service``

#. systemd unit files have many `directives`_ used to configure the 
   units. Some of these are set or have defaults that you may not be
   aware of. To see a list of the directives for a given unit and
   what these directives are set to, run

   .. literalinclude:: sysunitshow.txt
      :emphasize-lines: 1
      :linenos:

#. To see all logs of a given unit since the node was created, run

   ``journalctl -u consul.service``

#. Watch the logs of a given unit since the last reboot, run

   ``journalctl -b -u consul.service``

#. Watch the tail of the logs of a unit.

   ``journalctl -fu consul.service``

#. To see logs with explanation texts, run

   .. literalinclude:: journalunitbxu.txt
      :emphasize-lines: 1, 2, 3
      :linenos:

   Line 2 says what the date/time range of possible logs exist,
   but as you can see in line 3, the first log in this set is not
   a Jan 26 date, as could be possible according to line 2, but a 
   Jan 27 date, which is the last time this node was rebooted.

   This service started up just fine, so there's no failures to point
   out, but this is where you'd find them and any possible explanation
   for those failures.
   
#. If the unit you are running is running a Docker container, all 
   relevant and helpful information may not be available to you via
   ``journalctl``. To see logs from the Docker container itself, run

   .. literalinclude:: dockerlogsunit.txt
      :emphasize-lines: 1
      :linenos:

   This is generally the same output what you can get from ``journalctl``,
   but I think I have found other information in the docker logs than
   ``journalctl`` by itself.

   .. note::
   
      The name of the systemd service and the name of the Docker 
      container might NOT be the same. They *can* be the same.
      However, if, as in this example, you name your service
      "foo" so the service is "foo.service", and you name your
      Docker container "foo-$hostname", running ``docker logs
      foo.service`` or ``docker logs foo`` will not work. Don't
      get upset with Docker when it tells you there's no such
      container "foo.service" when you named a container 
      "foo-$hostname". :)

   ..

#. To follow the logs in real time, run

   ``docker logs -f consul-core-01``

.. _systemd unit files: http://www.freedesktop.org/software/systemd/man/systemd.unit.html
.. _How to Use Systemctl to Manage Systemd Services and Units: https://www.digitalocean.com/community/tutorials/how-to-use-systemctl-to-manage-systemd-services-and-units
.. _directives: http://www.freedesktop.org/software/systemd/man/systemd.directives.html

.. _managingsysunits:

Managing systemd units
----------------------

#. You can start, stop, restart, and reload units with

   ``sudo systemctl {start|stop|reload|restart} consul.service``

   You must run with sudo. 

   The "reload" option works for units which can reload their 
   configurations without restarting.

#. When you make changes to a unit and are going to restart that
   unit, first you must let the system daemon know that changes are 
   happening:

   ``sudo systemctl daemon-reload``

.. warning::

   This may seem obvious, but it's a good thing to remember: if 
   a systemd unit is running a Docker container, if you restart the unit,
   this doesn't necessarily mean the Docker container gets removed
   and you get a new container when the unit is restarted. 

..
