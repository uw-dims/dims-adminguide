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
another on the network.

Each DIMS instance has a routable Internet connection from at least one
node and an internal local area network on which the DIMS system components
are connected on the back end. This means there is at least one IP address
block that is shared on the back, regardless of whether the primary node
has its own DNS domain and Internet accessible IP address (as would be
the case for a production service deployment) or uses dynamic addressing
on WiFi or wired interface for a local development deployment.

A DIMS deployment that is to be used for public facing services on the
Internet requires a real DNS domain and routable IP address(es), with
SSL certificates to secure the web application front end. To remotely
administer the system requires setting up SSH keys for secure remote
access and/or remote administration using Ansible.

Accounts in the Trident user portal can be set up from the command
line using the ``tcli`` user interface, or by using the Trident
web application front end.

Single-host Virtual Machine Deployment
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


.. _bootstrappingusers:

Bootstrapping User Base
-----------------------

.. todo::

    Create initial system administration accounts to be able to
    bootstrap the initial user base.

    #. ...

..

.. EOF
