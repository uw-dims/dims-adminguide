.. _diagnosis:

Diagnosing System Problems and Outages
======================================

.. _usingdimscli:

Using ``dimscli``
-----------------

This chapter covers using ``dimscli`` as a distributed
shell for diagnosing problems throughout a DIMS deployment.

Ansible has two primary CLI programs, ``ansible`` and
``ansible-playbook``. Both of these programs are passed a
set of hosts on which they are to operate using an
`Inventory`_.

.. _Inventory: http://docs.ansible.com/ansible/intro_inventory.html

.. note::

    Read about Ansible and how it is used by the DIMS project
    in Section :ref:`ansibleplaybooks:ansiblefundamentals` of
    :ref:`ansibleplaybooks:ansibleplaybooks`.

..

.. code-block:: none

    [dimsenv] dittrich@dimsdemo1:~/dims/git/python-dimscli (develop*) $ cat complete_inventory
    [all]
    floyd2-p.prisem.washington.edu
    foswiki-int.prisem.washington.edu
    git.prisem.washington.edu
    hub.prisem.washington.edu
    jenkins-int.prisem.washington.edu
    jira-int.prisem.washington.edu
    lapp-int.prisem.washington.edu
    lapp.prisem.washington.edu
    linda-vm1.prisem.washington.edu
    rabbitmq.prisem.washington.edu
    sso.prisem.washington.edu
    time.prisem.washington.edu
    u12-dev-svr-1.prisem.washington.edu
    u12-dev-ws-1.prisem.washington.edu
    wellington.prisem.washington.edu

..

Using this inventory, the modules ``command`` and ``shell`` can be used to run commands
as needed to diagnose all of these hosts at once.

.. code-block:: none

    [dimsenv] dittrich@dimsdemo1:~/dims/git/python-dimscli (develop*) $ dimscli ansible command --program "uptime" --inventory complete_inventory --remote-port 8422 --remote-user dittrich
    +-------------------------------------+--------+-------------------------------------------------------------------------+
    | Host                                | Status | Results                                                                 |
    +-------------------------------------+--------+-------------------------------------------------------------------------+
    | rabbitmq.prisem.washington.edu      | GOOD   |  22:07:53 up 33 days,  4:32,  1 user,  load average: 0.07, 0.13, 0.09   |
    | wellington.prisem.washington.edu    | GOOD   |  22:07:57 up 159 days, 12:16,  1 user,  load average: 1.16, 0.86, 0.58  |
    | linda-vm1.prisem.washington.edu     | GOOD   |  22:07:54 up 159 days, 12:03,  1 user,  load average: 0.00, 0.01, 0.05  |
    | git.prisem.washington.edu           | GOOD   |  22:07:54 up 159 days, 12:03,  2 users,  load average: 0.00, 0.01, 0.05 |
    | time.prisem.washington.edu          | GOOD   |  22:07:55 up 33 days,  4:33,  2 users,  load average: 0.01, 0.07, 0.12  |
    | jenkins-int.prisem.washington.edu   | GOOD   |  22:07:55 up 159 days, 12:03,  1 user,  load average: 0.00, 0.01, 0.05  |
    | u12-dev-ws-1.prisem.washington.edu  | GOOD   |  22:07:56 up 159 days, 12:03,  1 user,  load average: 0.00, 0.02, 0.05  |
    | sso.prisem.washington.edu           | GOOD   |  22:07:56 up 159 days, 12:03,  1 user,  load average: 0.00, 0.01, 0.05  |
    | lapp-int.prisem.washington.edu      | GOOD   |  22:07:54 up 159 days, 12:04,  2 users,  load average: 0.00, 0.01, 0.05 |
    | foswiki-int.prisem.washington.edu   | GOOD   |  22:07:55 up 159 days, 12:04,  1 user,  load average: 0.00, 0.01, 0.05  |
    | u12-dev-svr-1.prisem.washington.edu | GOOD   |  22:07:59 up 155 days, 14:56,  1 user,  load average: 0.05, 0.08, 0.06  |
    | hub.prisem.washington.edu           | GOOD   |  06:07:53 up 141 days, 12:19,  1 user,  load average: 0.08, 0.03, 0.05  |
    | floyd2-p.prisem.washington.edu      | GOOD   |  22:07:53 up 33 days,  4:32,  1 user,  load average: 0.00, 0.01, 0.05   |
    | jira-int.prisem.washington.edu      | GOOD   |  22:07:54 up 159 days, 12:03,  2 users,  load average: 0.00, 0.01, 0.05 |
    | lapp.prisem.washington.edu          | GOOD   |  22:07:54 up 159 days, 12:04,  2 users,  load average: 0.00, 0.01, 0.05 |
    +-------------------------------------+--------+-------------------------------------------------------------------------+

..


.. code-block:: none
   :linenos:
   :emphasize-lines: 22,23,26,27

    To: dims-devops@uw.ops-trust.net
    From: Jenkins <dims@eclipse.prisem.washington.edu>
    Subject: [dims devops] [Jenkins] [FAILURE] jenkins-update-cifbulk-server-develop-16
    Date: Thu Jan 14 20:35:21 PST 2016
    Message-ID: <20160115043521.C7D5E1C004F@jenkins>
    
    Started by an SCM change
    [EnvInject] - Loading node environment variables.
    Building in workspace /var/lib/jenkins/jobs/update-cifbulk-server-develop/workspace
    
    Deleting project workspace... done
    
    [ssh-agent] Using credentials ansible (Ansible user ssh key - root)
    [ssh-agent] Looking for ssh-agent implementation...
    [ssh-agent]   Java/JNR ssh-agent
    [ssh-agent] Started.

     ...
    
    TASK: [cifbulk-server | Make config change available and restart if updating existing] *** 
    <rabbitmq.prisem.washington.edu> REMOTE_MODULE command . /opt/dims/envs/dimsenv/bin/activate && supervisorctl -c /etc/supervisord.conf reread #USE_SHELL
    failed: [rabbitmq.prisem.washington.edu] => (item=reread) => {"changed": true, "cmd": ". /opt/dims/envs/dimsenv/bin/activate && supervisorctl -c /etc/supervisord.conf reread", "delta": "0:00:00.229614", "end": "2016-01-14 20:34:49.409784", "item": "reread", "rc": 2, "start": "2016-01-14 20:34:49.180170"}
    stderr: Error: could not find config file /etc/supervisord.conf
    For help, use /usr/bin/supervisorctl -h
    <rabbitmq.prisem.washington.edu> REMOTE_MODULE command . /opt/dims/envs/dimsenv/bin/activate && supervisorctl -c /etc/supervisord.conf update #USE_SHELL
    failed: [rabbitmq.prisem.washington.edu] => (item=update) => {"changed": true, "cmd": ". /opt/dims/envs/dimsenv/bin/activate && supervisorctl -c /etc/supervisord.conf update", "delta": "0:00:00.235882", "end": "2016-01-14 20:34:50.097224", "item": "update", "rc": 2, "start": "2016-01-14 20:34:49.861342"}
    stderr: Error: could not find config file /etc/supervisord.conf
    For help, use /usr/bin/supervisorctl -h
    
    FATAL: all hosts have already failed -- aborting
    
    PLAY RECAP ******************************************************************** 
               to retry, use: --limit @/var/lib/jenkins/cifbulk-server-configure.retry
    
    rabbitmq.prisem.washington.edu : ok=11   changed=4    unreachable=0    failed=1   
    
    Build step 'Execute shell' marked build as failure
    [ssh-agent] Stopped.
    Warning: you have no plugins providing access control for builds, so falling back to legacy behavior of permitting any downstream builds to be triggered
    Finished: FAILURE
    -- 
    [[ UW/DIMS ]]: All message content remains the property of the author
    and must not be forwarded or redistributed without explicit permission.

..


.. code-block:: none

    [dimsenv] dittrich@dimsdemo1:~/dims/git/ansible-playbooks (develop*) $ grep -r supervisord.conf
    roles/supervisor-install/tasks/main.yml:  template: "src=supervisord.conf.j2 dest={{ dims_supervisord_conf }} owner=root group=root"
    roles/supervisor-install/tasks/main.yml:  file: path=/etc/dims-supervisord.conf state=absent
    roles/supervisor-install/templates/supervisor.j2:DAEMON_OPTS="-c {{ dims_supervisord_conf }} $DAEMON_OPTS"
    roles/cifbulk-server/tasks/main.yml:  shell: ". {{ dimsenv_activate }} && supervisorctl -c {{ dims_supervisord_conf }} {{ item }}"
    roles/cifbulk-server/tasks/main.yml:  shell: ". {{ dimsenv_activate }} && supervisorctl -c {{ dims_supervisord_conf }} start {{ name_base }}:"
    roles/prisem-scripts-deploy/tasks/main.yml:  shell: ". {{ dimsenv_activate }} && supervisorctl -c {{ dims_supervisord_conf }} restart {{ item }}:"
    roles/anon-server/tasks/main.yml:  shell: ". {{ dimsenv_activate }} && supervisorctl -c {{ dims_supervisord_conf }} {{ item }}"
    roles/anon-server/tasks/main.yml:  shell: ". {{ dimsenv_activate }} && supervisorctl -c {{ dims_supervisord_conf }} start {{ name_base }}:"
    roles/consul-install/tasks/main.yml:  shell: ". {{ dimsenv_activate }} && supervisorctl -c {{ dims_supervisord_conf }} remove {{ consul_basename }}"
    roles/consul-install/tasks/main.yml:  shell: ". {{ dimsenv_activate }} && supervisorctl -c {{ dims_supervisord_conf }} {{ item }}"
    roles/consul-install/tasks/main.yml:  shell: ". {{ dimsenv_activate }} && supervisorctl -c {{ dims_supervisord_conf }} start {{ consul_basename }}:"
    roles/crosscor-server/tasks/main.yml:  shell: ". {{ dimsenv_activate }} && supervisorctl -c {{ dims_supervisord_conf }} {{ item }}"
    roles/crosscor-server/tasks/main.yml:  shell: ". {{ dimsenv_activate }} && supervisorctl -c {{ dims_supervisord_conf }} start {{ name_base }}:"
    group_vars/all:dims_supervisord_conf: '/etc/supervisord.conf'

..

.. code-block:: none

    [dimsenv] dittrich@dimsdemo1:~/dims/git/python-dimscli (develop*) $ dimscli ansible shell --program "find /etc -name supervisord.conf" --inventory complete_inventory --remote-port 8422 --remote-u
    ser dittrich
    +-------------------------------------+--------+----------------------------------+
    | Host                                | Status | Results                          |
    +-------------------------------------+--------+----------------------------------+
    | rabbitmq.prisem.washington.edu      | GOOD   | /etc/supervisor/supervisord.conf |
    | wellington.prisem.washington.edu    | GOOD   |                                  |
    | hub.prisem.washington.edu           | GOOD   |                                  |
    | git.prisem.washington.edu           | GOOD   | /etc/supervisor/supervisord.conf |
    | u12-dev-ws-1.prisem.washington.edu  | GOOD   |                                  |
    | sso.prisem.washington.edu           | GOOD   |                                  |
    | jenkins-int.prisem.washington.edu   | GOOD   | /etc/supervisor/supervisord.conf |
    | foswiki-int.prisem.washington.edu   | GOOD   |                                  |
    | lapp-int.prisem.washington.edu      | GOOD   |                                  |
    | u12-dev-svr-1.prisem.washington.edu | GOOD   | /etc/supervisor/supervisord.conf |
    | linda-vm1.prisem.washington.edu     | GOOD   |                                  |
    | lapp.prisem.washington.edu          | GOOD   |                                  |
    | floyd2-p.prisem.washington.edu      | GOOD   |                                  |
    | jira-int.prisem.washington.edu      | GOOD   | /etc/supervisor/supervisord.conf |
    | time.prisem.washington.edu          | GOOD   |                                  |
    +-------------------------------------+--------+----------------------------------+

..


.. code-block:: none

    [dimsenv] dittrich@dimsdemo1:~/dims/git/python-dimscli (develop*) $ dimscli ansible shell --program "find /etc -name '*supervisor'*" --inventory complete_inventory --remote-port 8422 --remote-use
    r dittrich
    +-------------------------------------+--------+-------------------------------------------------+
    | Host                                | Status | Results                                         |
    +-------------------------------------+--------+-------------------------------------------------+
    | rabbitmq.prisem.washington.edu      | GOOD   | /etc/rc0.d/K20supervisor                        |
    |                                     |        | /etc/rc3.d/S20supervisor                        |
    |                                     |        | /etc/rc1.d/K20supervisor                        |
    |                                     |        | /etc/default/supervisor                         |
    |                                     |        | /etc/rc2.d/S20supervisor                        |
    |                                     |        | /etc/rc6.d/K20supervisor                        |
    |                                     |        | /etc/supervisor                                 |
    |                                     |        | /etc/supervisor/supervisord.conf.20140214204135 |
    |                                     |        | /etc/supervisor/supervisord.conf.20140214200547 |
    |                                     |        | /etc/supervisor/supervisord.conf.20140616162335 |
    |                                     |        | /etc/supervisor/supervisord.conf.20140814132409 |
    |                                     |        | /etc/supervisor/supervisord.conf.20140616162451 |
    |                                     |        | /etc/supervisor/supervisord.conf.20140616162248 |
    |                                     |        | /etc/supervisor/supervisord.conf.20140131230939 |
    |                                     |        | /etc/supervisor/supervisord.conf.20140222154901 |
    |                                     |        | /etc/supervisor/supervisord.conf.20140214194415 |
    |                                     |        | /etc/supervisor/supervisord.conf.20140222155042 |
    |                                     |        | /etc/supervisor/supervisord.conf.20150208174308 |
    |                                     |        | /etc/supervisor/supervisord.conf.20140814132717 |
    |                                     |        | /etc/supervisor/supervisord.conf.20140215134451 |
    |                                     |        | /etc/supervisor/supervisord.conf.20150208174742 |
    |                                     |        | /etc/supervisor/supervisord.conf.20140911193305 |
    |                                     |        | /etc/supervisor/supervisord.conf.20140219200951 |
    |                                     |        | /etc/supervisor/supervisord.conf.20140911202633 |
    |                                     |        | /etc/supervisor/supervisord.conf                |
    |                                     |        | /etc/supervisor/supervisord.conf.20140222154751 |
    |                                     |        | /etc/supervisor/supervisord.conf.20150208174403 |
    |                                     |        | /etc/supervisor/supervisord.conf.20140814132351 |
    |                                     |        | /etc/supervisor/supervisord.conf.20140814132759 |
    |                                     |        | /etc/rc4.d/S20supervisor                        |
    |                                     |        | /etc/init.d/supervisor                          |
    |                                     |        | /etc/rc5.d/S20supervisor                        |
    | wellington.prisem.washington.edu    | GOOD   |                                                 |
    | linda-vm1.prisem.washington.edu     | GOOD   | /etc/rc0.d/K20supervisor                        |
    |                                     |        | /etc/rc3.d/S20supervisor                        |
    |                                     |        | /etc/rc1.d/K20supervisor                        |
    |                                     |        | /etc/rc2.d/S20supervisor                        |
    |                                     |        | /etc/rc6.d/K20supervisor                        |
    |                                     |        | /etc/supervisor                                 |
    |                                     |        | /etc/rc4.d/S20supervisor                        |
    |                                     |        | /etc/dims-supervisord.conf                      |
    |                                     |        | /etc/init.d/supervisor                          |
    |                                     |        | /etc/rc5.d/S20supervisor                        |
    | git.prisem.washington.edu           | GOOD   | /etc/rc0.d/K20supervisor                        |
    |                                     |        | /etc/rc3.d/S20supervisor                        |
    |                                     |        | /etc/rc1.d/K20supervisor                        |
    |                                     |        | /etc/default/supervisor                         |
    |                                     |        | /etc/rc2.d/S20supervisor                        |
    |                                     |        | /etc/rc6.d/K20supervisor                        |
    |                                     |        | /etc/supervisor                                 |
    |                                     |        | /etc/supervisor/supervisord.conf                |
    |                                     |        | /etc/rc4.d/S20supervisor                        |
    |                                     |        | /etc/init.d/supervisor                          |
    |                                     |        | /etc/rc5.d/S20supervisor                        |
    | time.prisem.washington.edu          | GOOD   |                                                 |
    | jenkins-int.prisem.washington.edu   | GOOD   | /etc/rc0.d/K20supervisor                        |
    |                                     |        | /etc/rc3.d/S20supervisor                        |
    |                                     |        | /etc/rc1.d/K20supervisor                        |
    |                                     |        | /etc/default/supervisor                         |
    |                                     |        | /etc/rc2.d/S20supervisor                        |
    |                                     |        | /etc/rc6.d/K20supervisor                        |
    |                                     |        | /etc/supervisor                                 |
    |                                     |        | /etc/supervisor/supervisord.conf                |
    |                                     |        | /etc/rc4.d/S20supervisor                        |
    |                                     |        | /etc/init.d/supervisor                          |
    |                                     |        | /etc/rc5.d/S20supervisor                        |
    | u12-dev-ws-1.prisem.washington.edu  | GOOD   |                                                 |
    | sso.prisem.washington.edu           | GOOD   |                                                 |
    | lapp-int.prisem.washington.edu      | GOOD   |                                                 |
    | foswiki-int.prisem.washington.edu   | GOOD   |                                                 |
    | u12-dev-svr-1.prisem.washington.edu | GOOD   | /etc/rc2.d/S20supervisor                        |
    |                                     |        | /etc/rc4.d/S20supervisor                        |
    |                                     |        | /etc/init.d/supervisor                          |
    |                                     |        | /etc/rc5.d/S20supervisor                        |
    |                                     |        | /etc/rc3.d/S20supervisor                        |
    |                                     |        | /etc/supervisor                                 |
    |                                     |        | /etc/supervisor/supervisord.conf                |
    |                                     |        | /etc/rc6.d/K20supervisor                        |
    |                                     |        | /etc/rc1.d/K20supervisor                        |
    |                                     |        | /etc/rc0.d/K20supervisor                        |
    | hub.prisem.washington.edu           | GOOD   |                                                 |
    | floyd2-p.prisem.washington.edu      | GOOD   |                                                 |
    | jira-int.prisem.washington.edu      | GOOD   | /etc/rc0.d/K20supervisor                        |
    |                                     |        | /etc/rc3.d/S20supervisor                        |
    |                                     |        | /etc/rc1.d/K20supervisor                        |
    |                                     |        | /etc/default/supervisor                         |
    |                                     |        | /etc/rc2.d/S20supervisor                        |
    |                                     |        | /etc/rc6.d/K20supervisor                        |
    |                                     |        | /etc/supervisor                                 |
    |                                     |        | /etc/supervisor/supervisord.conf                |
    |                                     |        | /etc/rc4.d/S20supervisor                        |
    |                                     |        | /etc/init.d/supervisor                          |
    |                                     |        | /etc/rc5.d/S20supervisor                        |
    | lapp.prisem.washington.edu          | GOOD   |                                                 |
    +-------------------------------------+--------+-------------------------------------------------+

..

While the concept of putting a list of host names into a file with a label is simple
to understand, it is not very flexible or scalable. Ansible supports a concept
called a `Dynamic Inventory`_. Rather than passing a hosts file using ``-i`` or
``--inventory``, you can pass a Python script that produces a special JSON object.

.. _Dynamic Inventory: http://docs.ansible.com/ansible/intro_dynamic_inventory.html
.. _Developing Plugins: http://docs.ansible.com/ansible/developing_plugins.html

What is not very widely known is that you can also trigger creation of a
dynamic inventory within ``ansible`` or ``ansible-playbook`` by passing
a *list* for the ``-i`` or ``--inventory`` option. Rather than creating
a temporary file with ``[all]`` at the top, followed by a list of
three host names, then passing that file with ``-i`` or ``--inventory``, just
pass a comma-separated list instead:

.. code-block:: none

    [dimsenv] dittrich@dimsdemo1:~/dims/git/python-dimscli (develop*) $ dimscli ansible shell --program "find /etc -name supervisord.conf" --inventory rabbitmq.prisem.washington.edu,time.prisem.washi
    ngton.edu,u12-dev-svr-1.prisem.washington.edu --remote-port 8422 --remote-user dittrich
    +-------------------------------------+--------+----------------------------------+
    | Host                                | Status | Results                          |
    +-------------------------------------+--------+----------------------------------+
    | rabbitmq.prisem.washington.edu      | GOOD   | /etc/supervisor/supervisord.conf |
    | time.prisem.washington.edu          | GOOD   |                                  |
    | u12-dev-svr-1.prisem.washington.edu | GOOD   | /etc/supervisor/supervisord.conf |
    +-------------------------------------+--------+----------------------------------+

..

There is a subtle trick for passing just a single host, and that is to pass
the name with a trailing comma (``,``), as seen here:

.. code-block:: none

    [dimsenv] dittrich@dimsdemo1:~/dims/git/python-dimscli (develop*) $ dimscli ansible shell --program "find /etc -name supervisord.conf" --inventory rabbitmq.prisem.washington.edu, --remote-port 84
    22 --remote-user dittrich
    +--------------------------------+--------+----------------------------------+
    | Host                           | Status | Results                          |
    +--------------------------------+--------+----------------------------------+
    | rabbitmq.prisem.washington.edu | GOOD   | /etc/supervisor/supervisord.conf |
    +--------------------------------+--------+----------------------------------+

..


.. code-block:: none

   ...

..

.. _otherdiagnosingtools:

Other Tools for Diagnosing System Problems
------------------------------------------

.. _smartmontools:

smartmontools
^^^^^^^^^^^^^

Hardware makes up the physical layer of the DIMS system. Developers are
currently using Dell Precision M4800 laptops to develop the software
layers of DIMS.

These laptops have had multiple issues, specifically including not sleeping
properly and heating up to extreme temperatures, heating up to extreme
temperatures when not sitting on solid, very well ventilated surfaces, and
these specific problems have led to malfunctions with the hard drives. At
least one laptop has completely stopped being able to boot.  Multiple other
laptops have struggled during the boot up process and have had other problems
that may indicate a near-term hard drive failure. 

In an effort to turn a black box into less of a black box and to try to see
ahead of time if there are any indicators that may be pointing to a failure
before a failure, we are now employing the use of a tool called ``smartmontools``.
This package comes with two tools -- ``smartctl`` and ``smartd`` -- which
control and monitor storage systems using the ``Self-Monitoring, Analysis
and Reporting Technology System (SMART)`` built in to a lot of modern hard drives,
including the ones on the developer laptops. When using this tool as a daemon,
it can give advanced warning of disk degradation and failure. (For more
information, see `smartmontools home`_.

The package will be added to the list of base packages installed on all
DIMS systems, and the rest of this section will be devoted to a brief
introduction for how to use the tool. 

.. note::

    These instructions were taken from `ubuntu smartmontools docs`_.
    If it differs on other Linux flavors (particularly Debian Jessie), new
    instructions will be added.

..

You will be using the ``smartctl`` utility to manually monitor your drives.
First, you need to double check that your hard drive is SMART-enabled.

.. code-block:: none
    :linenos:
    :emphasize-lines: 18,19 

    [dimsenv] mboggess@dimsdev2:it/dims-adminguide/docs/source (develop*) $ sudo smartctl -i /dev/sda
    smartctl 6.2 2013-07-26 r3841 [x86_64-linux-4.4.0-42-generic] (local build)
    Copyright (C) 2002-13, Bruce Allen, Christian Franke, www.smartmontools.org
    
    === START OF INFORMATION SECTION ===
    Model Family:     Seagate Laptop SSHD
    Device Model:     ST1000LM014-1EJ164
    Serial Number:    W771CY1P
    LU WWN Device Id: 5 000c50 089fc94f9
    Firmware Version: DEMB
    User Capacity:    1,000,204,886,016 bytes [1.00 TB]
    Sector Sizes:     512 bytes logical, 4096 bytes physical
    Rotation Rate:    5400 rpm
    Device is:        In smartctl database [for details use: -P show]
    ATA Version is:   ACS-2, ACS-3 T13/2161-D revision 3b
    SATA Version is:  SATA 3.1, 6.0 Gb/s (current: 6.0 Gb/s)
    Local Time is:    Fri Oct 14 11:08:25 2016 EDT
    SMART support is: Available - device has SMART capability.
    SMART support is: Enabled

..

This output gives you information about the hard drive, including if SMART
is support and enabled.

In the event that somehow SMART is available but not enabled, run

.. code-block:: none

   sudo smartctl -s on /dev/sda 

..

There are several different types of tests you can run via ``smartctl``. A full
list is documented in the help/usage output which you can obtain by running

.. code-block:: none

    [dimsenv] mboggess@dimsdev2:it/dims-adminguide/docs/source (develop*) $ smartctl -h

..

To find an estimate of the time it will take to complete the various tests, run

.. code-block:: none
    :linenos:
    :emphasize-lines: 13,14,29-34

    [dimsenv] mboggess@dimsdev2:it/dims-adminguide/docs/source (develop*) $ sudo smartctl -c /dev/sda
    smartctl 6.2 2013-07-26 r3841 [x86_64-linux-4.4.0-42-generic] (local build)
    Copyright (C) 2002-13, Bruce Allen, Christian Franke, www.smartmontools.org
    
    === START OF READ SMART DATA SECTION ===
    General SMART Values:
    Offline data collection status:  (0x00) Offline data collection activity
                                            was never started.
                                            Auto Offline Data Collection: Disabled.
    Self-test execution status:      (   0) The previous self-test routine completed
                                            without error or no self-test has ever 
                                            been run.
    Total time to complete Offline 
    data collection:                (  139) seconds.
    Offline data collection
    capabilities:                    (0x73) SMART execute Offline immediate.
                                            Auto Offline data collection on/off support.
                                            Suspend Offline collection upon new
                                            command.
                                            No Offline surface scan supported.
                                            Self-test supported.
                                            Conveyance Self-test supported.
                                            Selective Self-test supported.
    SMART capabilities:            (0x0003) Saves SMART data before entering
                                            power-saving mode.
                                            Supports SMART auto save timer.
    Error logging capability:        (0x01) Error logging supported.
                                            General Purpose Logging supported.
    Short self-test routine 
    recommended polling time:        (   2) minutes.
    Extended self-test routine
    recommended polling time:        ( 191) minutes.
    Conveyance self-test routine
    recommended polling time:        (   3) minutes.
    SCT capabilities:              (0x10b5) SCT Status supported.
                                            SCT Feature Control supported.
                                            SCT Data Table supported.

..

As you can see, the ``long`` test is rather long--191 minutes!

To run the ``long`` test, run

.. code-block:: none

    [dimsenv] mboggess@dimsdev2:it/dims-adminguide/docs/source (develop*) $ sudo smartctl -t long /dev/sda
    smartctl 6.2 2013-07-26 r3841 [x86_64-linux-4.4.0-42-generic] (local build)
    Copyright (C) 2002-13, Bruce Allen, Christian Franke, www.smartmontools.org
    
    === START OF OFFLINE IMMEDIATE AND SELF-TEST SECTION ===
    Sending command: "Execute SMART Extended self-test routine immediately in off-line mode".
    Drive command "Execute SMART Extended self-test routine immediately in off-line mode" successful.
    Testing has begun.
    Please wait 191 minutes for test to complete.
    Test will complete after Fri Oct 14 15:00:32 2016
    
    Use smartctl -X to abort test.

..

To abort the test:

.. code-block:: none

    [dimsenv] mboggess@dimsdev2:it/dims-adminguide/docs/source (develop*) $ sudo smartctl -X /dev/sda
    smartctl 6.2 2013-07-26 r3841 [x86_64-linux-4.4.0-42-generic] (local build)
    Copyright (C) 2002-13, Bruce Allen, Christian Franke, www.smartmontools.org
    
    === START OF OFFLINE IMMEDIATE AND SELF-TEST SECTION ===
    Sending command: "Abort SMART off-line mode self-test routine".
    Self-testing aborted!

..

To get test results, for a SATA drive, run

.. code-block:: none

    [dimsenv] mboggess@dimsdev2:it/dims-adminguide/docs/source (develop*) $ sudo smartctl -a -d ata /dev/sda

..

To get test results, for an IDE drive, run

.. code-block:: none

    [dimsenv] mboggess@dimsdev2:it/dims-adminguide/docs/source (develop*) $ sudo smartctl -a /dev/sda

..

Additionally, you can run ``smartmontools`` as a daemon, but for now, that
will be left for an admin to research and develop on their own. In the future,
this has potential to be turned into an Ansible role. Documentation from Ubuntu
on how to use ``smartmontools`` as a daemon can be found in the `daemon subsection`_
of the Ubuntu ``smartmontools`` documentation.

.. _smartmontools home: https://www.smartmontools.org/
.. _ubuntu smartmontools docs: https://help.ubuntu.com/community/Smartmontools
.. _daemon subsection: https://help.ubuntu.com/community/Smartmontools#Advanced:_Running_as_Smartmontools_as_a_Daemon

