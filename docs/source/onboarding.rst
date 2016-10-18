.. _onboarding:

Onboarding Developers
=====================

This chapter covers the process for onboarding new developers
into the DIMS project.

.. _initialaccountsetup:

Initial Account Setup
---------------------

The first step in adding a new DIMS developer is getting them set up
with an account on our internal ``ops-trust`` portal instance.

Our FosWiki server has a page that was dedicated to the steps necessary
for `Provisioning New DIMS Users`_.

.. _Provisioning New DIMS Users: http://foswiki.devops.develop/Development/ProvisionNewUsers

.. caution::

    The FosWiki page `Provisioning New DIMS Users`_ looks like it may be out of
    date, or include steps that may not be necessary for just adding a new
    user. It has a huge number of steps that should be made more streamlined
    or added to the DIMS web app to simplify the process of adding and removing
    DIMS users in concert with the ``ops-trust`` portal at the center of DIMS.

..

Once the user has been given their password to the ``ops-trust`` portal,
they need to change their ``MemberID`` to match the account name that
should be used within DIMS. (E.g., Dave Dittrich may be given the
``MemberID`` of ``davedittrich2475`` by the portal, but the desired
account name within DIMS subsystems should be ``dittrich``.)

GPG Encryption Keys for Email, etc.
-----------------------------------

Each ``ops-trust`` portal account holder needs a GPG key to be able to
send/receive encrypted emails. In normal operation, one's ``ops-trust``
portal account is not fully enabled until the user has uploaded their
GPG key.

One of the easiest ways to process GPG-encrypted email is using `Enigmail`_
with the `The GNU Privacy Guard`_ from the `Thunderbird`_ email client. Follow
the `Enigmail Quick Start Guide`_ to install, configure, and generate a GPG key
for use with `Thunderbird`_ (which is supported on Mac, Linux, and Windows, and
is installed by default on the DIMS Ubuntu developer laptops).

After you have set up `The GNU Privacy Guard`_ and uploaded your key,
log in to the ``ops-trust`` portal and select ``PGP Keys`` from the menu
on the left of the screen to download all GPG keys for other portal
users and all email lists to which you subscribe.

.. note::

    This step will only download keys that are in the system at the time
    you press the link, which means they will get out-of-date with respect
    to new users, regenerated keys, and/or new email lists that may be
    created over time. Get in the habit of updating your GPG key ring
    regularly, or at least remember that failure to encrypt/decrypt
    and email may be due to your keyring being out of date and needing
    a refresh.

..

.. _Enigmail: https://www.enigmail.net/home/index.php
.. _The GNU Privacy Guard: https://www.gnupg.org
.. _Enigmail Quick Start Guide: https://www.enigmail.net/documentation/quickstart.php
.. _Thunderbird: https://www.mozilla.org/en-US/thunderbird/


.. _creatingaccounts:

Creating accounts
-----------------

After a new user has successfully set up their ``ops-trust`` portal
account and modified their ``MemberID`` to align with their desired DIMS
account name, they must be added to the ``dims_users`` array in
the ``$GIT/ansible-playbooks/group_vars/all`` file. Once added,
the Ansible playbook roles that generate DIMS user accounts
(e.g., ``dims-users-create``) can be played to create accounts
as needed.

.. todo::

    This step should be automated, so that adding (or removing) a user
    from the DIMS system triggers account creation (or archiving and deletion)
    automatically on DIMS-OPS systems. DIMS-DevOps systems may need to have
    this step be done manually, or triggered by adding/removing the user to/from
    one of the DevOps related trust groups.

    One method for automating this is to replace the hard-coded ``dims_users`` variable
    with a Jinja2 template, then extract the list of users from the ``ops-trust`` portal
    via an RPC call from a Python script that performs the Jinja2 template substitution
    immediately before running Ansible playbooks to create users. (A separate step
    to archive/delete terminated user accounts would be triggered by deletion
    events in the ``ops-trust`` portal, or from an administration panel accessible
    only to users in the "admin" group in the DIMS web application.)

    See :ref:`dimssr:dimssystemrequirements` sections :ref:`dimssr:accountAccessControls`,
    :ref:`dimssr:diutUserStory5`, and :ref:`dimssr:accountSuspension`, and
    Jira Epic `DIMS-64`_.

..

.. _DIMS-64: http://jira.devops.develop/browse/DIMS-64


.. _sshkeys:

Installing initial SSH key(s)
-----------------------------

Before someone can clone Git repositories, or use SSH to log in to DIMS
systems for interactive shell access, they must (a) have a DIMS SSH key,
and (b) have the public key and ``authorized_keys`` file(s) on target
systems set up properly.

#. Create the user's DIMS SSH key pair...

   .. todo::

      Replace this with example...

      make key
      git commit it

   ..

#. Generate accounts using Ansible playbook ($whatever), which creates
   the accounts and installs their public key.

#. Copy their key pair into the account on the system where they will be
   doing their development (i.e., a DIMS developer laptop, Vagrant virtual
   machine, or bare-metal workstation.) Also make sure their key is
   included in the ``authorized_keys`` file in the ``git`` account on
   ``git.devops.develop`` in order for them to be able to read/write
   source code using Git.

   .. todo::

      Replace this with example...

   ..

#.  Trigger a Jenkins build job for `public-keys-configure`_ to push the
    new user's key to all DIMS-DevOps and DIMS-OPS systems.


    .. todo::

        This step could be added to a script, which invokes the job using
        the REST API. That saves the manual step of clicking the
        *Build Now* button in a browser.

    ..


#.  Set the password on the account they are supposed to use so they can log in to it,
    and/or securely transfer their public SSH key to them so they can use it
    to access the account without needing a password.

    .. note::

        They will need a password on the account for ``sudo`` on commands
        like ``dims-ci-utils.install.user`` that ask for the ``sudo``
        password in order to pass it to Ansible.

    ..

   Use command ``passwd <username>``.

   .. code-block:: none

      [dimsenv] mboggess@b52:~ () $ passwd mboggess
      Changing password for mboggess.
      (current) UNIX password: 
      Enter new UNIX password: 
      Retype new UNIX password: 
      passwd: password updated successfully

   ..

.. _public-keys-configure: http://jenkins.devops.develop/job/public-keys-configure/

.. _remoteaccountsetup:

Remote Account Setup
--------------------

This section details how to set up a new account for a current developer
on a remote machine, after being logged in to the remote machine.

Change password
~~~~~~~~~~~~~~~

Use command ``passwd <username>``.

.. code-block:: none

   [dimsenv] mboggess@b52:~ () $ passwd mboggess
   Changing password for mboggess.
   (current) UNIX password: 
   Enter new UNIX password: 
   Retype new UNIX password: 
   passwd: password updated successfully

..

Transfer SSH Keys to Remote Machine
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*  Once logged in to remote machine, check ~/.ssh/authorized_keys file 
   for public key:

   .. code-block:: none

      [dimsenv] mboggess@b52:~ () $ cd .ssh
      [dimsenv] mboggess@b52:~/.ssh () $ ls
      authorized_keys  config  known_hosts
      [dimsenv] mboggess@b52:~/.ssh () $ vim authorized_keys

   ..

*  Securely transfer DIMS RSA keys from local machine to remote machine

   Keys are located in ``~/.ssh/`` and should be named:

   * ``dims_${dimsusername}_rsa`` for private key
   * ``dims_${dimsusername}rsa.pub`` for public key
   * ``dims_${dimsusername}_rsa.sig`` for signature

   Copy all three files from local machine with DIMS RSA keys:

   .. code-block:: none

      [dimsenv] mboggess@dimsdev2:~ () $ cd .ssh
      [dimsenv] mboggess@dimsdev2:~/.ssh () $ scp dims_mboggess_rsa* mboggess@b52.tacoma.uw.edu:/home/mboggess/.ssh/
      dims_mboggess_rsa                                     100% 1675     1.6KB/s   00:00
      dims_mboggess_rsa.pub                                 100%  403     0.4KB/s   00:00
      dims_mboggess_rsa.sig                                 100%   82     0.1KB/s   00:00

   ..

   Check on remote machine:

   .. code-block:: none

      [dimsenv] mboggess@b52:~/.ssh () $ ls
      authorized_keys  dims_mboggess_rsa      dims_mboggess_rsa.sig
      config           dims_mboggess_rsa.pub  known_hosts

   ..

.. note::

   This solves the "second hop issue": a user can access machines one hop
   away because the necessary keys are available on their local machine,
   but when trying to go one hop further, keys are not available. For 
   example, I can log in to b52 just fine, but when I try to run 
   ``dims.git.syncrepos``, which requires access to 
   ``git.devops.develop``, I ran into trouble because my keys 
   were not on b52.

..

Sync Repos on Remote Machine
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

There probably will not be a .mrconfig file on the remote machine,
so you must create an empty file with that name before you sync repos
or the command will fail.

Failure when running ``dims.git.syncrepos`` because no .mrconfig:

.. code-block:: none

   <snip>

   [+++] Adding Repo[49] umich-botnets to /home/mboggess/dims/.mrconfig and checking it out.
   cp: cannot stat ‘/home/mboggess/dims/.mrconfig’: No such file or directory

   [+++] Updated 49 of 49 available repos.
   [+++] Summary of actions for repos that were updated:
   - Any changes to branches at origin have been downloaded to your local repository
   - Any branches that have been deleted at origin have also been deleted from your local repository
   - Any changes from origin/master have been merged into branch 'master'
   - Any changes from origin/develop have been merged into branch 'develop'
   - Any resolved merge conflicts have been pushed back to origin
   [+++] Added 49 new repos: ansible-inventory ansible-playbooks cif-client cif-java configs dims dims-ad dims-adminguide dims-asbuilt dims-ci-utils dims-dashboard dims-db-recovery dims-devguide dims-dockerfiles dims-dsdd dims-jds dims-keys dims-ocd dims-packer dims-parselogs dims-sample-data dims-sr dims-supervisor dims-svd dimssysconfig dims-test-repo dims-tp dims-tr dims-vagrant ELK fuse4j ipgrep java-native-loader java-stix-v1.1.1 mal4s MozDef ops-trust-openid ops-trust-portal poster-deck-2014-noflow prisem prisem-replacement pygraph rwfind sphinx-autobuild stix-java ticketing-redis tsk4j tupelo umich-botnets
   [+++] Updating repos took 00:00:00

..

Looking in ~/dims/ for .mrconfig:

.. code-block:: none

   [dimsenv] mboggess@b52:~ () $ cd dims
   [dimsenv] mboggess@b52:~/dims () $ ls -a
   .  ..  git

..

*  Create .mrconfig

   .. code-block:: none
 
      [dimsenv] mboggess@b52:~/dims () $ touch .mrconfig
      [dimsenv] mboggess@b52:~/dims () $ ls -a
      .  ..  git  .mrconfig
   
   ..

*  Run dims.git.syncrepos

   .. code-block:: none
   
      [dimsenv] mboggess@b52:~/dims () $ cd ..
      [dimsenv] mboggess@b52:~ () $ dims.git.syncrepos
      [+++] Found 49 available repos at git@git.devops.develop
      [+++] Adding Repo[1] ansible-inventory to /home/mboggess/dims/.mrconfig and checking it out.
      mr checkout: /home/mboggess/dims/git/ansible-inventory
      Cloning into 'ansible-inventory'...
      remote: Counting objects: 481, done.
      remote: Compressing objects: 100% (387/387), done.
      remote: Total 481 (delta 237), reused 122 (delta 65)
      Receiving objects: 100% (481/481), 62.36 KiB | 0 bytes/s, done.
      Resolving deltas: 100% (237/237), done.
      Checking connectivity... done.
      Using default branch names.
      
      Which branch should be used for tracking production releases?
         - master
      Branch name for production releases: [master]
      Branch name for "next release" development: [develop]
      
      How to name your supporting branch prefixes?
      Feature branches? [feature/]
      Release branches? [release/]
      Hotfix branches? [hotfix/]
      Support branches? [support/]
      Version tag prefix? []
      
      mr checkout: finished (1 ok)
      
      <snip>
      
      [+++] Updated 49 of 49 available repos.
      [+++] Summary of actions for repos that were updated:
      - Any changes to branches at origin have been downloaded to your local repository
      - Any branches that have been deleted at origin have also been deleted from your local repository
      - Any changes from origin/master have been merged into branch 'master'
      - Any changes from origin/develop have been merged into branch 'develop'
      - Any resolved merge conflicts have been pushed back to origin
      [+++] Added 49 new repos: ansible-inventory ansible-playbooks cif-client cif-java configs dims dims-ad dims-adminguide dims-asbuilt dims-ci-utils dims-dashboard dims-db-recovery dims-devguide dims-dockerfiles dims-dsdd dims-jds dims-keys dims-ocd dims-packer dims-parselogs dims-sample-data dims-sr dims-supervisor dims-svd dimssysconfig dims-test-repo dims-tp dims-tr dims-vagrant ELK fuse4j ipgrep java-native-loader java-stix-v1.1.1 mal4s MozDef ops-trust-openid ops-trust-portal poster-deck-2014-noflow prisem prisem-replacement pygraph rwfind sphinx-autobuild stix-java ticketing-redis tsk4j tupelo umich-botnets
      [+++] Updating repos took 00:07:19
   
   ..

Build Python Virtual Environment on Remote Machine
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*  When logged in to remote machine, change directories to location of
   virtual environment build scripts: 

   .. code-block:: none
      
      [dimsenv] mboggess@b52:~ () $ cd $GIT/ansible-playbooks
   
   ..

*  Run the DIMS command to build the system virtualenv for access to
    system DIMS commands:

   .. code-block:: none
   
      [dimsenv] mboggess@b52:~/dims/git/ansible-playbooks (develop) $ ./dimsenv.install.system
   
   ..

*  Run ``exec bash`` to refresh:

   .. code-block:: none
   
      [dimsenv] mboggess@b52:~/dims/git/ansible-playbooks (develop) $ exec bash
      [+++] DIMS shell initialization [ansible-playbooks v1.2.107]
      [+++] Sourcing /opt/dims/etc/bashrc.dims.d/bashrc.dims.network ...
      [+++] OpenVPN status:
       * VPN '01_uwapl_daveb52' is running
       * VPN '02_prsm_dave-prisem-2' is running
      [+++] Sourcing /opt/dims/etc/bashrc.dims.d/bashrc.dims.virtualenv ...
      [+++] Activating virtual environment (/home/mboggess/dims/envs/dimsenv) [ansible-playbooks v1.2.107]
      [+++] (Create file /home/mboggess/.DIMS_NO_DIMSENV_ACTIVATE to disable)
      [+++] Virtual environment 'dimsenv' activated [ansible-playbooks v1.2.107]
      [+++] Installed /home/mboggess/dims/envs/dimsenv/bin/dimsenv.install.user
      [+++] Installed /home/mboggess/dims/envs/dimsenv/bin/dimsenv.install.system
      [+++] Sourcing /opt/dims/etc/bashrc.dims.d/git-prompt.sh ...
      [+++] Sourcing /opt/dims/etc/bashrc.dims.d/hub.bash_completion.sh ...
   
   ..
   
   Line "Activating virtual environment" should have path to dimsenv/ 
   via $HOME/dims.

*  Run DIMS command to build user virtualenv:

   .. code-block:: none
   
      [dimsenv] mboggess@b52:~/dims/git/ansible-playbooks (develop) $ ./dimsenv.install.user
   
   ..

*  Run ``exec bash`` to refresh again.

*  Check $HOME/dims/envs/ for dimsenv/ and activation scripts:

   .. code-block:: none
   
      [dimsenv] mboggess@b52:~/dims/git/ansible-playbooks (develop) $ ls $HOME/dims/envs
      dimsenv          initialize    postdeactivate  postmkvirtualenv  preactivate    premkproject     prermvirtualenv
      get_env_details  postactivate  postmkproject   postrmvirtualenv  predeactivate  premkvirtualenv
   
   ..

Transfer Config Files
~~~~~~~~~~~~~~~~~~~~~

*  Your account personalization files need to be transferred to the
   remote machine as well, including .gitconfig, .vimrc, and 
   .bash_aliases.


   From the local machine:

   .. code-block:: none
   
      [dimsenv] mboggess@dimsdev2:~ () $ scp .bash_aliases mboggess@b52.tacoma.uw.edu:/home/mboggess/
      .bash_aliases                                 100%  510     0.5KB/s   00:00    
      [dimsenv] mboggess@dimsdev2:~ () $ scp .gitconfig mboggess@b52.tacoma.uw.edu:/home/mboggess/
      .gitconfig                                    100%  847     0.8KB/s   00:00    
      [dimsenv] mboggess@dimsdev2:~ () $ scp .vimrc mboggess@b52.tacoma.uw.edu:/home/mboggess/
      .vimrc                                        100%  314     0.3KB/s   00:00      
      
   ..

   On the remote machine, check for files and refresh bash:

   .. code-block:: none

      [dimsenv] mboggess@b52:~ () $ ls -a
      .   .ansible       .bash_history  .bashrc  dims              .gitconfig  .profile      .ssh      .vimrc
      ..  .bash_aliases  .bash_logout   .cache   examples.desktop  .mrtrust    .python-eggs  .viminfo
      [dimsenv] mboggess@b52:~ () $ exec bash
      
   ..
