.. _appendices:

Appendices
==========

.. _apacheDirectoryStudioSetup:

Add New Connection to Apache Directory Studio
---------------------------------------------

.. note::

    These instructions are based on contents from this original DIMS project
    `FosWiki Provision New Users`_ page.

..

.. note::

    We are in the process of moving to a "split-horizon DNS" configuration using
    the subdomains ``ops.develop`` and/or ``devops.develop`` as opposed to the
    original monolithic domain ``prisem.washington.edu`` that was being overlayed
    with both routable and non-routable IP address mappings.  As a result, some
    configuration using the original ``prisem.washington.edu`` domain remains,
    such as the **DN** entry information shown below.

..

If you have never connected to our LDAP before, you will need to add the
connection to Apache Directory Studio (``apache-directory-studio``).
You can see your saved connections in the Connections tab.  To add a new
connection, do the following:

  #. On the LDAP menu, select **New Connection**. The **Network Parameter**
     dialog will display.

     .. figure:: images/apache-directory-studio-newconnection.png
        :width: 85%
        :align: center

        Entering Network Parameters

     ..

     #. Enter a name for the connection. Use ``ldap.devops.develop``
     #. Enter hostname: ``ldap.devops.develop``
     #. Port should be ``389``
     #. No encryption

  #. You can click **Check Nework Parameter** to check the connection

  #. Click **Next**. The **Authentication** dialog will display.


     .. figure:: images/apache-directory-studio-connect.png
        :width: 85%
        :align: center

        LDAP Connection Authentication

     ..

     #. Leave **Authentication Method** as **Simple Authentication**
     #. Bind DN or user: ``cn=admin,dc=prisem,dc=washington,dc=edu``
     #. Bind password: [See the `FosWiki Provision New Users`_ page for password.]
     #. Click the checkbox to save the password if it is not already checked.
     #. Click the **Check Authentication** button to make sure you can authenticate.

  #. Click **Finish**. The new connection will appear in the **Connections** list and
     will open. If you minimize the **Welcome** window, the **LDAP Brower** window
     will occupy the full application window and will remain visible as you operate
     on the connection.

     .. figure:: images/apache-directory-studio-browser.png
        :width: 85%
        :align: center

        Main LDAP Browser window

     ..


.. _FosWiki Provision New Users: http://foswiki.prisem.washington.edu/Development/ProvisionNewUsers:

.. EOF
