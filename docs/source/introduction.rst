.. _introduction:

Introduction 
============

This chapter introduces the system administration policies,
methodology for configuration file management, automated
installation and configuration of DIMS components using
Ansible, and use of continuous integration mechanisms
used for deployment and testing of DIMS components.

This document is closely related to the :ref:`dimsdevguide:dimsdeveloperguide`,
which covers a number of related tasks and steps that will not be repeated here
(rather, will be cross-referenced using `intersphinx`_ links.)

.. _intersphinx: http://sphinx-doc.org/latest/ext/intersphinx.html

+ All documentation for the DIMS project is written using restructured text
  (reST) and Sphinx. Section :ref:`dimsdevguide:documentation` of the
  :ref:`dimsdevguide:dimsdeveloperguide` covers how to use these
  tools for producing professional looking and cross-referenced on-line (HTML)
  and off-line (PDF) documentation.

+ DIMS software -- including Ansible playbooks for installation and configuration
  of DIMS system components, Packer, Vagrant, and Docker subsystem creation
  scripts, are all maintained under version control using Git and the HubFlow
  methodology and tool set. Section :ref:`dimsdevguide:sourcemanagement` of the
  :ref:`dimsdevguide:dimsdeveloperguide` covers how these tools are used for
  source code, documentation, and system configuration files.

+ Changes to source code that are pushed to Git repositories trigger build
  processes using the Jenkins continuous integration environment.  These triggers
  build and/or deploy software to specified locations, run tests, and/or
  configure service components. In most cases, Ansible is used as part of the
  process driven by Jenkins.  Section :ref:`dimsdevguide:continuousintegration`
  of the :ref:`dimsdevguide:dimsdeveloperguide` provides an overview of how
  this works and how to use it in development and testing DIMS components.

+ System software installation and configuration of DIMS components are managed
  using Ansible playbooks that are in turn maintained in Git repositories. Only
  a bare minimum of manual steps are required to bootstrap a DIMS deployment.
  After that, configuration changes are made to Git repositories and those
  changes trigger continuous integration processes to get these changes into
  the running system.  Section :ref:`deployconfigure` of the
  :ref:`dimsdevguide:dimsdeveloperguide` covers how to use this
  framework for adding or managing the open source components that are used
  in a DIMS deployment.

Overview
--------

This document is focused on the system administrative tasks that are involved
in adding open source software components to the DIMS framework, how to
convert installation instructions into Ansible playbooks or ``Dockerfile``
instructions that can be used to instantiate a service or microservice,
how a complete DIMS instance (i.e., a complementary set of service
and microservice components that function together as a coherent
system) is installed, configured, debugged and/or tuned, and kept in running
order over time.

