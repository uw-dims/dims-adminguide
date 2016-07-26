.. _manageservices:

Managing Long-running Services
==============================

This chapter covers the process of keeping a service program alive
across system reboots, using ``supervisord`` or **Upstart**.
Regardless of which of these mechanisms is used, the concept is similar:

* A program that provides a network service is supposed to be started
  when the system starts, and stopped when the system is brought down.
  This should be done cleanly, so that any required state is maintained
  across reboots.

* If the program exits for any reason, this reason should be checked and
  acted upon such that the desired goal of having the service be available
  when you want it to be available is maintained. This means that when the
  service program exists with an unexpected return code, it is restarted.

  .. note::

     If the program is supposed to be turned off, and it exits with an
     expected "normal" exit code, it is left off until it is explicitly
     started again.

  ..

The ``supervisord`` program is much simpler than Upstart, but in some cases
is sufficient to get the job done with a minimum of effort, and is much
easier to debug. Upstart, on the other hand, is very complex and feature-rich,
lending to more sophisticated capabilities (e.g., monitoring multiple hierarchical
dependent services to control starting and stopping service daemons in
complex inter-dependent situations). This flexibility comes at the cost of
much more difficulty in designing, developing, and most importantly
*debugging* these services and requires significantly greater system
administration and programming experience to accomplish.  The section
on Upstart includes some techniques for debugging services.

.. note::

    Section :ref:`rpidocker` covers this topic in the specific context of a
    prototype Docker containerized service using the **HypriotOS** on a
    RaspberryPI. This section covers the same material in the context of the
    primary operating system used by the DIMS project, **Ubuntu**.

..

.. _supervisordServices:

Services using ``supervisord``
------------------------------

.. _upstartServices:

Services using Upstart
----------------------

By default, Upstart does not log very much. To see the logging level currently
set, do:

.. code-block:: none

    $ sudo initctl log-priority
    message

..

To increase the logging level, do:

.. code-block:: none

    $ sudo initctl log-priority info
    $ sudo initctl log-priority
    info

..

Now you can follow the system logs using ``sudo tail -f /var/log/syslog`` and
watch events.  In this case, we want to see all of the ``init`` events
associated with restarting the OpenVPN tunnel (which is the pathway used by the
Consul agents for communicating.)

To know which events are associated with the action we are about to cause, use
the ``logger`` program to insert markers immediately before the ``restart``
is triggered. Then wait until it looks like the service is completely restarted
before inserting another marker and then copying the log output.

.. attention::

   Because service are stopped and started asynchronously in the background,
   the only marker that is easy to accurately set is the one immediately before
   the ``restart`` is triggered.  If another ``&&`` was added to insert a
   marker **immediately** after the ``sudo service openvpn restart`` command
   returned and the shell allowed the ``logger`` command to run, it would
   insert the marker in the middle of the actions going on in the background.
   
   Be careful to keep this asynchrony in your mind and separate the act of the
   shell returning from the unrelated act of the service being restarted, or
   else you will not get the results you expect.

   Additionally, on a busy system there may also be other events that show up
   in the log file between the ``logger`` command and the initiation of the
   ``restart`` action (and interspersed with the logs that are important for
   our purposes. You will need to carefully delete those log entries that
   are not important in order to minimize the "noise" of all the state
   transition messages from ``init``.

..

* http://askubuntu.com/questions/28281/what-events-are-available-for-upstart

.. code-block:: none

    $ logger -t DITTRICH -p local0.info "Restarting OpenVPN" && sudo service openvpn restart
    * Stopping virtual private network daemon(s)...
    *   Stopping VPN '01_prsm_dimsdemo1'
      ...done.
    *   Stopping VPN '02_uwapl_dimsdemo1'
      ...done.
    * Starting virtual private network daemon(s)...
    *   Autostarting VPN '01_prsm_dimsdemo1'
    *   Autostarting VPN '02_uwapl_dimsdemo1'
    $ logger -t DITTRICH -p local0.info "Done"

..

.. code-block:: none

    Jun  4 20:07:16 dimsdemo1.node.consul DITTRICH: Restarting OpenVPN
    Jun  4 20:07:16 dimsdemo1.node.consul ovpn-01_prsm_dimsdemo1[14113]: event_wait : Interrupted system call (code=4)
    Jun  4 20:07:16 dimsdemo1.node.consul ovpn-01_prsm_dimsdemo1[14113]: /sbin/ip route del 10.142.29.0/24
    Jun  4 20:07:16 dimsdemo1.node.consul ovpn-01_prsm_dimsdemo1[14113]: ERROR: Linux route delete command failed: external program exited with error status: 2
    Jun  4 20:07:16 dimsdemo1.node.consul ovpn-01_prsm_dimsdemo1[14113]: Closing TUN/TAP interface
    Jun  4 20:07:16 dimsdemo1.node.consul ovpn-01_prsm_dimsdemo1[14113]: /sbin/ip addr del dev tun0 10.86.86.4/24
    Jun  4 20:07:16 dimsdemo1.node.consul ovpn-01_prsm_dimsdemo1[14113]: Linux ip addr del failed: external program exited with error status: 2
    Jun  4 20:07:16 dimsdemo1.node.consul NetworkManager[1055]:    SCPlugin-Ifupdown: devices removed (path: /sys/devices/virtual/net/tun0, iface: tun0)
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.461020] init: Handling queues-device-removed event
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.461202] init: Handling queues-device-removed event
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.461321] init: Handling net-device-removed event
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.461372] init: network-interface (tun0) goal changed from start to stop
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.461400] init: network-interface (tun0) state changed from running to stopping
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.461449] init: Handling stopping event
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.461482] init: network-interface (tun0) state changed from stopping to killed
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.461517] init: network-interface (tun0) state changed from killed to post-stop
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.462204] init: network-interface (tun0) post-stop process (26911)
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.463454] init: network-interface (tun0) post-stop process (26911) exited normally
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.463512] init: network-interface (tun0) state changed from post-stop to waiting
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.463686] init: Handling stopped event
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.463772] init: startpar-bridge (network-interface-tun0-stopped) goal changed from stop to start
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.463807] init: startpar-bridge (network-interface-tun0-stopped) state changed from waiting to starting
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.463929] init: network-interface-security (network-interface/tun0) goal changed from start to stop
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.463956] init: network-interface-security (network-interface/tun0) state changed from running to stopping
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.464026] init: Handling starting event
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.464080] init: startpar-bridge (network-interface-tun0-stopped) state changed from starting to security
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.464113] init: startpar-bridge (network-interface-tun0-stopped) state changed from security to pre-start
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.464146] init: startpar-bridge (network-interface-tun0-stopped) state changed from pre-start to spawned
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.464639] init: startpar-bridge (network-interface-tun0-stopped) main process (26914)
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.464660] init: startpar-bridge (network-interface-tun0-stopped) state changed from spawned to post-start
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.464705] init: startpar-bridge (network-interface-tun0-stopped) state changed from post-start to running
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.464784] init: Handling stopping event
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.464903] init: network-interface-security (network-interface/tun0) state changed from stopping to killed
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.464936] init: network-interface-security (network-interface/tun0) state changed from killed to post-stop
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.464967] init: network-interface-security (network-interface/tun0) state changed from post-stop to waiting
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.465100] init: Handling started event
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.465180] init: Handling stopped event
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.465236] init: startpar-bridge (network-interface-security-network-interface/tun0-stopped) goal changed from stop to start
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.465267] init: startpar-bridge (network-interface-security-network-interface/tun0-stopped) state changed from waiting to starting
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.465339] init: Handling starting event
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.465379] init: startpar-bridge (network-interface-security-network-interface/tun0-stopped) state changed from starting to security
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.465410] init: startpar-bridge (network-interface-security-network-interface/tun0-stopped) state changed from security to pre-start
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.465438] init: startpar-bridge (network-interface-security-network-interface/tun0-stopped) state changed from pre-start to spawned
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.466165] init: startpar-bridge (network-interface-security-network-interface/tun0-stopped) main process (26915)
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.466190] init: startpar-bridge (network-interface-security-network-interface/tun0-stopped) state changed from spawned to post-start
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.466244] init: startpar-bridge (network-interface-security-network-interface/tun0-stopped) state changed from post-start to running
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.466331] init: Handling started event
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.466610] init: startpar-bridge (network-interface-tun0-stopped) main process (26914) exited normally
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.466667] init: startpar-bridge (network-interface-tun0-stopped) goal changed from start to stop
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.466729] init: startpar-bridge (network-interface-tun0-stopped) state changed from running to stopping
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.466796] init: startpar-bridge (network-interface-security-network-interface/tun0-stopped) main process (26915) exited normally
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.466848] init: startpar-bridge (network-interface-security-network-interface/tun0-stopped) goal changed from start to stop
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.466883] init: startpar-bridge (network-interface-security-network-interface/tun0-stopped) state changed from running to stopping
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.466921] init: Handling stopping event
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.466959] init: startpar-bridge (network-interface-tun0-stopped) state changed from stopping to killed
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.466990] init: startpar-bridge (network-interface-tun0-stopped) state changed from killed to post-stop
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.467020] init: startpar-bridge (network-interface-tun0-stopped) state changed from post-stop to waiting
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.467134] init: Handling stopping event
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.467169] init: startpar-bridge (network-interface-security-network-interface/tun0-stopped) state changed from stopping to killed
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.467199] init: startpar-bridge (network-interface-security-network-interface/tun0-stopped) state changed from killed to post-stop
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.467248] init: startpar-bridge (network-interface-security-network-interface/tun0-stopped) state changed from post-stop to waiting
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.467398] init: Handling stopped event
    Jun  4 20:07:16 dimsdemo1.node.consul kernel: [58061.467490] init: Handling stopped event
    Jun  4 20:07:16 dimsdemo1.node.consul ovpn-01_prsm_dimsdemo1[14113]: SIGTERM[hard,] received, process exiting
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[14127]: event_wait : Interrupted system call (code=4)
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[14127]: /sbin/ip route del 38.111.193.0/24
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[14127]: ERROR: Linux route delete command failed: external program exited with error status: 2
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[14127]: /sbin/ip route del 199.168.91.0/24
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[14127]: ERROR: Linux route delete command failed: external program exited with error status: 2
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[14127]: /sbin/ip route del 192.168.88.0/24
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[14127]: ERROR: Linux route delete command failed: external program exited with error status: 2
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[14127]: Closing TUN/TAP interface
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[14127]: /sbin/ip addr del dev tun88 10.88.88.5/24
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[14127]: Linux ip addr del failed: external program exited with error status: 2
    Jun  4 20:07:17 dimsdemo1.node.consul NetworkManager[1055]:    SCPlugin-Ifupdown: devices removed (path: /sys/devices/virtual/net/tun88, iface: tun88)
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.504410] init: Handling queues-device-removed event
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.504612] init: Handling queues-device-removed event
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.504723] init: Handling net-device-removed event
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.504763] init: network-interface (tun88) goal changed from start to stop
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.504799] init: network-interface (tun88) state changed from running to stopping
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.504844] init: Handling stopping event
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.504877] init: network-interface (tun88) state changed from stopping to killed
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.504907] init: network-interface (tun88) state changed from killed to post-stop
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.505652] init: network-interface (tun88) post-stop process (26927)
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.506919] init: network-interface (tun88) post-stop process (26927) exited normally
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.506976] init: network-interface (tun88) state changed from post-stop to waiting
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.507159] init: Handling stopped event
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.507234] init: startpar-bridge (network-interface-tun88-stopped) goal changed from stop to start
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.507263] init: startpar-bridge (network-interface-tun88-stopped) state changed from waiting to starting
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.507431] init: network-interface-security (network-interface/tun88) goal changed from start to stop
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.507470] init: network-interface-security (network-interface/tun88) state changed from running to stopping
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.507511] init: Handling starting event
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.507554] init: startpar-bridge (network-interface-tun88-stopped) state changed from starting to security
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.507575] init: startpar-bridge (network-interface-tun88-stopped) state changed from security to pre-start
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.507594] init: startpar-bridge (network-interface-tun88-stopped) state changed from pre-start to spawned
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.508094] init: startpar-bridge (network-interface-tun88-stopped) main process (26930)
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.508133] init: startpar-bridge (network-interface-tun88-stopped) state changed from spawned to post-start
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.508181] init: startpar-bridge (network-interface-tun88-stopped) state changed from post-start to running
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.508275] init: Handling stopping event
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.508410] init: network-interface-security (network-interface/tun88) state changed from stopping to killed
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.508441] init: network-interface-security (network-interface/tun88) state changed from killed to post-stop
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.508473] init: network-interface-security (network-interface/tun88) state changed from post-stop to waiting
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.508609] init: Handling started event
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.508713] init: Handling stopped event
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.508803] init: startpar-bridge (network-interface-security-network-interface/tun88-stopped) goal changed from stop to start
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.508863] init: startpar-bridge (network-interface-security-network-interface/tun88-stopped) state changed from waiting to starting
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.508967] init: Handling starting event
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.509008] init: startpar-bridge (network-interface-security-network-interface/tun88-stopped) state changed from starting to security
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.509060] init: startpar-bridge (network-interface-security-network-interface/tun88-stopped) state changed from security to pre-start
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.509109] init: startpar-bridge (network-interface-security-network-interface/tun88-stopped) state changed from pre-start to spawned
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.509733] init: startpar-bridge (network-interface-security-network-interface/tun88-stopped) main process (26931)
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.509753] init: startpar-bridge (network-interface-security-network-interface/tun88-stopped) state changed from spawned to post-start
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.509804] init: startpar-bridge (network-interface-security-network-interface/tun88-stopped) state changed from post-start to running
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.509897] init: Handling started event
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.510246] init: startpar-bridge (network-interface-tun88-stopped) main process (26930) exited normally
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.510303] init: startpar-bridge (network-interface-tun88-stopped) goal changed from start to stop
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.510366] init: startpar-bridge (network-interface-tun88-stopped) state changed from running to stopping
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.510433] init: startpar-bridge (network-interface-security-network-interface/tun88-stopped) main process (26931) exited normally
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.510501] init: startpar-bridge (network-interface-security-network-interface/tun88-stopped) goal changed from start to stop
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.510535] init: startpar-bridge (network-interface-security-network-interface/tun88-stopped) state changed from running to stopping
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.510573] init: Handling stopping event
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.510610] init: startpar-bridge (network-interface-tun88-stopped) state changed from stopping to killed
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.510642] init: startpar-bridge (network-interface-tun88-stopped) state changed from killed to post-stop
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.510672] init: startpar-bridge (network-interface-tun88-stopped) state changed from post-stop to waiting
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.510785] init: Handling stopping event
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.510819] init: startpar-bridge (network-interface-security-network-interface/tun88-stopped) state changed from stopping to killed
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.510849] init: startpar-bridge (network-interface-security-network-interface/tun88-stopped) state changed from killed to post-stop
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.510879] init: startpar-bridge (network-interface-security-network-interface/tun88-stopped) state changed from post-stop to waiting
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.511028] init: Handling stopped event
    Jun  4 20:07:17 dimsdemo1.node.consul kernel: [58061.511120] init: Handling stopped event
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[14127]: SIGTERM[hard,] received, process exiting
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-01_prsm_dimsdemo1[26949]: OpenVPN 2.3.2 x86_64-pc-linux-gnu [SSL (OpenSSL)] [LZO] [EPOLL] [PKCS11] [eurephia] [MH] [IPv6] built on Dec  1 2014
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-01_prsm_dimsdemo1[26949]: Control Channel Authentication: tls-auth using INLINE static key file
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-01_prsm_dimsdemo1[26949]: Outgoing Control Channel Authentication: Using 160 bit message hash 'SHA1' for HMAC authentication
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-01_prsm_dimsdemo1[26949]: Incoming Control Channel Authentication: Using 160 bit message hash 'SHA1' for HMAC authentication
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-01_prsm_dimsdemo1[26949]: Socket Buffers: R=[212992->131072] S=[212992->131072]
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-01_prsm_dimsdemo1[26950]: NOTE: UID/GID downgrade will be delayed because of --client, --pull, or --up-delay
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-01_prsm_dimsdemo1[26950]: UDPv4 link local: [undef]
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-01_prsm_dimsdemo1[26950]: UDPv4 link remote: [AF_INET]140.142.29.115:500
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[26963]: OpenVPN 2.3.2 x86_64-pc-linux-gnu [SSL (OpenSSL)] [LZO] [EPOLL] [PKCS11] [eurephia] [MH] [IPv6] built on Dec  1 2014
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[26963]: Control Channel Authentication: tls-auth using INLINE static key file
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[26963]: Outgoing Control Channel Authentication: Using 160 bit message hash 'SHA1' for HMAC authentication
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[26963]: Incoming Control Channel Authentication: Using 160 bit message hash 'SHA1' for HMAC authentication
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[26963]: Socket Buffers: R=[212992->131072] S=[212992->131072]
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[26964]: NOTE: UID/GID downgrade will be delayed because of --client, --pull, or --up-delay
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[26964]: UDPv4 link local: [undef]
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[26964]: UDPv4 link remote: [AF_INET]140.142.29.118:8989
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[26964]: TLS: Initial packet from [AF_INET]140.142.29.118:8989, sid=adf2b40a afa33d74
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-01_prsm_dimsdemo1[26950]: TLS: Initial packet from [AF_INET]140.142.29.115:500, sid=3cf9074f 2e93fa51
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-01_prsm_dimsdemo1[26950]: Data Channel Encrypt: Cipher 'AES-128-CBC' initialized with 128 bit key
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-01_prsm_dimsdemo1[26950]: Data Channel Encrypt: Using 160 bit message hash 'SHA1' for HMAC authentication
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-01_prsm_dimsdemo1[26950]: Data Channel Decrypt: Cipher 'AES-128-CBC' initialized with 128 bit key
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-01_prsm_dimsdemo1[26950]: Data Channel Decrypt: Using 160 bit message hash 'SHA1' for HMAC authentication
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-01_prsm_dimsdemo1[26950]: Control Channel: TLSv1, cipher TLSv1/SSLv3 DHE-RSA-AES256-SHA, 2048 bit RSA
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-01_prsm_dimsdemo1[26950]: [eclipse-prisem] Peer Connection Initiated with [AF_INET]140.142.29.115:500
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[26964]: Data Channel Encrypt: Cipher 'AES-128-CBC' initialized with 128 bit key
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[26964]: Data Channel Encrypt: Using 160 bit message hash 'SHA1' for HMAC authentication
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[26964]: Data Channel Decrypt: Cipher 'AES-128-CBC' initialized with 128 bit key
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[26964]: Data Channel Decrypt: Using 160 bit message hash 'SHA1' for HMAC authentication
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[26964]: Control Channel: TLSv1, cipher TLSv1/SSLv3 DHE-RSA-AES256-SHA, 2048 bit RSA
    Jun  4 20:07:17 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[26964]: [server] Peer Connection Initiated with [AF_INET]140.142.29.118:8989
    Jun  4 20:07:19 dimsdemo1.node.consul ovpn-01_prsm_dimsdemo1[26950]: SENT CONTROL [eclipse-prisem]: 'PUSH_REQUEST' (status=1)
    Jun  4 20:07:19 dimsdemo1.node.consul ovpn-01_prsm_dimsdemo1[26950]: PUSH: Received control message: ...
    Jun  4 20:07:19 dimsdemo1.node.consul ovpn-01_prsm_dimsdemo1[26950]: OPTIONS IMPORT: timers and/or timeouts modified
    Jun  4 20:07:19 dimsdemo1.node.consul ovpn-01_prsm_dimsdemo1[26950]: OPTIONS IMPORT: LZO parms modified
    Jun  4 20:07:19 dimsdemo1.node.consul ovpn-01_prsm_dimsdemo1[26950]: OPTIONS IMPORT: --ifconfig/up options modified
    Jun  4 20:07:19 dimsdemo1.node.consul ovpn-01_prsm_dimsdemo1[26950]: OPTIONS IMPORT: route options modified
    Jun  4 20:07:19 dimsdemo1.node.consul ovpn-01_prsm_dimsdemo1[26950]: OPTIONS IMPORT: route-related options modified
    Jun  4 20:07:19 dimsdemo1.node.consul ovpn-01_prsm_dimsdemo1[26950]: OPTIONS IMPORT: --ip-win32 and/or --dhcp-option options modified
    Jun  4 20:07:19 dimsdemo1.node.consul ovpn-01_prsm_dimsdemo1[26950]: ROUTE_GATEWAY 192.168.0.1/255.255.255.0 IFACE=wlan0 HWADDR=d0:53:49:d7:9e:bd
    Jun  4 20:07:19 dimsdemo1.node.consul ovpn-01_prsm_dimsdemo1[26950]: TUN/TAP device tun0 opened
    Jun  4 20:07:19 dimsdemo1.node.consul ovpn-01_prsm_dimsdemo1[26950]: TUN/TAP TX queue length set to 100
    Jun  4 20:07:19 dimsdemo1.node.consul ovpn-01_prsm_dimsdemo1[26950]: do_ifconfig, tt->ipv6=0, tt->did_ifconfig_ipv6_setup=0
    Jun  4 20:07:19 dimsdemo1.node.consul ovpn-01_prsm_dimsdemo1[26950]: /sbin/ip link set dev tun0 up mtu 1500
    Jun  4 20:07:19 dimsdemo1.node.consul NetworkManager[1055]:    SCPlugin-Ifupdown: devices added (path: /sys/devices/virtual/net/tun0, iface: tun0)
    Jun  4 20:07:19 dimsdemo1.node.consul NetworkManager[1055]:    SCPlugin-Ifupdown: device added (path: /sys/devices/virtual/net/tun0, iface: tun0): no ifupdown configuration found.
    Jun  4 20:07:19 dimsdemo1.node.consul NetworkManager[1055]: <warn> /sys/devices/virtual/net/tun0: couldn't determine device driver; ignoring...
    Jun  4 20:07:19 dimsdemo1.node.consul ovpn-01_prsm_dimsdemo1[26950]: /sbin/ip addr add dev tun0 10.86.86.4/24 broadcast 10.86.86.255
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.897552] init: Handling net-device-added event
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.897768] init: network-interface (tun0) goal changed from stop to start
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.897831] init: network-interface (tun0) state changed from waiting to starting
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.897933] init: Handling starting event
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.898119] init: network-interface-security (network-interface/tun0) goal changed from stop to start
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.898175] init: network-interface-security (network-interface/tun0) state changed from waiting to starting
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.898246] init: Handling starting event
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.898319] init: network-interface-security (network-interface/tun0) state changed from starting to security
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.898373] init: network-interface-security (network-interface/tun0) state changed from security to pre-start
    Jun  4 20:07:19 dimsdemo1.node.consul ovpn-01_prsm_dimsdemo1[26950]: /sbin/ip route add 10.142.29.0/24 via 10.86.86.1
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.899415] init: network-interface-security (network-interface/tun0) pre-start process (27032)
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.899754] init: Handling queues-device-added event
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.900062] init: Handling queues-device-added event
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.900301] init: network-interface-security (network-interface/tun0) pre-start process (27032) exited normally
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.900403] init: network-interface-security (network-interface/tun0) state changed from pre-start to spawned
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.900465] init: network-interface-security (network-interface/tun0) state changed from spawned to post-start
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.900527] init: network-interface-security (network-interface/tun0) state changed from post-start to running
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.900591] init: network-interface (tun0) state changed from starting to security
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.900641] init: network-interface (tun0) state changed from security to pre-start
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.901534] init: network-interface (tun0) pre-start process (27033)
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.901884] init: Handling started event
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.902189] init: startpar-bridge (network-interface-security-network-interface/tun0-started) goal changed from stop to start
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.902361] init: startpar-bridge (network-interface-security-network-interface/tun0-started) state changed from waiting to starting
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.902728] init: Handling starting event
    Jun  4 20:07:19 dimsdemo1.node.consul ovpn-01_prsm_dimsdemo1[26950]: GID set to nogroup
    Jun  4 20:07:19 dimsdemo1.node.consul ovpn-01_prsm_dimsdemo1[26950]: UID set to nobody
    Jun  4 20:07:19 dimsdemo1.node.consul ovpn-01_prsm_dimsdemo1[26950]: Initialization Sequence Completed
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.902874] init: startpar-bridge (network-interface-security-network-interface/tun0-started) state changed from starting to security
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.903036] init: startpar-bridge (network-interface-security-network-interface/tun0-started) state changed from security to pre-start
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.903191] init: startpar-bridge (network-interface-security-network-interface/tun0-started) state changed from pre-start to spawned
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.904568] init: startpar-bridge (network-interface-security-network-interface/tun0-started) main process (27035)
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.904606] init: startpar-bridge (network-interface-security-network-interface/tun0-started) state changed from spawned to post-start
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.904693] init: startpar-bridge (network-interface-security-network-interface/tun0-started) state changed from post-start to running
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.904841] init: Handling started event
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.905285] init: startpar-bridge (network-interface-security-network-interface/tun0-started) main process (27035) exited normally
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.905430] init: startpar-bridge (network-interface-security-network-interface/tun0-started) goal changed from start to stop
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.905509] init: startpar-bridge (network-interface-security-network-interface/tun0-started) state changed from running to stopping
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.905583] init: Handling stopping event
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.905688] init: startpar-bridge (network-interface-security-network-interface/tun0-started) state changed from stopping to killed
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.905752] init: startpar-bridge (network-interface-security-network-interface/tun0-started) state changed from killed to post-stop
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.905809] init: startpar-bridge (network-interface-security-network-interface/tun0-started) state changed from post-stop to waiting
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.906042] init: Handling stopped event
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.907410] init: network-interface (tun0) pre-start process (27033) exited normally
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.907464] init: network-interface (tun0) state changed from pre-start to spawned
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.907497] init: network-interface (tun0) state changed from spawned to post-start
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.907531] init: network-interface (tun0) state changed from post-start to running
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.907616] init: Handling started event
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.907693] init: startpar-bridge (network-interface-tun0-started) goal changed from stop to start
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.907727] init: startpar-bridge (network-interface-tun0-started) state changed from waiting to starting
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.907816] init: Handling starting event
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.907870] init: startpar-bridge (network-interface-tun0-started) state changed from starting to security
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.907897] init: startpar-bridge (network-interface-tun0-started) state changed from security to pre-start
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.907927] init: startpar-bridge (network-interface-tun0-started) state changed from pre-start to spawned
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.908460] init: startpar-bridge (network-interface-tun0-started) main process (27039)
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.908481] init: startpar-bridge (network-interface-tun0-started) state changed from spawned to post-start
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.908526] init: startpar-bridge (network-interface-tun0-started) state changed from post-start to running
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.908606] init: Handling started event
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.908945] init: startpar-bridge (network-interface-tun0-started) main process (27039) exited normally
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.909008] init: startpar-bridge (network-interface-tun0-started) goal changed from start to stop
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.909044] init: startpar-bridge (network-interface-tun0-started) state changed from running to stopping
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.909082] init: Handling stopping event
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.909120] init: startpar-bridge (network-interface-tun0-started) state changed from stopping to killed
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.909151] init: startpar-bridge (network-interface-tun0-started) state changed from killed to post-stop
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.909183] init: startpar-bridge (network-interface-tun0-started) state changed from post-stop to waiting
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58063.909293] init: Handling stopped event
    Jun  4 20:07:19 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[26964]: SENT CONTROL [server]: 'PUSH_REQUEST' (status=1)
    Jun  4 20:07:19 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[26964]: PUSH: Received control message: ...
    Jun  4 20:07:19 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[26964]: OPTIONS IMPORT: timers and/or timeouts modified
    Jun  4 20:07:19 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[26964]: OPTIONS IMPORT: --ifconfig/up options modified
    Jun  4 20:07:19 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[26964]: OPTIONS IMPORT: route options modified
    Jun  4 20:07:19 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[26964]: OPTIONS IMPORT: route-related options modified
    Jun  4 20:07:19 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[26964]: ROUTE_GATEWAY 192.168.0.1/255.255.255.0 IFACE=wlan0 HWADDR=d0:53:49:d7:9e:bd
    Jun  4 20:07:19 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[26964]: TUN/TAP device tun88 opened
    Jun  4 20:07:19 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[26964]: TUN/TAP TX queue length set to 100
    Jun  4 20:07:19 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[26964]: do_ifconfig, tt->ipv6=0, tt->did_ifconfig_ipv6_setup=0
    Jun  4 20:07:19 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[26964]: /sbin/ip link set dev tun88 up mtu 1500
    Jun  4 20:07:19 dimsdemo1.node.consul NetworkManager[1055]:    SCPlugin-Ifupdown: devices added (path: /sys/devices/virtual/net/tun88, iface: tun88)
    Jun  4 20:07:19 dimsdemo1.node.consul NetworkManager[1055]:    SCPlugin-Ifupdown: device added (path: /sys/devices/virtual/net/tun88, iface: tun88): no ifupdown configuration found.
    Jun  4 20:07:19 dimsdemo1.node.consul NetworkManager[1055]: <warn> /sys/devices/virtual/net/tun88: couldn't determine device driver; ignoring...
    Jun  4 20:07:19 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[26964]: /sbin/ip addr add dev tun88 10.88.88.2/24 broadcast 10.88.88.255
    Jun  4 20:07:19 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[26964]: /sbin/ip route add 192.168.88.0/24 via 10.88.88.1
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.341486] init: Handling net-device-added event
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.341622] init: network-interface (tun88) goal changed from stop to start
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.341655] init: network-interface (tun88) state changed from waiting to starting
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.341714] init: Handling starting event
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.341838] init: network-interface-security (network-interface/tun88) goal changed from stop to start
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.341869] init: network-interface-security (network-interface/tun88) state changed from waiting to starting
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.341905] init: Handling starting event
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.341945] init: network-interface-security (network-interface/tun88) state changed from starting to security
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.341976] init: network-interface-security (network-interface/tun88) state changed from security to pre-start
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.342560] init: network-interface-security (network-interface/tun88) pre-start process (27060)
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.342787] init: Handling queues-device-added event
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.342956] init: Handling queues-device-added event
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.343091] init: network-interface-security (network-interface/tun88) pre-start process (27060) exited normally
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.343149] init: network-interface-security (network-interface/tun88) state changed from pre-start to spawned
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.343187] init: network-interface-security (network-interface/tun88) state changed from spawned to post-start
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.343217] init: network-interface-security (network-interface/tun88) state changed from post-start to running
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.343275] init: network-interface (tun88) state changed from starting to security
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.343310] init: network-interface (tun88) state changed from security to pre-start
    Jun  4 20:07:19 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[26964]: /sbin/ip route add 199.168.91.0/24 via 10.88.88.1
    Jun  4 20:07:19 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[26964]: /sbin/ip route add 38.111.193.0/24 via 10.88.88.1
    Jun  4 20:07:19 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[26964]: GID set to nogroup
    Jun  4 20:07:19 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[26964]: UID set to nobody
    Jun  4 20:07:19 dimsdemo1.node.consul ovpn-02_uwapl_dimsdemo1[26964]: Initialization Sequence Completed
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.343904] init: network-interface (tun88) pre-start process (27062)
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.344021] init: Handling started event
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.344112] init: startpar-bridge (network-interface-security-network-interface/tun88-started) goal changed from stop to start
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.344155] init: startpar-bridge (network-interface-security-network-interface/tun88-started) state changed from waiting to starting
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.344310] init: Handling starting event
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.344352] init: startpar-bridge (network-interface-security-network-interface/tun88-started) state changed from starting to security
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.344387] init: startpar-bridge (network-interface-security-network-interface/tun88-started) state changed from security to pre-start
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.344418] init: startpar-bridge (network-interface-security-network-interface/tun88-started) state changed from pre-start to spawned
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.344889] init: startpar-bridge (network-interface-security-network-interface/tun88-started) main process (27064)
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.344908] init: startpar-bridge (network-interface-security-network-interface/tun88-started) state changed from spawned to post-start
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.344956] init: startpar-bridge (network-interface-security-network-interface/tun88-started) state changed from post-start to running
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.345036] init: Handling started event
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.345420] init: startpar-bridge (network-interface-security-network-interface/tun88-started) main process (27064) exited normally
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.345490] init: startpar-bridge (network-interface-security-network-interface/tun88-started) goal changed from start to stop
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.345534] init: startpar-bridge (network-interface-security-network-interface/tun88-started) state changed from running to stopping
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.345573] init: Handling stopping event
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.345641] init: startpar-bridge (network-interface-security-network-interface/tun88-started) state changed from stopping to killed
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.345680] init: startpar-bridge (network-interface-security-network-interface/tun88-started) state changed from killed to post-stop
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.345709] init: startpar-bridge (network-interface-security-network-interface/tun88-started) state changed from post-stop to waiting
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.345834] init: Handling stopped event
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.347178] init: network-interface (tun88) pre-start process (27062) exited normally
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.347251] init: network-interface (tun88) state changed from pre-start to spawned
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.347299] init: network-interface (tun88) state changed from spawned to post-start
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.347333] init: network-interface (tun88) state changed from post-start to running
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.347414] init: Handling started event
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.347488] init: startpar-bridge (network-interface-tun88-started) goal changed from stop to start
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.347525] init: startpar-bridge (network-interface-tun88-started) state changed from waiting to starting
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.347619] init: Handling starting event
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.347660] init: startpar-bridge (network-interface-tun88-started) state changed from starting to security
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.347691] init: startpar-bridge (network-interface-tun88-started) state changed from security to pre-start
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.347719] init: startpar-bridge (network-interface-tun88-started) state changed from pre-start to spawned
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.348254] init: startpar-bridge (network-interface-tun88-started) main process (27069)
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.348277] init: startpar-bridge (network-interface-tun88-started) state changed from spawned to post-start
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.348328] init: startpar-bridge (network-interface-tun88-started) state changed from post-start to running
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.348422] init: Handling started event
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.348731] init: startpar-bridge (network-interface-tun88-started) main process (27069) exited normally
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.348796] init: startpar-bridge (network-interface-tun88-started) goal changed from start to stop
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.348841] init: startpar-bridge (network-interface-tun88-started) state changed from running to stopping
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.348874] init: Handling stopping event
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.348913] init: startpar-bridge (network-interface-tun88-started) state changed from stopping to killed
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.348934] init: startpar-bridge (network-interface-tun88-started) state changed from killed to post-stop
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.348953] init: startpar-bridge (network-interface-tun88-started) state changed from post-stop to waiting
    Jun  4 20:07:19 dimsdemo1.node.consul kernel: [58064.349059] init: Handling stopped event
    Jun  4 20:07:36 dimsdemo1.node.consul DITTRICH: Done

..

.. code-block:: none


..
