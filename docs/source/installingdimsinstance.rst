.. _installation:

Installation of a Complete DIMS Instance
========================================

The Distributed Incident Management System (DIMS) is a system comprised of
many sub-systems. That is to say, there are many inter-related and
inter-dependent services that work together to provide a coherent whole which
is called a *DIMS instance*. These subsytems may be provided by daemons running
in a normal Linux system running on bare-metal (i.e., an operating system
installed onto a standard computer hardware server), in a virtual machine
running on a bare-metal host, or in Docker containers. Conceptually, it does
not matter what underlying operating system is used, whether it is physical or
virtual, or whether it is a Docker container: DIMS is comprised of
micro-services that communicate using standard TCP/IP connections, regardless
of where those services are running.

This chapter covers the steps necessary to install and configure a DIMS
instance using (a) a single server running a cluster comprised of three virtual
machines, and (b) a three-node bare-metal cluster.

.. _clusterSetup:

Cluster Foundation Setup
------------------------

To bootstrap a DIMS instance, it is necessary to first install the required
base operating system, pre-requisite packages, and software components that
serve as the foundation for running the DIMS micro-services. This includes the
DIMS software and configuration files that differentiate one DIMS instance from
another on the network. Each instance lives on its own IP address block, has
its own DNS domain, and is branded uniquely for its owners. Configuration for
the specific deployment is applied, resulting in a functioning DIMS instance
that can be accessed from the internet by system administrators, who in turn
bootstrap user accounts to start using DIMS.

.. _singleHostVirtualCluster:

Single-host Virtual Machine Cluster Configuration
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. todo::

    Write up instructions on how to deploy a three-node CoreOS
    cluster using Virtualbox on a Debian Linux host operating
    system using a single compute server.

    #. Install base operating system ala the dev laptops
       with Ubuntu ``kickstart`` (or similar).

    #. Install core pre-requisite packages (including
       Virtualbox, etc.)

    #. Configure to suit to be functional on the network.

    #. Generate three VMs to create a CoreOS cluster on
       the single host using its IP address as the entry
       point (using port forwarding as needed to map
       external IP address/port combinations to appropriate
       internal IP address/port for micro-services.)

..

.. _threeNodeBareMetalCluster:

Three-node Bare Metal Cluster Configuration
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. todo::

    Write up instructions on how to deploy a three-node CoreOS
    cluster on bare-metal hosts. This requires three servers
    and one to three IP addresses (depending on ability to do
    NAT forwarding from two of the three servers).

    #. Prepare three ``cloud-config`` files for each of the
       three CoreOS servers in the cluster.

    #. Install CoreOS using bootable Linux ISO that references
       ``cloud-config`` files by MAC address or other unique
       identifier obtainable from the live-Linux OS).

..

.. _bootstrappingusers:

Bootstrapping User Base
-----------------------

.. todo::

    Create initial system administration accounts to be able to
    bootstrap the initial user base.

    #. ...

..

.. EOF
