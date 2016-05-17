.. _managingvms:

Managing Virtualbox VMs
=======================

This chapter covers using Virtualbox command line tools,
most importantly ``VBoxManage``,  to manage core DIMS
virtual machines.

.. note::

   See also the descriptions of :ref:`dimsasbuilt:wellington` and
   :ref:`dimsasbuilt:stirling` in :ref:`dimsasbuilt:dimsasbuilt`.

..

.. _remotelymanagevbox:

Remotely Managing Virtualbox
----------------------------

Virtualbox can be managed remotely using X11 ("X Window System") clients
like those in `virt tools`_. From a system running an X11 server, you
can use SSH with:

  * `How to forward X over SSH from Ubuntu machine?`_

  * `Use the virt-manager X11 GUI`_

  * `Use virt-install and connect by using a local VNC client`_

  * `virt-manager won’t release the mouse when using ssh forwarding from OS X`_

.. todo::

    Figure out how to use ``virt-manager`` remotely to control VMs (for use
    on ``time`` where ``eclipse`` is running).

..

.. _virt tools: http://virt-tools.org/index.html
.. _How to forward X over SSH from Ubuntu machine?: http://unix.stackexchange.com/questions/12755/how-to-forward-x-over-ssh-from-ubuntu-machine
.. _Use the virt-manager X11 GUI: http://docs.openstack.org/image-guide/virt-manager.html
.. _Use virt-install and connect by using a local VNC client: http://docs.openstack.org/image-guide/virt-install.html
.. _virt-manager won’t release the mouse when using ssh forwarding from OS X: https://major.io/2013/03/20/virt-manager-wont-release-the-mouse-when-using-ssh-forwarding-from-os-x/

.. code-block:: none

    [root@wellington ~]# VBoxManage list runningvms
    "vpn" {4f6ed378-8a9d-4c69-a380-2c194bc4eae0}
    "foswiki" {8978f52d-1251-4fea-a3d7-8d9a0950bad1}
    "lapp" {511b9f91-9323-476e-baf3-9bc64f97511e}
    "jira" {c873db45-b81a-47fe-a5e3-6bdfe96b0dea}
    "jenkins" {28e023eb-f4c4-40f5-b4e8-d37cfafde3be}
    "linda-vm1" {df5fdc5e-d508-4007-9f5d-84a000a2b5c5}
    "sso" {3916fa49-d251-4ced-9275-c8757aceaf66}
    "u12-dev-ws-1" {9f58eca0-b3a6-451e-9b2b-f458c75d6869}
    "u12-dev-svr-1" {cc1fefa3-61f4-4d67-b767-1f4add8f760a}
    "hub" {4b530a22-df34-4fd2-89df-2e0a5844b397}

..

.. code-block:: none

    [lparsons@wellington ~]$ vboxmanage list bridgedifs
    Name:            em1
    GUID:            00316d65-0000-4000-8000-f04da240a9e1
    DHCP:            Disabled
    IPAddress:       172.28.234.234
    NetworkMask:     255.255.255.0
    IPV6Address:     fe80:0000:0000:0000:f24d:a2ff:fe40:a9e1
    IPV6NetworkMaskPrefixLength: 64
    HardwareAddress: f0:4d:a2:40:a9:e1
    MediumType:      Ethernet
    Status:          Up
    VBoxNetworkName: HostInterfaceNetworking-em1

    Name:            em2
    GUID:            00326d65-0000-4000-8000-f04da240a9e3
    DHCP:            Disabled
    IPAddress:       0.0.0.0
    NetworkMask:     0.0.0.0
    IPV6Address:
    IPV6NetworkMaskPrefixLength: 0
    HardwareAddress: f0:4d:a2:40:a9:e3
    MediumType:      Ethernet
    Status:          Down
    VBoxNetworkName: HostInterfaceNetworking-em2

    Name:            em3
    GUID:            00336d65-0000-4000-8000-f04da240a9e5
    DHCP:            Disabled
    IPAddress:       0.0.0.0
    NetworkMask:     0.0.0.0
    IPV6Address:
    IPV6NetworkMaskPrefixLength: 0
    HardwareAddress: f0:4d:a2:40:a9:e5
    MediumType:      Ethernet
    Status:          Down
    VBoxNetworkName: HostInterfaceNetworking-em3

    Name:            em4
    GUID:            00346d65-0000-4000-8000-f04da240a9e7
    DHCP:            Disabled
    IPAddress:       10.11.11.1
    NetworkMask:     255.255.255.0
    IPV6Address:     fe80:0000:0000:0000:f24d:a2ff:fe40:a9e7
    IPV6NetworkMaskPrefixLength: 64
    HardwareAddress: f0:4d:a2:40:a9:e7
    MediumType:      Ethernet
    Status:          Up
    VBoxNetworkName: HostInterfaceNetworking-em4

..

.. code-block:: none

    [lparsons@wellington ~]$ vboxmanage list hostonlyifs
    Name:            vboxnet0
    GUID:            786f6276-656e-4074-8000-0a0027000000
    DHCP:            Disabled
    IPAddress:       192.168.88.0
    NetworkMask:     255.255.255.0
    IPV6Address:     fe80:0000:0000:0000:0800:27ff:fe00:0000
    IPV6NetworkMaskPrefixLength: 64
    HardwareAddress: 0a:00:27:00:00:00
    MediumType:      Ethernet
    Status:          Up
    VBoxNetworkName: HostInterfaceNetworking-vboxnet0

    Name:            vboxnet1
    GUID:            786f6276-656e-4174-8000-0a0027000001
    DHCP:            Disabled
    IPAddress:       192.168.57.1
    NetworkMask:     255.255.255.0
    IPV6Address:
    IPV6NetworkMaskPrefixLength: 0
    HardwareAddress: 0a:00:27:00:00:01
    MediumType:      Ethernet
    Status:          Down
    VBoxNetworkName: HostInterfaceNetworking-vboxnet1

    Name:            vboxnet2
    GUID:            786f6276-656e-4274-8000-0a0027000002
    DHCP:            Disabled
    IPAddress:       192.168.58.1
    NetworkMask:     255.255.255.0
    IPV6Address:
    IPV6NetworkMaskPrefixLength: 0
    HardwareAddress: 0a:00:27:00:00:02
    MediumType:      Ethernet
    Status:          Down
    VBoxNetworkName: HostInterfaceNetworking-vboxnet2

    Name:            vboxnet3
    GUID:            786f6276-656e-4374-8000-0a0027000003
    DHCP:            Disabled
    IPAddress:       172.17.8.1
    NetworkMask:     255.255.255.0
    IPV6Address:     fe80:0000:0000:0000:0800:27ff:fe00:0003
    IPV6NetworkMaskPrefixLength: 64
    HardwareAddress: 0a:00:27:00:00:03
    MediumType:      Ethernet
    Status:          Up
    VBoxNetworkName: HostInterfaceNetworking-vboxnet3

..


.. code-block:: none

    [lparsons@wellington ~]$ sudo vboxmanage list dhcpservers
    NetworkName:    HostInterfaceNetworking-vboxnet0
    IP:             192.168.88.100
    NetworkMask:    255.255.255.0
    lowerIPAddress: 192.168.88.102
    upperIPAddress: 192.168.88.254
    Enabled:        Yes

    NetworkName:    HostInterfaceNetworking-vboxnet2
    IP:             0.0.0.0
    NetworkMask:    0.0.0.0
    lowerIPAddress: 0.0.0.0
    upperIPAddress: 0.0.0.0
    Enabled:        No

    NetworkName:    HostInterfaceNetworking-vboxnet1
    IP:             0.0.0.0
    NetworkMask:    0.0.0.0
    lowerIPAddress: 0.0.0.0
    upperIPAddress: 0.0.0.0
    Enabled:        No

..


.. todo::

    Write up instructions on how to use ``virtualbox`` graphical manager remotely
    to control VMs (for use on ``wellington`` where ``lancaster``, ``jira``, etc.
    are running).

..

http://superuser.com/questions/375316/closing-gui-session-while-running-virtual-mashine-virtual-box


